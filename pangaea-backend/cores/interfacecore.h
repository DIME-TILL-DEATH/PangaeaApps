#ifndef INTERFACECORE_H
#define INTERFACECORE_H

#include <QObject>
#include <QSettings>

#include "devicedescription.h"
#include "abstractinterface.h"
#include "bleinterface.h"
#include "usbinterface.h"
#include "offlineinterface.h"

class InterfaceCore : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceCore(QObject *parent = nullptr);
    ~InterfaceCore();

    bool connectToDevice(DeviceDescription device);
    void disconnectFromDevice();
    void writeToDevice(QByteArray data, bool logCommand = true);
    void silentWriteToDevice(QByteArray data);

    void startScanning(DeviceConnectionType connectionType);
    void stopScanning();

    void setModuleName(QString name);
    void rssiMeasuring(bool isEnabled);

signals:
    void sgInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgDeviceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list);
    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription device);
    void sgInterfaceDisconnected(DeviceDescription device);
    void sgInterfaceError(QString errorDescription);
    void sgNewData(QByteArray data);
    void sgModuleNameUpdated(QString name);

    void sgCoreDisconnect();


    void sgRssiReaded(qint16 rssi);
private:
    AbstractInterface* m_exchangeInterface{nullptr};
    BleInterface* m_bleInterface{nullptr};
    UsbInterface* m_usbInterface{nullptr};
    OfflineInterface* m_offlineInterface{nullptr};

    QSettings* appSettings;

    DeviceDescription m_connectedDeviceDescription;

private slots:
//    void slNewData(QByteArray data);
    void slInterfaceError(QString errorDescription);
    void slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void slDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list);
};

#endif // INTERFACECORE_H
