#ifndef UIINTERFACEMANAGER_H
#define UIINTERFACEMANAGER_H

#include <QObject>

#include "deviceslistmodel.h"
#include "devicedescription.h"

class UiInterfaceManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isBleAvaliable READ isBleAvaliable NOTIFY isBleAvaliableChanged)
public:
    explicit UiInterfaceManager(QObject *parent = nullptr);
    ~UiInterfaceManager();

    Q_INVOKABLE void startScanning();
    Q_INVOKABLE void connectToDevice(DeviceDescription device);
    Q_INVOKABLE void disconnectFromDevice();

    void updateDevicesList(DeviceConnectionType connectionType, QList<DeviceDescription> list);

    DevicesListModel devicesListModel() const;

    bool isBleAvaliable();

public slots:
    void slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);

signals:
    void sgStartScanning();

    void sgInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgConnectToDevice(DeviceDescription device);
    void sgDisconnectFromDevice();
    void sgDeviceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgExchangeError();

    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription device);
    void sgInterfaceDisconnected(DeviceDescription device);
    void sgInterfaceError(QString errorDescription);

    void isBleAvaliableChanged();

private:
    bool m_isBleAvaliable{true};

    DevicesListModel m_devicesListModel;
};

#endif // UIINTERFACEMANAGER_H
