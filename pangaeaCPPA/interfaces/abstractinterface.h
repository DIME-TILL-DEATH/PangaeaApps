#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H

#include <QObject>

#include "devicedescription.h"

class AbstractInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractInterface(QObject *parent = nullptr);

    virtual void discoverDevices() = 0;
    virtual QList<DeviceDescription> discoveredDevicesList() = 0;

    virtual bool connect(DeviceDescription device) = 0;
    virtual void checkConnection() = 0;
    virtual bool isConnected() = 0;

    virtual void write(QByteArray data) = 0;

    virtual void disconnect() = 0;

    virtual void flushBuffers() = 0;
    virtual QString connectionDescription() = 0;

signals:
    void sgNewData(QByteArray data);
    void sgInterfaceError(QString errorDescription);


};

#endif // ABSTRACTINTERFACE_H
