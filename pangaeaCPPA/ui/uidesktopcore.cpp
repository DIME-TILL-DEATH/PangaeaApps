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
#include "uidesktopcore.h"

UiDesktopCore::UiDesktopCore(QObject *parent)
    : QObject{parent}
{

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

void UiDesktopCore::readAllParameters()
{
    emit sgReadAllParameters();
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

void UiDesktopCore::openManualExternally(QString fileName)
{
    QString appLanguage = QSettings(QSettings::UserScope).value("application_language", "autoselect").toString();

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
    qDebug() << "Run converter" << irConvertorProcess.startDetached(QCoreApplication::applicationDirPath() + "/IrConverter");
#endif
}

void UiDesktopCore::updateFirmware(QString filePath)
{
    emit sgSetFirmware(filePath);
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

void UiDesktopCore::slNewAppVersionAvaliable(QString appVersion)
{
    emit sgSetUIText("new_app_version_avaliable", appVersion);
}
