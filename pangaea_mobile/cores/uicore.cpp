#include <QDebug>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QCoreApplication>

#include "uicore.h"
//#include "resampler.h"

#ifdef __ANDROID__
#include <jni.h>
#include "activityresultmanager.h"

ActivityResultManager activityResultHandler;
#endif

UICore::UICore(QObject *parent)
    : QObject{parent}
{
#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);

    connect(&activityResultHandler, &ActivityResultManager::sgIrFilePicked, this, &UICore::slImpulseFilePicked);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFilePicked, this, &UICore::sgImportPreset);
    connect(&activityResultHandler, &ActivityResultManager::sgFirmwareFilePicked, this, &UICore::slFirmwareFilePicked);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFileCreated, this, &UICore::sgExportPreset);
#else
    appSettings = new QSettings(QSettings::UserScope);
#endif

    loadDefaultTranslator();
}

void UICore::setupApplication()
{
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    emit sgSetUIText("application_language", appLanguage);

    QString colorTheme = appSettings->value("color_theme", "dark_orange").toString();
    emit sgSetUIText("color_theme", colorTheme);

    bool isAutoconnectEnabled = appSettings->value("autoconnect_enable").toBool();
    emit sgSetUIParameter("autoconnect_enable", isAutoconnectEnabled);

    bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable", true).toBool();
    emit sgSetUIParameter("check_updates_enable", isCheckUpdatesEnabled);

    bool firstRun = !appSettings->value("first_run", true).toBool();
    emit sgSetUIParameter("first_run", firstRun);
    appSettings->setValue("first_run", false);
}


void UICore::setParameter(QString name, quint8 val)
{
    emit sgSetParameter(name, val);
}

void UICore::restoreParameter(QString name)
{
    emit sgRestoreValue(name);
}

void UICore::readAll()
{
    emit sgReadAllParameters();
}

void UICore::setImpuls(QString fullFilePath)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(fullFilePath)  
    pickFile(ActivityType::PICK_IR, "audio/*");
#else
    QFileInfo fileInfo(fullFilePath);
    QString impulseName = fileInfo.fileName();
    emit sgSetImpuls(fullFilePath, impulseName);
#endif
}

void UICore::convertAndUploadImpulse(QString filePath)
{
    Q_UNUSED(filePath);
    QFileInfo irFileInfo(m_pickedImpulsePath);

    QString irOutFileName = irFileInfo.fileName();
    QString outFolder = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/convertedIR/";
    QString tmpFilePath = outFolder+"tmp.wav";

    if(!QDir(outFolder).exists())
    {
        QDir().mkpath(outFolder);
    }
    QString outpuFilePath = outFolder + irOutFileName;

    QFile tmpFile;
    tmpFile.setFileName(m_pickedImpulsePath);
    tmpFile.copy(tmpFilePath);

//    Resampler().convertFile(tmpFilePath, outpuFilePath);

    tmpFile.remove(tmpFilePath);

    emit sgSetImpuls(outpuFilePath, irOutFileName);
}

void UICore::pickFile(ActivityType fileType, QString filter)
{
#ifdef Q_OS_ANDROID
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
#endif
}

void UICore::slProposeNetFirmwareUpdate(Firmware* updateFirmware, Firmware* oldFirmware)
{
    emit sgSetUIText("firmware_local_path", updateFirmware->path());
    emit sgSetUIText("new_firmware_avaliable", oldFirmware->firmwareVersion() + ',' + updateFirmware->firmwareVersion());
}

void UICore::slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware)
{
    emit sgSetUIText("firmware_version_error",
                     actualFirmware->firmwareVersion()+","+minimalFirmware->firmwareVersion());
    emit sgSetUIText("firmware_local_path", minimalFirmware->path());
}

void UICore::slImpulseFilePicked(QString filePath, QString fileName)
{
    emit sgSetImpuls(filePath, fileName);
    qDebug() << "Impulse picked";
    m_pickedImpulsePath = filePath;
}

void UICore::pickFirmwareFile()
{
    pickFile(ActivityType::PICK_FIRMWARE, "*/*");
}

void UICore::slFirmwareFilePicked(QString filePath, QString fileName)
{
    emit sgSetUIText("firmware_file_picked", filePath + ',' + fileName);
}

void UICore::setFirmware(QString fullFilePath)
{
    // TODO firmwareupdatemode
//    emit sgSetUIParameter("update_firmware_mode", FirmwareUpdateMode::Offline);
    emit sgSetFirmware(fullFilePath);
}

void UICore::doOnlineFirmwareUpdate()
{
//    emit sgSetUIParameter("update_firmware_mode", FirmwareUpdateMode::Online);
    emit sgDoOnlineFirmwareUpdate();
}

void UICore::escImpuls()
{
    emit sgEscImpuls();
}

void UICore::exportPreset(QString fileName)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(fileName)

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
#endif
}

void UICore::importPreset(QString filePath)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(filePath)

    pickFile(ActivityType::PICK_PRESET, "*/*");
#else
    emit sgImportPreset(filePath, "");
#endif
}

void UICore::setLanguage(QString languageCode)
{
    appSettings->setValue("application_language", languageCode);
    appSettings->sync();

    loadTranslator(languageCode);
}

void UICore::loadTranslator(QString languageCode)
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
    }
    else qDebug() << "Translator not found. Using english";
}

void UICore::loadDefaultTranslator()
{
    if (m_translator.load(QLocale(), QLatin1String("pangaea-mobile"),
                        QLatin1String("_"), ":/translations/"))
    {
        qDebug() << "Default ranslator loaded. Locale: " << QLocale();
        QCoreApplication::installTranslator(&m_translator);

        emit sgTranslatorChanged(QLocale().nativeLanguageName());
    }
}

void UICore::saveSetting(QString settingName, QVariant settingValue)
{
    appSettings->setValue(settingName, settingValue);
    appSettings->sync();

    qDebug() << __FUNCTION__ << "Setting name: " << settingName << "Setting value:" << settingValue;
}

void UICore::openManualExternally(QString fileName)
{
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    if(appLanguage=="autoselect")
    {
        appLanguage = QLocale().name().left(2);
    }

    QString fullFileName =  fileName + "_" + appLanguage + ".pdf";
    QString filePath = ":/docs/" + fullFileName;
    QFile pdfFile(filePath);

    if(!pdfFile.exists())
    {
        fullFileName = fileName + ".pdf";
        filePath = ":/docs/" + fullFileName;
        pdfFile.setFileName(filePath);
    }

    #ifdef Q_OS_ANDROID
        QString temporallyPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)+"/" + fullFileName;
        pdfFile.copy(temporallyPath);

        qDebug() << "Final manual path: " << temporallyPath;

        QJniObject::callStaticMethod<void>(
                "com.amtelectronics.utils/JavaFile", "openFileExternally",
                "(Ljava/lang/String;Landroid/content/Context;)V",
                QJniObject::fromString(fullFileName).object<jstring>(),
                QNativeInterface::QAndroidApplication::context());
    #else
        QDesktopServices::openUrl(filePath);
    #endif
}

const QString &UICore::moduleName() const
{
    return m_moduleName;
}

void UICore::setModuleName(const QString &newModuleName)
{
    if (m_moduleName == newModuleName)
        return;
    m_moduleName = newModuleName;
    emit sgModuleNameChanged(newModuleName);
}

void UICore::sw4Enable()
{
    emit sgSw4Enable();
}
