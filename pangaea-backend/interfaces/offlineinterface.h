#ifndef OFFLINEINTERFACE_H
#define OFFLINEINTERFACE_H

#include "abstractinterface.h"
#include "abstractmockdevice.h"

class OfflineInterface : public AbstractInterface
{
public:
    explicit OfflineInterface(QObject *parent = nullptr);

    void startScan() override;
    void stopScan() override;
    QList<DeviceDescription> discoveredDevicesList() override;
    bool connect(DeviceDescription device) override;
    void disconnectFromDevice() override;
    void write(const QByteArray& data) override;

private:
    AbstractMockDevice* m_mockDevice;
};

#endif // OFFLINEINTERFACE_H
