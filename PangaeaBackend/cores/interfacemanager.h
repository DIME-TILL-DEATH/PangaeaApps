#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include <QObject>
#include <QSettings>

#include "devicedescription.h"
#include "abstractinterface.h"
#include "bleinterface.h"
#ifndef Q_OS_IOS
#include "usbinterface.h"
#endif
#include "offlineinterface.h"

class InterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceManager(QObject *parent = nullptr);
    ~InterfaceManager();

    bool connectToDevice(DeviceDescription device);
    void disconnectFromDevice();
    void writeToDevice(QByteArray data);
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

    void sgErrorDisconnect();

    void sgRssiReaded(qint16 rssi);
private:
    bool m_logCommand{true};

    AbstractInterface* m_exchangeInterface{nullptr};
    BleInterface* m_bleInterface{nullptr};
#ifndef Q_OS_IOS
    UsbInterface* m_usbInterface{nullptr};
#else
    AbstractInterface* m_usbInterface{nullptr};
#endif
    OfflineInterface* m_offlineInterface{nullptr};

    QSettings* appSettings;

    DeviceDescription m_connectedDeviceDescription;

    void slNewDataArrived(QByteArray data);

private slots:
//    void slNewData(QByteArray data);
    void slInterfaceError(QString errorDescription);
    void slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void slDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list);

public slots:
    void setLogEnadled(bool logEnabled);
};

#endif // INTERFACEMANAGER_H
