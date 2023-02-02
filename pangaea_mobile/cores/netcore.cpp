#include <QMetaEnum>
#include <QDebug>

#include "netcore.h"

NetCore::NetCore(QObject *parent)
    : QObject{parent}
{
}

void NetCore::initialize()
{
    m_networkManager = new QNetworkAccessManager();
    m_networkManager->moveToThread(this->thread());

    jsonDataRequest.setUrl(QUrl("https://amtelectronics.com/new/pangaea-app-mob/actual_firmwares.json"));
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
        case DeviceType::CP16: m_deviceTypeString = "CP16"; break;
        case DeviceType::CP16PA: m_deviceTypeString = "CP16PA"; break;
        case DeviceType::CP100: m_deviceTypeString = "CP100"; break;
        case DeviceType::CP100PA: m_deviceTypeString = "CP100PA"; break;
        default: qDebug() << __FUNCTION__ << "Unknown device"; break;
    }

    deviceFirmware = actualFirmware;

    if(isAutocheckFirmwareEnabled)
        requestJsonData();
}

void NetCore::requestJsonData()
{
    m_networkManager->get(jsonDataRequest);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnJsonReqResult);
}

void NetCore::requestFirmwareFile()
{
    QNetworkReply* reply = m_networkManager->get(firmwareFileRequest);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnFileReqResult);
    connect(reply, &QNetworkReply::downloadProgress, this, &NetCore::sgDownloadProgress);
}

void NetCore::slOnJsonReqResult(QNetworkReply *reply)
{
    qDebug() << "Server answer for json request recieved";
    if(!reply->error())
    {
        parseJsonAnswer(reply);

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
    disconnect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnJsonReqResult);
}

void NetCore::slOnFileReqResult(QNetworkReply *reply)
{
    qDebug() << "Server answer for firmware file request recieved";

    newestFirmware->setRawData(reply->readAll());
    emit sgFirmwareDownloaded(newestFirmware->rawData());

    disconnect(m_networkManager, &QNetworkAccessManager::finished, this, &NetCore::slOnFileReqResult);
    disconnect(reply, &QNetworkReply::downloadProgress, this, &NetCore::sgDownloadProgress);
}

bool NetCore::parseJsonAnswer(QNetworkReply* reply)
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
