#ifndef OFFLINEINTERFACE_H
#define OFFLINEINTERFACE_H

#include <QThread>
#include <QMutex>

#include "abstractinterface.h"
#include "abstractmockdevice.h"

class OfflineInterface : public AbstractInterface
{
    Q_OBJECT
public:
    explicit OfflineInterface(QObject *parent = nullptr);

    void startScan() override;
    void stopScan() override;
    QList<DeviceDescription> discoveredDevicesList() override;
    bool connect(DeviceDescription device) override;
    void disconnectFromDevice() override;
    void write(const QByteArray& data) override;

signals:
    void sgUartBufferUpdated();

private:
    AbstractMockDevice* m_mockDevice;

    QThread* m_mockThread;
    mutable QMutex mutex;

    QByteArray m_uartBuffer;

    QList<DeviceDescription> m_discoveredDevicesList;
};

#endif // OFFLINEINTERFACE_H
