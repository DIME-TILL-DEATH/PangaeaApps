#include "offlineinterface.h"

#include <QDebug>

#include "mockcp16legacy.h"
#include "mockcp16modern.h"

OfflineInterface::OfflineInterface(QObject *parent)
    : AbstractInterface{parent}
{
    m_mockThread = new QThread();

    m_mockThread->setObjectName("Mock thread");
    m_mockThread->start();
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
        m_mockDevice = new MockCP16Legacy(&mutex, &m_uartBuffer);
    }
    else if(device.name() == MockCP16Modern::mockName())
    {
        m_mockDevice = new MockCP16Modern(&mutex, &m_uartBuffer);
    }
    else
    {
        m_mockDevice = new AbstractMockDevice(&mutex, &m_uartBuffer);
    }
    m_mockDevice->moveToThread(m_mockThread);

    if(m_mockDevice)
    {
        QObject::connect(m_mockDevice, &AbstractMockDevice::answerReady, this, &AbstractInterface::sgNewData);
        QObject::connect(this, &OfflineInterface::sgUartBufferUpdated, m_mockDevice, &AbstractMockDevice::newDataRecieved);

        m_uartBuffer.clear();

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

    m_mockDevice->deleteLater();
    m_mockDevice = nullptr;

    emit sgInterfaceDisconnected(m_connectedDevice);
    setState(InterfaceState::Idle);
}

void OfflineInterface::write(const QByteArray &data)
{  
    if(m_mockDevice)
    {
        QMutexLocker locker(&mutex);
        m_uartBuffer.append(data);
        emit sgUartBufferUpdated();
    }
}
