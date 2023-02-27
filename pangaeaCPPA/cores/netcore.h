#ifndef NETCORE_H
#define NETCORE_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QJsonDocument>
#include <QJsonObject>

#include <QSettings>

#include "firmware.h"

class NetCore : public QObject
{
    Q_OBJECT
public:
    explicit NetCore(QObject *parent = nullptr);

//    void initialize();
    void requestAppUpdates();
    void requestNewestFirmware(Firmware* actualFirmware);
    void requestFirmwareFile();

//    void requestJsonData();

signals:
    void sgFirmwareDownloaded(const QByteArray& firmware);
    void sgNewFirmwareAvaliable(Firmware* newFirmware, Firmware* oldFirmware);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void slOnFirmwareVersionReqResult(QNetworkReply* reply);
    void slOnFileReqResult(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_networkManager;

    QNetworkRequest jsonDataRequest;
    QNetworkRequest firmwareFileRequest;

    bool isAutocheckFirmwareEnabled{true};

    QString m_deviceTypeString{"CP16"};

    Firmware* deviceFirmware{nullptr};
    Firmware* newestFirmware{nullptr};

    bool parseJsonAnswer(QNetworkReply *reply);
};

#endif // NETCORE_H
