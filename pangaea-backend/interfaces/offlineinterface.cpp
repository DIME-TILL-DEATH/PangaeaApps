#include "offlineinterface.h"

#include <QDebug>

#include "mockcp16legacy.h"
#include "mockcp16modern.h"

OfflineInterface::OfflineInterface(QObject *parent)
    : AbstractInterface{parent}
{

}

void OfflineInterface::startScan()
{
    m_discoveredDevicesList.append({DeviceDescription(MockCP16Legacy::mockName(), "virtual", DeviceConnectionType::Offline)});
    m_discoveredDevicesList.append({DeviceDescription(MockCP16Modern::mockName(), "virtual", DeviceConnectionType::Offline)});
    emit sgDeviceListUpdated(DeviceConnectionType::Offline, m_discoveredDevicesList);
}

void OfflineInterface::stopScan()
{
}

QList<DeviceDescription> OfflineInterface::discoveredDevicesList()
{
    return m_discoveredDevicesList;
}

bool OfflineInterface::connect(DeviceDescription device)
{
    if(device.name() == MockCP16Legacy::mockName())
    {
        m_mockDevice = new MockCP16Legacy(this);
    }
    else if(device.name() == MockCP16Modern::mockName())
    {
        m_mockDevice = new MockCP16Modern(this);
    }
    else
    {
        m_mockDevice = new AbstractMockDevice(this);
    }

    if(m_mockDevice)
    {
        QObject::connect(m_mockDevice, &AbstractMockDevice::answerReady, this, &AbstractInterface::sgNewData);
        qInfo() << "Virtual device connected" << m_mockDevice->mockDeviceType();
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
