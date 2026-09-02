#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QProcess>
#include <QCoreApplication>
#include <QUrl>
#include <QQmlFile>

#include <QSettings>

// #include "resampler.h"

#include "irworker.h"

#include "uicore.h"

#ifdef Q_OS_ANDROID
#include <jni.h>
#include "activityresultmanager.h"

ActivityResultManager activityResultHandler;
#endif

#ifdef Q_OS_IOS
#include "Mobile/ios/src/iosutils.hpp"
#endif

UiCore* UiCore::instance = nullptr;

UiCore::UiCore(QObject *parent)
    : QObject{parent}
{
    m_currentDevice = &dummyDevice;

#if defined(Q_OS_ANDROID)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat, this);

    connect(&activityResultHandler, &ActivityResultManager::sgIrFilePicked, this, qOverload<QString, QString>(&UiCore::uploadIr));
    connect(&activityResultHandler, &ActivityResultManager::sgIrFileListPicked, this, qOverload<QList<QUrl>, QUrl>(&UiCore::uploadIr));
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFilePicked, this, &UiCore::slImportPreset);
    connect(&activityResultHandler, &ActivityResultManager::sgFirmwareFilePicked, this, &UiCore::slFirmwareFilePicked);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFileCreated, this, &UiCore::slExportPreset);
#elif defined(Q_OS_IOS)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.plist", QSettings::NativeFormat, this);

    IosUtils::wakeLockDisable();
#else
    appSettings = new QSettings(QSettings::UserScope);
#endif
}

UiCore::~UiCore()
{
#ifdef Q_OS_ANDROID
    QString outFolder = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0) +
                        "/AMT/pangaea_mobile/convertedIR/";
#elif defined(Q_OS_MACOS)
    QString outFolder = QDir::homePath() + "/Documents" + "/AMT/Pangaea-desktop/convertedIR/";
#else
    QString outFolder = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) +
                        "/AMT/Pangaea-desktop/convertedIR/";
#endif

    QDir tmpDir(outFolder);
    tmpDir.removeRecursively();
}

void UiCore::disconnectFromDevice()
{
    disconnect(m_currentDevice);
    m_currentDevice = &dummyDevice;
    emit currentDeviceChanged();
    emit sgDisconnectFromDevice();
}


void UiCore::uploadIr(QString srcFilePath, QString dstFilePath)
{
    qDebug() << __FUNCTION__ << "QString, QString";

#ifndef Q_OS_ANDROID
    m_dstIrPath = dstFilePath;
#endif

#ifdef Q_OS_IOS
    IosUtils::copyFileToTmp(srcFilePath, m_pickedIrPath);
#else
    m_pickedIrPath = srcFilePath;
#endif
    QFileInfo fileInfo(m_pickedIrPath);
    QString fileName = fileInfo.fileName();

    bool autoTrim = appSettings->value("auto_trim_wav", false).toBool();
    bool autoConvert = appSettings->value("auto_convert_wav", false).toBool();

    if(autoConvert)
    {
        stWavHeader wavHead = IRWorker::getFormatWav(srcFilePath);

        if((wavHead.sampleRate != 48000) || (wavHead.bitsPerSample != 24) || (wavHead.numChannels != 1))
        {
            m_pickedIrPath = IRWorker::convertWav(m_pickedIrPath);
        }
    }

    if((m_currentDevice->deviceType() > DeviceType::LEGACY_DEVICES) && !autoTrim)
    {
        if(fileInfo.size() > m_currentDevice->maxIrSize())
        {
            emit sgUiMessage(UiMessageType::PROPOSE_IR_TRIM, "File is bigger than processing IR", {fileName, m_pickedIrPath, m_dstIrPath});
            return;
        }
    }

    m_currentDevice->startIrUpload(m_pickedIrPath, m_dstIrPath, autoTrim);
}

void UiCore::uploadIr(QUrl srcFilePath, QUrl dstFilePath)
{
#ifdef Q_OS_ANDROID
    m_dstIrPath = dstFilePath.path();
    pickFile(ActivityType::PICK_IR, "audio/*");
    return;
#else

    QString filePath =  srcFilePath.path();
#ifdef Q_OS_WINDOWS
    filePath.remove(0, 1); // remove first absolute '/' symbol
#endif
    m_dstIrPath = dstFilePath.path();

    qDebug() << filePath << " to " << m_dstIrPath;
    uploadIr(filePath, m_dstIrPath);
#endif
}

void UiCore::uploadIr(QList<QUrl> fileList, QUrl dstFilePath)
{
    m_uploadFileList = fileList;

#ifndef Q_OS_ANDROID
    m_dstIrPath = dstFilePath.path();
#endif
    impulseUploaded();
}

void UiCore::convertAndUploadIr()
{
    QString outpuFilePath = IRWorker::convertWav(m_pickedIrPath);

    m_currentDevice->startIrUpload(outpuFilePath, m_dstIrPath);
}

void UiCore::exportPreset(QUrl dstPath)
{
#ifdef Q_OS_ANDROID
    QJniObject ACTION_CREATE_DOCUMENT = QJniObject::getStaticObjectField<jstring>("android/content/Intent", "ACTION_CREATE_DOCUMENT");
    QJniObject CATEGORY_OPENABLE = QJniObject::getStaticObjectField<jstring>("android/content/Intent", "CATEGORY_OPENABLE");
    QJniObject EXTRA_TITLE = QJniObject::getStaticObjectField<jstring>("android/content/Intent", "EXTRA_TITLE");

    jint FLAG_READ_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_READ_URI_PERMISSION");
    jint FLAG_WRITE_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_WRITE_URI_PERMISSION");
    jint FLAG_PERSISTABLE_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_PERSISTABLE_URI_PERMISSION");

    QJniObject intent("android/content/Intent");
    if (ACTION_CREATE_DOCUMENT.isValid() && intent.isValid())
    {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", ACTION_CREATE_DOCUMENT.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString("audio/*").object<jstring>());


        intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                EXTRA_TITLE.object<jstring>(),
                                QJniObject::fromString("Pangaea_preset.pst").object<jstring>());

        intent.callObjectMethod("addCategory", "(Ljava/lang/String;)Landroid/content/Intent;", CATEGORY_OPENABLE.object<jstring>());

        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_READ_PERMISSION);
        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_WRITE_PERMISSION);
        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_PERSISTABLE_PERMISSION);

        QtAndroidPrivate::startActivity(intent, ActivityType::CREATE_PRESET, &activityResultHandler);
    }
// #elif defined(Q_OS_IOS)

#else
    QString filePath = dstPath.path();
    #ifdef Q_OS_WIN
    filePath.remove(0, 1);
    #endif
    m_currentDevice->exportPreset(filePath, dstPath.fileName());
#endif
}

void UiCore::importPreset(QString filePath)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(filePath)

    pickFile(ActivityType::PICK_PRESET, "*/*", false);
#elif defined(Q_OS_IOS)
    QString tmpFilePath;
    IosUtils::copyFileToTmp(filePath, tmpFilePath);
    slImportPreset(tmpFilePath, "");
#else
    slImportPreset(filePath, "");
#endif
}

void UiCore::slExportPreset(QString fullFilePath, QString fileName)
{
    m_currentDevice->exportPreset(fullFilePath, fileName);
}

void UiCore::slImportPreset(QString fullFilePath, QString fileName)
{
    m_currentDevice->importPreset(fullFilePath, fileName);
}

void UiCore::impulseUploaded()
{
    if(m_uploadFileList.isEmpty()) return;

    QUrl fileUrl = m_uploadFileList.takeFirst();
#ifdef Q_OS_ANDROID
    uploadIr(fileUrl.toString(QUrl::FullyEncoded), m_dstIrPath);
#else
    uploadIr(fileUrl, m_dstIrPath);
#endif
}

void UiCore::slFirmwareFilePicked(QString filePath, QString fileName)
{
#ifdef Q_OS_IOS
    QString tmpFilePath;
    IosUtils::copyFileToTmp(filePath, tmpFilePath);

    filePath = tmpFilePath;

    QFileInfo fileInfo(filePath);
    fileName = fileInfo.fileName();
#endif

    emit sgFirmwareFilePicked(filePath, fileName);
}

void UiCore::slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware)
{
    emit sgNewFirmwareAvaliable(updateFirmware->firmwareVersion());
}

void UiCore::openManualExternally(QString fileName)
{
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    if(appLanguage=="autoselect")
    {
        appLanguage = QLocale().name().left(2);
    }

    QString fullFileName =  fileName + "_" + appLanguage + ".pdf";

#ifdef Q_OS_ANDROID
    QString filePath = ":/docs/" + fullFileName;
    QFile pdfFile(filePath);

    if(!pdfFile.exists())
    {
        fullFileName = fileName + ".pdf";
        filePath = ":/docs/" + fullFileName;
        pdfFile.setFileName(filePath);
    }

    QString temporallyPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)+"/" + fullFileName;
    pdfFile.copy(temporallyPath);

    qDebug() << "Final manual path: " << temporallyPath;

    QJniObject::callStaticMethod<void>(
        "com.amtelectronics/JavaFile", "openFileExternally",
        "(Ljava/lang/String;Landroid/content/Context;)V",
        QJniObject::fromString(fullFileName).object<jstring>(),
        QNativeInterface::QAndroidApplication::context());
#elif defined(Q_OS_IOS)
        QString filePath = ":/docs/" + fullFileName;
        QFile pdfFile(filePath);

        if(!pdfFile.exists())
        {
            qDebug() << "Manual finded inresources";
            fullFileName = fileName + ".pdf";
            filePath = ":/docs/" + fullFileName;
            pdfFile.setFileName(filePath);
        }

        QString temporallyPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/" + fullFileName;
        pdfFile.copy(temporallyPath);

        qDebug() << "Final manual path: " << temporallyPath;
        QDesktopServices::openUrl(QUrl::fromLocalFile(temporallyPath));
#elif defined(Q_OS_LINUX)
        QString filePath = QCoreApplication::applicationDirPath() + "/../docs/" + fullFileName;

        QProcess proc;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.remove("LD_LIBRARY_PATH"); // force evince use system librarys

        proc.setProcessEnvironment(env);
        proc.setProgram("evince");
        proc.setArguments(QStringList(filePath));
        proc.startDetached();
#elif defined(Q_OS_MACOS)
    QString filePath =  QCoreApplication::applicationDirPath() + "/../Resources/docs/" + fullFileName;
    qInfo() << __FUNCTION__ << filePath;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
#else
    QString filePath =  QCoreApplication::applicationDirPath() + "/docs/" + fullFileName;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
#endif
}

void UiCore::setModuleName(const QString &newModuleName)
{
    if (m_moduleName == newModuleName)
        return;
    m_moduleName = newModuleName;
    emit sgModuleNameChanged(newModuleName);
}

void UiCore::slCurrentDeviceChanged(AbstractDevice *newDevice)
{
    if(m_currentDevice)
    {
        disconnect(m_currentDevice, &AbstractDevice::impulseUploaded, this, &UiCore::impulseUploaded);
    }

    if(newDevice != nullptr)
    {
        m_currentDevice = newDevice;
        connect(m_currentDevice, &AbstractDevice::impulseUploaded, this, &UiCore::impulseUploaded);
    }
    else
    {
        m_currentDevice = &dummyDevice;
    }

    emit currentDeviceChanged();
}

#ifdef Q_OS_ANDROID
void UiCore::pickFirmwareFile()
{
    pickFile(ActivityType::PICK_FIRMWARE, "*/*", false);
}

void UiCore::pickFile(ActivityType fileType, QString filter, bool allowMultiple)
{
    QJniObject ACTION_OPEN_DOCUMENT = QJniObject::getStaticObjectField<jstring>("android/content/Intent", "ACTION_OPEN_DOCUMENT");
    QJniObject intent("android/content/Intent");

    if(ACTION_OPEN_DOCUMENT.isValid() && intent.isValid())
    {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", ACTION_OPEN_DOCUMENT.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString(filter).object<jstring>());

        if(allowMultiple)
        {
            QJniObject EXTRA_ALLOW_MULTIPLE =  QJniObject::getStaticField<jstring>("android/content/Intent", "EXTRA_ALLOW_MULTIPLE");
            intent.callObjectMethod("putExtra", "(Ljava/lang/String;Z)Landroid/content/Intent;", EXTRA_ALLOW_MULTIPLE.object<jstring>(), true);
        }

        jint FLAG_READ_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_READ_URI_PERMISSION");
        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_READ_PERMISSION);

        jint FLAG_PERSISTABLE_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_PERSISTABLE_URI_PERMISSION");
        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_PERSISTABLE_PERMISSION);

        QtAndroidPrivate::startActivity(intent, fileType, &activityResultHandler);
    }
}
#endif

void UiCore::runIrConvertor()
{

#ifdef Q_OS_WIN
    QProcess irConvertorProcess;
    irConvertorProcess.setWorkingDirectory(QCoreApplication::applicationDirPath());
    irConvertorProcess.setProgram("IrConverter.exe");
    irConvertorProcess.startDetached();
#endif

#ifdef Q_OS_MACOS
    QProcess irConvertorProcess;
    qInfo() << "Run converter" << irConvertorProcess.startDetached(QCoreApplication::applicationDirPath() + "/IrConverter.app");
#endif

#ifdef Q_OS_LINUX
    QProcess irConvertorProcess;
    QString path = QCoreApplication::applicationDirPath() + "/IrConverter";
    qDebug() << "Run converter, path" << path << "result:" << irConvertorProcess.startDetached(path);
#endif
}

quint16 UiCore::apiVersion()
{
#ifdef Q_OS_ANDROID
    jint version = QJniObject::getStaticField<jint>("android/os/Build$VERSION", "SDK_INT");
    return version;
#else
    return 1;
#endif
}
