#ifndef BLEINTERFACE_H
#define BLEINTERFACE_H

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>

#include <QHash>
#include <QSettings>
#include <qpermissions.h>
#include <qregularexpression.h>

#include <QTimer>

#include "devicedescription.h"
#include "abstractinterface.h"

#define UARTSERVICEUUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define RXUUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define TXUUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

class  BleInterface : public AbstractInterface
{
    Q_OBJECT
public:
    explicit BleInterface(QObject *parent = nullptr);
    ~BleInterface();

    void startScan();
    void stopScan();
    QList<DeviceDescription> discoveredDevicesList();

    bool connect(DeviceDescription device);
    void disconnectFromDevice();

    void write(const QByteArray &data);

    void setModuleName(QString name);
    const QString &moduleName() const;

    void rssiMeasuring(bool isEnabled);
signals:
    void rssiReaded(qint16 rssi);


private slots:
    void startDiscovering();
    void updateBLEDevicesList();
    const QBluetoothDeviceInfo* getDeviceByAddress(const QString& address);

    /* Slots for QBluetothDeviceDiscoveryAgent */
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);
    void scanTimeout();

    /* Slots for QLowEnergyController */
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error);
    void deviceConnected();
    void deviceDisconnected();
    void requestRssi();
    void connectionParametersUpdated(const QLowEnergyConnectionParameters &newParameters);

    /* Slotes for QLowEnergyService */
    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateData(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);

    /* Slots for user */
    void slStartConnect(QString address);
    void checkDevicesAvaliabliy();

    void serviceError(QLowEnergyService::ServiceError newError);

private:
    bool isAvaliable;

    int mtu;

    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent{nullptr};

    QList<QBluetoothDeviceInfo> m_avaliableDevices;
    QList<DeviceDescription> m_qlFoundDevices;

    QLowEnergyController *m_control;
    QLowEnergyService *m_service;
    QLowEnergyDescriptor m_notificationDescTx;
    QLowEnergyService *m_UARTService;
    bool m_bFoundUARTService;

    QSettings* appSettings;

    QString m_currentDeviceAddress;
    QString m_moduleName;

    QHash<QString, QString> m_moduleUniqueNames;

    QTimer* rssiUpdateTimer;
};

#endif // BLEINTERFACE_H
