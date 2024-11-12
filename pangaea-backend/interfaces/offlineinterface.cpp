#include "offlineinterface.h"

#include <QDebug>

#include "mockcp16legacy.h"

OfflineInterface::OfflineInterface(QObject *parent)
    : AbstractInterface{parent}
{

}

void OfflineInterface::startScan()
{
}

void OfflineInterface::stopScan()
{
}

QList<DeviceDescription> OfflineInterface::discoveredDevicesList()
{
    return {DeviceDescription("Offline CP16/CP100 legacy", "virtual", DeviceConnectionType::Offline)};
}

bool OfflineInterface::connect(DeviceDescription device)
{
    m_mockDevice = new MockCP16Legacy;
    if(m_mockDevice)
    {
        QObject::connect(m_mockDevice, &AbstractMockDevice::answerReady, this, &AbstractInterface::sgNewData);
        qDebug() << "Virtual device connected" << m_mockDevice;
        setState(InterfaceState::Connected);
        emit sgInterfaceConnected(device);
        return true;
    }
    else return false;
}

void OfflineInterface::disconnectFromDevice()
{
    disconnect(m_mockDevice);
    delete(m_mockDevice);
    m_mockDevice = nullptr;

    emit sgInterfaceDisconnected(m_connectedDevice);
    setState(InterfaceState::Idle);
}

void OfflineInterface::write(const QByteArray &data)
{
    if(m_mockDevice) m_mockDevice->writeToDevice(data);
}
