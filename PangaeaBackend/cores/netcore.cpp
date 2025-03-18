#include <QMetaEnum>
#include <QDebug>
#include <QSysInfo>
#include <QCoreApplication>

#include <QVersionNumber>
#include <QStandardPaths>

#include "netcore.h"

NetCore::NetCore(QObject *parent)
    : QObject{parent}
{
    m_networkManager = new QNetworkAccessManager(this);

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#elif defined(Q_OS_IOS)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                           + "/settings.plist", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif
}

void NetCore::requestAppUpdates()
{
    if(appSettings->value("check_updates_enable", false).toBool())
    {
        jsonDataRequest.setUrl(QUrl("https://amtelectronics.com/new/pangaea-app-mob/actual_applications.json"));
        m_networkManager->get(jsonDataRequest);
        connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnApplicationVersionReqResult);
    }
}

void NetCore::requestNewestFirmware(Firmware *actualFirmware)
{
    if(actualFirmware == nullptr)
    {
        qWarning() << __FUNCTION__ << "Pointer to device firmware is null!";
        return;
    }

    switch(actualFirmware->deviceType())
    {
        case DeviceType::LEGACY_CP16: m_deviceTypeString = "CP16"; break;
        case DeviceType::LEGACY_CP16PA: m_deviceTypeString = "CP16PA"; break;
        case DeviceType::LEGACY_CP100: m_deviceTypeString = "CP100"; break;
        case DeviceType::LEGACY_CP100PA: m_deviceTypeString = "CP100PA"; break;
        default: qDebug() << __FUNCTION__ << "Unknown device"; break;
    }

    deviceFirmware = actualFirmware;

    if(appSettings->value("check_updates_enable", false).toBool())
    {
        qInfo() << "Checking updates...";
        jsonDataRequest.setUrl(QUrl("https://amtelectronics.com/new/pangaea-app-mob/actual_firmwares.json"));
        m_networkManager->get(jsonDataRequest);
        connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnFirmwareVersionReqResult);
    }
}

void NetCore::slOnFirmwareVersionReqResult(QNetworkReply *reply)
{
    qDebug() << "Server answer for firmware json request recieved";
    if(!reply->error())
    {
        parseFirmwareJsonAnswer(reply);

        qDebug() << "actual: " << deviceFirmware->firmwareVersion() << " avaliable: " << newestFirmware->firmwareVersion();

        if(*newestFirmware > *deviceFirmware)
        {
            qDebug() << "New firmware avaliable on server";
            emit sgNewFirmwareAvaliable(newestFirmware, deviceFirmware);
        }
    }
    else
    {
        QMetaEnum errorString = QMetaEnum::fromType<QNetworkReply::NetworkError>();
        qDebug() << "Server reply error" << errorString.valueToKey(reply->error());
    }
    reply->deleteLater();
    disconnect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnFirmwareVersionReqResult);
}

void NetCore::requestFirmwareFile()
{
    QNetworkReply* reply = m_networkManager->get(firmwareFileRequest);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnFileReqResult);
    connect(reply, &QNetworkReply::downloadProgress, this, &NetCore::sgDownloadProgress);
}

void NetCore::slOnFileReqResult(QNetworkReply *reply)
{
    qDebug() << "Server answer for firmware file request recieved";

    newestFirmware->setRawData(reply->readAll());
    emit sgFirmwareDownloaded(newestFirmware->rawData());

    disconnect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnFileReqResult);
    disconnect(reply, &QNetworkReply::downloadProgress, this, &NetCore::sgDownloadProgress);
}

void NetCore::slOnApplicationVersionReqResult(QNetworkReply *reply)
{
    qDebug() << "Server answer for applications json request recieved";
    if(!reply->error())
    {
        parseApplicationJsonAnswer(reply);
    }
    else
    {
        QMetaEnum errorString = QMetaEnum::fromType<QNetworkReply::NetworkError>();
        qDebug() << "Server reply error" << errorString.valueToKey(reply->error());
    }
    reply->deleteLater();
    disconnect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnApplicationVersionReqResult);
}

bool NetCore::parseFirmwareJsonAnswer(QNetworkReply* reply)
{ 
    QByteArray baReply = reply->readAll();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(baReply);
    QJsonObject jsonRoot = jsonDocument.object();

    QString newestFirmwareVersionString;

    if(jsonRoot.contains(m_deviceTypeString) && jsonRoot[m_deviceTypeString].isObject())
    {
        QJsonObject jsonDeviceObject = jsonRoot[m_deviceTypeString].toObject();

        if(jsonDeviceObject.contains("version") && jsonDeviceObject["version"].isString())
        {
            newestFirmwareVersionString = jsonDeviceObject["version"].toString();
        }
        else return false;

        if(jsonDeviceObject.contains("path") && jsonDeviceObject["path"].isString())
        {
            QUrl firmwareUrl = jsonDeviceObject["path"].toString();
            firmwareFileRequest.setUrl(firmwareUrl);
        }
        else return false;
    }
    else return false;

    if(newestFirmware != nullptr)
        delete newestFirmware;

    newestFirmware = new Firmware(newestFirmwareVersionString, deviceFirmware->deviceType(), FirmwareType::NetworkUpdate, "net:/rawByteArray");

    return true;
}

bool NetCore::parseApplicationJsonAnswer(QNetworkReply *reply)
{
    QByteArray baReply = reply->readAll();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(baReply);
    QJsonObject jsonRoot = jsonDocument.object();

    QString osName = QSysInfo::productType();
    qDebug() << "OS name: " << osName;

    if(jsonRoot.contains(osName) && jsonRoot[osName].isObject())
    {
        QJsonObject jsonDeviceObject = jsonRoot[osName].toObject();

        if(jsonDeviceObject.contains("version") && jsonDeviceObject["version"].isString())
        {
            QString newestApplicationVersionString = jsonDeviceObject["version"].toString();

            QVersionNumber newestVersion = QVersionNumber::fromString(newestApplicationVersionString);
            QVersionNumber currentVersion = QVersionNumber::fromString(QCoreApplication::applicationVersion());

            qDebug() << "App versions, current: " << currentVersion << " newest: " << newestVersion;

            if(newestVersion > currentVersion) emit sgNewAppVersionAvaliable(newestApplicationVersionString);
        }
        else return false;
    }
    else return false;

    return true;
}
