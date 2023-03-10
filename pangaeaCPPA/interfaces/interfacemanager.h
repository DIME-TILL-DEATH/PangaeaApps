#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include <QObject>

#include "devicedescription.h"
#include "abstractinterface.h"
#include "bleinterface.h"
#include "usbinterface.h"

class InterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceManager(QObject *parent = nullptr);
    ~InterfaceManager();

    bool connectToDevice(DeviceDescription device);
    void disconnectFromDevice();
    void writeToDevice(QByteArray data);

    void startScanning();
    void stopScanning();

signals:
    void sgInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgDeviceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list);
    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription device);
    void sgInterfaceDisconnected(DeviceDescription device);
    void sgInterfaceError(QString errorDescription);
    void sgNewData(QByteArray data);

private:
    AbstractInterface* m_exchangeInterface{nullptr};
    BleInterface* m_bleInterface{nullptr};
    UsbInterface* m_usbInterface{nullptr};

private slots:
    void slInterfaceError(QString errorDescription);
    void slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
};

#endif // INTERFACEMANAGER_H
