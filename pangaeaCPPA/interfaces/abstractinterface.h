#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H

#include <QObject>

#include "devicedescription.h"

class AbstractInterface : public QObject
{
    Q_OBJECT
public:
    enum InterfaceState {
        Idle = 0,
        Scanning,
        UpdateModuleName,
        Connecting,
        Connected,
        ServiceFound,
        AcquireData
    };
    Q_ENUM(InterfaceState)

    explicit AbstractInterface(QObject *parent = nullptr);

    virtual void discoverDevices() = 0;
    virtual QList<DeviceDescription> discoveredDevicesList() = 0;

    virtual bool connect(DeviceDescription device) = 0;
    virtual bool isConnected() = 0;

    virtual void write(QByteArray data) = 0;

    virtual void disconnect() = 0;

    virtual QString connectionDescription() = 0;

    void setState(InterfaceState newState);
    void prevState();
    InterfaceState state() const;

signals:
    void sgDeviceListUpdated();
    void sgInterfaceConnected();
    void sgInterfaceError(QString errorDescription);
    void sgChangedState(AbstractInterface::InterfaceState newState);
    void sgNewData(QByteArray data);

private:
    InterfaceState m_state{InterfaceState::Idle};
    InterfaceState m_prevState{InterfaceState::Idle};
};

#endif // ABSTRACTINTERFACE_H
