#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QProcess>
#include <QCoreApplication>
#include <QUrl>

#include "uidesktopcore.h"

UiDesktopCore::UiDesktopCore(QObject *parent)
    : QObject{parent}
{
    appSettings = new QSettings(QSettings::UserScope);
}

void UiDesktopCore::setupApplication()
{
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    emit sgSetUIText("application_language", appLanguage);

//    QString colorTheme = appSettings->value("color_theme", "dark_orange").toString();
//    emit sgSetUIText("color_theme", colorTheme);

    bool isModulesRightAligned = appSettings->value("modules_right_aligned", true).toBool();
    emit sgSetUIParameter("modules_right_aligned", isModulesRightAligned);

    quint16 windowWidth = appSettings->value("window_width").toUInt();
    if(windowWidth>0) emit sgSetUIParameter("window_width", windowWidth);

    quint16 windowHeight = appSettings->value("window_height").toUInt();
    if(windowHeight) emit sgSetUIParameter("window_height", windowHeight);

    bool isAutoconnectEnabled = appSettings->value("autoconnect_enable").toBool();
    emit sgSetUIParameter("autoconnect_enable", isAutoconnectEnabled);

    bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable", true).toBool();
    emit sgSetUIParameter("check_updates_enable", isCheckUpdatesEnabled);

    bool firstRun = !appSettings->value("first_run", true).toBool();
    emit sgSetUIParameter("first_run", firstRun);
    appSettings->setValue("first_run", false);
}

void UiDesktopCore::setParameter(QString name, quint8 val)
{
    emit sgSetParameter(name, val);
}

void UiDesktopCore::restoreParameter(QString name)
{
    emit sgRestoreValue(name);
}

void UiDesktopCore::setImpuls(QString fullFilePath)
{
    QFileInfo fileInfo(fullFilePath);
    QString impulseName = fileInfo.fileName();
    emit sgSetImpuls(fullFilePath, impulseName);
}

void UiDesktopCore::escImpuls()
{
    emit sgEscImpuls();
}

void UiDesktopCore::convertAndUploadImpulse(QString fullFilePath)
{
    QFileInfo irFileInfo(fullFilePath);
    QString outFolder = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)+
            "/AMT/pangaeaCPPA/convertedIR/";

    if(!QDir(outFolder).exists())
    {
        QDir().mkpath(outFolder);
    }
    QString outpuFilePath = outFolder+irFileInfo.fileName();

    Resampler().convertFile(fullFilePath, outpuFilePath);

    setImpuls(outpuFilePath);
}

void UiDesktopCore::exportPreset(QString filePath)
{
    QFileInfo fileInfo(filePath);

    emit sgExportPreset(filePath, fileInfo.fileName());
}

void UiDesktopCore::importPreset(QString filePath)
{
    QFileInfo fileInfo(filePath);

    emit sgImportPreset(filePath, fileInfo.fileName());
}

void UiDesktopCore::setLanguage(QString languageCode)
{
    appSettings->setValue("application_language", languageCode);
    appSettings->sync();

    loadTranslator(languageCode);
}

void UiDesktopCore::saveSetting(QString settingName, QVariant settingValue)
{
    appSettings->setValue(settingName, settingValue);
    appSettings->sync();

    qDebug() << __FUNCTION__ << "Setting name: " << settingName << "Setting value:" << settingValue;
}

void UiDesktopCore::sw4Enable()
{

}

void UiDesktopCore::openManualExternally(QString fileName)
{
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    if(appLanguage=="autoselect")
    {
        appLanguage = QLocale().name().left(2);
    }

    QString fullFileName =  fileName + "_" + appLanguage + ".pdf";

    QString filePath =  QCoreApplication::applicationDirPath() + "/docs/" + fullFileName;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void UiDesktopCore::runIrConvertor()
{
    QProcess irConvertorProcess;

#ifdef Q_OS_WIN
    irConvertorProcess.setWorkingDirectory(QCoreApplication::applicationDirPath());
    irConvertorProcess.setProgram("IrConverter.exe");
    irConvertorProcess.startDetached();
#endif

#ifdef Q_OS_MACOS
    QDir appBundlePath(QCoreApplication::applicationDirPath());

    irConvertorProcess.setWorkingDirectory(appBundlePath.absolutePath());
    irConvertorProcess.setProgram("IrConverter");
    irConvertorProcess.startDetached();
#endif
}

void UiDesktopCore::loadTranslator(QString languageCode)
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
        //m_qmlEngine->retranslate();

        emit sgTranslatorChanged(languageCode);
    }
    else qDebug() << "Translator not found. Using english";
}

void UiDesktopCore::loadDefaultTranslator()
{
    if (m_translator.load(QLocale(), QLatin1String("pangaea-mobile"),
                        QLatin1String("_"), ":/translations/"))
    {
        qDebug() << "Default ranslator loaded. Locale: " << QLocale();
        QCoreApplication::installTranslator(&m_translator);
        emit sgTranslatorChanged(QLocale().nativeLanguageName());
    }
}

void UiDesktopCore::slProposeNetFirmwareUpdate(Firmware *updateFirmware, Firmware *oldFirmware)
{
    emit sgSetUIText("firmware_local_path", updateFirmware->path());
    emit sgSetUIText("new_firmware_avaliable", oldFirmware->firmwareVersion() + ',' + updateFirmware->firmwareVersion());
}

void UiDesktopCore::slProposeOfflineFirmwareUpdate(Firmware *minimalFirmware, Firmware *actualFirmware)
{
    emit sgSetUIText("firmware_version_error",
                     actualFirmware->firmwareVersion()+","+minimalFirmware->firmwareVersion());
    emit sgSetUIText("firmware_local_path", minimalFirmware->path());
}

const QString &UiDesktopCore::moduleName() const
{
    return m_moduleName;
}

void UiDesktopCore::setModuleName(const QString &newModuleName)
{
    if (m_moduleName == newModuleName)
        return;
    m_moduleName = newModuleName;
    emit sgModuleNameChanged(newModuleName);
}
