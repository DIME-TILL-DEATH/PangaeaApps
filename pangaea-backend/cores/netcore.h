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

    void requestAppUpdates();
    void requestNewestFirmware(Firmware* actualFirmware);
    void requestFirmwareFile();

signals:
    void sgFirmwareDownloaded(const QByteArray& firmware);
    void sgNewFirmwareAvaliable(Firmware* newFirmware, Firmware* oldFirmware);
    void sgNewAppVersionAvaliable(QString newAppVersion);
    void sgDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void slOnFirmwareVersionReqResult(QNetworkReply* reply);
    void slOnFileReqResult(QNetworkReply* reply);
    void slOnApplicationVersionReqResult(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_networkManager;

    QNetworkRequest jsonDataRequest;
    QNetworkRequest firmwareFileRequest;

    QString m_deviceTypeString{"CP16"};

    Firmware* deviceFirmware{nullptr};
    Firmware* newestFirmware{nullptr};

    bool parseFirmwareJsonAnswer(QNetworkReply *reply);
    bool parseApplicationJsonAnswer(QNetworkReply *reply);
};

#endif // NETCORE_H
