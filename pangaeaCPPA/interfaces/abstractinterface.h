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
    ~AbstractInterface();

    virtual void startScan() = 0;
    virtual void stopScan() = 0;
    virtual QList<DeviceDescription> discoveredDevicesList() = 0;

    virtual bool connect(DeviceDescription device) = 0;
    virtual void disconnectFromDevice() = 0;

    virtual void write(QByteArray data) = 0;

    void setState(InterfaceState newState);
    void prevState();
    InterfaceState state() const;

    DeviceConnectionType connectionType() const;

signals:
    void sgInterfaceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgDeviceUnavaliable(DeviceConnectionType senderType, QString reason);
    void sgDeviceListUpdated(DeviceConnectionType senderType, QList<DeviceDescription>& list);
    void sgConnectionStarted();
    void sgInterfaceConnected(DeviceDescription& device);
    void sgInterfaceDisconnected(DeviceDescription& device);
    void sgInterfaceError(QString errorDescription);
    void sgNewData(QByteArray data);

    void sgChangedState(AbstractInterface::InterfaceState newState);

protected:
    QString m_description{"abstract"};
    DeviceConnectionType m_connectionType{DeviceConnectionType::Unknown};
    DeviceDescription m_connectedDevice;

private:
    InterfaceState m_state{InterfaceState::Idle};
    InterfaceState m_prevState{InterfaceState::Idle};

};

#endif // ABSTRACTINTERFACE_H
