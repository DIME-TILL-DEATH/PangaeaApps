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
    InterfaceManager();
    ~InterfaceManager();

    bool connectToDevice(DeviceDescription device);
    void disconnectFromDevice();
    void writeToDevice(QByteArray data);

    void startScanning();
    void stopScanning();

signals:
    void sgDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list);
    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription device);
    void sgInterfaceError(QString errorDescription);
    void sgNewData(QByteArray data);

private:
    AbstractInterface* m_exchangeInterface{nullptr};
    BleInterface m_bleInterface{this};
    UsbInterface m_usbInterface{this};

private slots:
    void slInterfaceError(QString errorDescription);
};

#endif // INTERFACEMANAGER_H
