#ifndef UIINTERFACEMANAGER_H
#define UIINTERFACEMANAGER_H

#include <QObject>

#include "interfacelistmodel.h"
#include "devicedescription.h"

class UiInterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit UiInterfaceManager(QObject *parent = nullptr);
    ~UiInterfaceManager();

    Q_INVOKABLE void startScanning();
    Q_INVOKABLE void connectToDevice(DeviceDescription device);

    void updateDevicesList(DeviceConnectionType connectionType, QList<DeviceDescription> list);

signals:
    void sgStartScanning();
    void sgConnectToDevice(DeviceDescription device);

    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription device);
    void sgInterfaceDisconnected();
    void sgInterfaceError(QString errorDescription);

private:
    InterfaceListModel m_interfaceListModel;
};

#endif // UIINTERFACEMANAGER_H
