#include <QDebug>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QCoreApplication>

#include "uicore.h"

#ifdef __ANDROID__
#include <jni.h>
#include "activityresultmanager.h"

ActivityResultManager activityResultHandler;
#endif

UICore::UICore(BluetoothleUART* bleConnection, QQmlApplicationEngine *engine, QObject *parent)
    : QObject{parent},
      m_qmlEngine{engine},
      m_bleConnection{bleConnection}
{
#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);

    connect(&activityResultHandler, &ActivityResultManager::sgIrFilePicked, this, &UICore::sgSetImpuls, Qt::QueuedConnection);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFilePicked, this, &UICore::sgImportPreset, Qt::QueuedConnection);
    connect(&activityResultHandler, &ActivityResultManager::sgFirmwareFilePicked, this, &UICore::slFirmwareFilePicked, Qt::QueuedConnection);
    connect(&activityResultHandler, &ActivityResultManager::sgPresetFileCreated, this, &UICore::sgExportPreset, Qt::QueuedConnection);
#else
    appSettings = new QSettings(QSettings::UserScope);
#endif

    loadDefaultTranslator();

    connect(m_bleConnection, &BluetoothleUART::sgLocalBluetoothNotReady, this, &UICore::sgLocalBluetoothNotReady);
    connect(m_bleConnection, &BluetoothleUART::sgConnect, this, &UICore::connectToDevice);

    connect(this, &UICore::sgStartScan, m_bleConnection, &BluetoothleUART::startScan, Qt::QueuedConnection);
    connect(this, &UICore::sgDoConnect, m_bleConnection, &BluetoothleUART::doConnect, Qt::QueuedConnection);
    connect(this, &UICore::sgDoDisconnect, m_bleConnection, &BluetoothleUART::doDisconnect, Qt::QueuedConnection);
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

void UICore::connectToDevice(quint8 devNum)
{
    emit sgConnectToDevice(devNum);
}

void UICore::disconnectFromDevice()
{
    emit sgDoDisconnect();
    emit sgSetUIText("devVersion" ,"");
    emit sgSetUIParameter("wait", false);
}

void UICore::doConnect(quint8 numDev, QString address)
{
    emit sgDoConnect(numDev, address);
}

void UICore::rescanDevices()
{
    emit sgStartScan();
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
    emit sgReadAll();
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

void UICore::pickFirmwareFile()
{
    // TODO ???? ?????????? ???????????????? ???? ??????-???? ?????????? ????????????????. ???????? ???????????? UICore ?????? desktop ?? mobile, ???????? ???????????? ??????????????
    pickFile(ActivityType::PICK_FIRMWARE, "*/*");
}

void UICore::slFirmwareFilePicked(QString filePath, QString fileName)
{
    emit sgSetUIText("firmware_file_picked", filePath + ',' + fileName);
}

void UICore::setFirmware(QString fullFilePath)
{
    emit sgSetUIParameter("update_firmware_mode", FirmwareUpdateMode::Offline);
    emit sgSetFirmware(fullFilePath);
}

void UICore::doOnlineFirmwareUpdate()
{
    emit sgSetUIParameter("update_firmware_mode", FirmwareUpdateMode::Online);
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

        // TODO qQmlEngine ?????????? ???????????????????????? ?? ???????????????? sgTranslatorChanged ????????????. ?????? ?????????????? retranslate.
        // ?????????????????? ?????????? ???? languegeCode. ???????????????? ?? QML?

        m_qmlEngine->retranslate();

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
        m_qmlEngine->retranslate();
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
