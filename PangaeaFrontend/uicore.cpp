#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QProcess>
#include <QCoreApplication>
#include <QUrl>

#include <QSettings>

#include "resampler.h"

#include "uicore.h"

#ifdef Q_OS_ANDROID
#include <jni.h>
#include "activityresultmanager.h"

ActivityResultManager activityResultHandler;
#endif

#ifdef Q_OS_IOS
#include "Mobile/ios/src/iosfileutils.hpp"
#endif

UiCore::UiCore(QObject *parent)
    : QObject{parent}
{
    m_currentDevice = &dummyDevice;

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);
    
    connect(&activityResultHandler, &ActivityResultManager::sgIrFilePicked, this, &UiCore::slImpulseFilePicked);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFilePicked, this, &UiCore::slImportPreset);
    connect(&activityResultHandler, &ActivityResultManager::sgFirmwareFilePicked, this, &UiCore::slFirmwareFilePicked);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFileCreated, this, &UiCore::slExportPreset);
#else
    appSettings = new QSettings(QSettings::UserScope);
#endif

    loadDefaultTranslator();
}

void UiCore::setupApplication()
{
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    emit sgSetUIText("application_language", appLanguage);

    QString colorTheme = appSettings->value("color_theme", "dark_orange").toString();
    emit sgSetUIText("color_theme", colorTheme);

    bool isAutoconnectEnabled = appSettings->value("autoconnect_enable").toBool();
    emit sgSetUIParameter("autoconnect_enable", isAutoconnectEnabled);

    bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable", false).toBool();
    emit sgSetUIParameter("check_updates_enable", isCheckUpdatesEnabled);

    bool firstRun = !appSettings->value("first_run", true).toBool();
    emit sgSetUIParameter("first_run", firstRun);
    appSettings->setValue("first_run", false);
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
    m_dstIrPath = dstFilePath;
#ifdef Q_OS_ANDROID
    Q_UNUSED(srcFilePath)
    pickFile(ActivityType::PICK_IR, "audio/*");
    return;
#elif defined(Q_OS_IOS)
    IosFileUtils::copyFileToTmp(srcFilePath, m_pickedIrPath);
#else
    m_pickedIrPath = srcFilePath;
#endif
    QFileInfo fileInfo(m_pickedIrPath);
    QString fileName = fileInfo.fileName();

    if(m_currentDevice->deviceType() > DeviceType::LEGACY_DEVICES)
    {
        if(fileInfo.size() > m_currentDevice->maxIrSize())
        {
            emit sgUiMessage(UiMessageType::PROPOSE_IR_TRIM, "File is bigger than processing IR", {fileName, m_pickedIrPath, m_dstIrPath});
            return;
        }
    }
    m_currentDevice->startIrUpload(m_pickedIrPath, m_dstIrPath, false);
}

void UiCore::convertAndUploadIr(QString srcFilePath, QString dstFilePath)
{
    QFileInfo irFileInfo(m_pickedIrPath);
#ifdef Q_OS_ANDROID
    QString outFolder = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0) +
                        "/AMT/pangaea_mobile/convertedIR/";
#else
    QString outFolder = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) +
                        "/AMT/pangaeaCPPA/convertedIR/";
#endif
    QString tmpFilePath = outFolder+"tmp.wav";

    if(!QDir(outFolder).exists())
    {
        QDir().mkpath(outFolder);
    }
    QString outpuFilePath = outFolder + irFileInfo.fileName();;

    QFile tmpFile;
    tmpFile.setFileName(m_pickedIrPath);
    tmpFile.copy(tmpFilePath);

    Resampler().convertFile(tmpFilePath, outpuFilePath);

    tmpFile.remove(tmpFilePath);

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


        intent.callObjectMethod("putExtra", "(Ljava/lang/String;[Ljava/lang/String;)Landroid/content/Intent;",
                                EXTRA_TITLE.object<jstring>(),
                                QJniObject::fromString("Pangaea_preset.pst").object<jstring>());

        intent.callObjectMethod("addCategory", "(Ljava/lang/String;)Landroid/content/Intent;", CATEGORY_OPENABLE.object<jstring>());

        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_READ_PERMISSION);
        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_WRITE_PERMISSION);
        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_PERSISTABLE_PERMISSION);

        QtAndroidPrivate::startActivity(intent, ActivityType::CREATE_PRESET, &activityResultHandler);
    }
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

    pickFile(ActivityType::PICK_PRESET, "*/*");
#elif defined(Q_OS_IOS)
    QString tmpFilePath;
    IosFileUtils::copyFileToTmp(filePath, tmpFilePath);
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

void UiCore::slFirmwareFilePicked(QString filePath, QString fileName)
{
#ifdef Q_OS_IOS
    QString tmpFilePath;
    IosFileUtils::copyFileToTmp(filePath, tmpFilePath);

    filePath = tmpFilePath;

    QFileInfo fileInfo(filePath);
    fileName = fileInfo.fileName();
#endif

    emit sgSetUIText("firmware_file_picked", filePath + ',' + fileName);
}

void UiCore::setFirmware(QString fullFilePath)
{
    emit sgSetFirmware(fullFilePath);
    //m_currentDevice->setFirmware(fullFilePath);
}

void UiCore::slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware)
{
    emit sgSetUIText("firmware_local_path", updateFirmware->path());
    emit sgSetUIText("new_firmware_avaliable", oldFirmware->firmwareVersion() + ',' + updateFirmware->firmwareVersion());
}

void UiCore::slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware)
{
    emit sgSetUIText("firmware_version_error",
                     actualFirmware->firmwareVersion()+","+minimalFirmware->firmwareVersion());
    emit sgSetUIText("firmware_local_path", minimalFirmware->path());
}

// TODO прямой запуск сигнала из QML
void UiCore::doOnlineFirmwareUpdate()
{
    emit sgDoOnlineFirmwareUpdate();
}

void UiCore::saveSetting(QString settingName, QVariant settingValue)
{
    appSettings->setValue(settingName, settingValue);
    appSettings->sync();

    qInfo() << __FUNCTION__ << "Setting name: " << settingName << "Setting value:" << settingValue;
}

void UiCore::setLanguage(QString languageCode)
{
    appSettings->setValue("application_language", languageCode);
    appSettings->sync();

    loadTranslator(languageCode);
}

void UiCore::loadTranslator(QString languageCode)
{
    if(QCoreApplication::removeTranslator(&m_translator)) qDebug() << "Old translator removed";

    if(languageCode=="autoselect")
    {
        loadDefaultTranslator();
        return;
    }

    if (m_translator.load(pathFromCode.value(languageCode)))
    {
        qDebug() << "Translator loaded. Language: " << m_translator.language();
        QCoreApplication::installTranslator(&m_translator);

        emit sgTranslatorChanged(languageCode);
        if(m_currentDevice)
        {
            m_currentDevice->updateOutputModeNames();
        }
    }
    else qDebug() << "Translator not found. Using english";
}

void UiCore::loadDefaultTranslator()
{
    if (m_translator.load(QLocale(), QLatin1String("pangaea-mobile"), QLatin1String("_"), ":/translations/"))
    {
        qDebug() << "Default translator loaded. Locale: " << QLocale();
        QCoreApplication::installTranslator(&m_translator);

        emit sgTranslatorChanged(QLocale().nativeLanguageName());
    }
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
        "com.amtelectronics.utils/JavaFile", "openFileExternally",
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
    if(newDevice != nullptr)
        m_currentDevice = newDevice;
    else
        m_currentDevice = &dummyDevice;

    emit currentDeviceChanged();
}

#ifdef Q_OS_ANDROID
void UiCore::pickFirmwareFile()
{
    pickFile(ActivityType::PICK_FIRMWARE, "*/*");
}

void UiCore::slImpulseFilePicked(QString filePath, QString fileName)
{
    m_pickedIrPath = filePath;
    QFileInfo fileInfo(filePath);
    qDebug() << "Impulse picked" << fileName
             << "size:" << fileInfo.size() << "ir effective:" << m_currentDevice->maxIrSize();

    if(m_currentDevice->deviceType() > DeviceType::LEGACY_DEVICES)
    {
        if(fileInfo.size() > m_currentDevice->maxIrSize())
        {
            emit sgUiMessage(UiMessageType::PROPOSE_IR_TRIM, "File is bigger than processing IR", {fileName, m_pickedIrPath, m_dstIrPath});
            return;
        }
    }

    m_currentDevice->startIrUpload(filePath, m_dstIrPath, false);
}

void UiCore::pickFile(ActivityType fileType, QString filter)
{
    QJniObject ACTION_OPEN_DOCUMENT = QJniObject::getStaticObjectField<jstring>("android/content/Intent", "ACTION_OPEN_DOCUMENT");

    jint FLAG_READ_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_READ_URI_PERMISSION");
    jint FLAG_PERSISTABLE_PERMISSION = QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_PERSISTABLE_URI_PERMISSION");

    QJniObject intent("android/content/Intent");
    if (ACTION_OPEN_DOCUMENT.isValid() && intent.isValid())
    {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", ACTION_OPEN_DOCUMENT.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString(filter).object<jstring>());

        intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;", FLAG_READ_PERMISSION);
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
    qDebug() << "Run converter" << irConvertorProcess.startDetached(QCoreApplication::applicationDirPath() + "/IrConverter");
#endif

#ifdef Q_OS_LINUX
    QProcess irConvertorProcess;
    QString path = QCoreApplication::applicationDirPath() + "/IrConverter";
    qDebug() << "Run converter, path" << path << "result:" << irConvertorProcess.startDetached(path);
#endif
}
