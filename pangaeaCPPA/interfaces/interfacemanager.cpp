#include <QDebug>
#include <QThread>

#include "interfacemanager.h"

InterfaceManager::InterfaceManager()
{

}

InterfaceManager::~InterfaceManager()
{
    disconnectFromDevice();

    m_usbInterface.stopScan();
    m_bleInterface.stopScan();
}

bool InterfaceManager::connectToDevice(DeviceDescription device)
{
    stopScanning();

    if(m_exchangeInterface) disconnectFromDevice();

    switch(device.connectionType())
    {
        case DeviceConnectionType::BLE:
        {
            qDebug() << "Settling bluetooth interface";
            m_exchangeInterface = &m_bleInterface;
            break;
        }
        case DeviceConnectionType::USBAuto:
        {
            qDebug() << "Settling USB interface";
            m_exchangeInterface = &m_usbInterface;
            break;
        }
        default:
        {
            qDebug() << "Inteface type not implemented";
            return false;
        }
    }
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgNewData, this, &InterfaceManager::sgNewData);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgConnectionStarted, this, &InterfaceManager::sgConnectionStarted);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceError, this, &InterfaceManager::slInterfaceError);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceConnected, this, &InterfaceManager::sgInterfaceConnected);

    return m_exchangeInterface->connect(device);
}

void InterfaceManager::disconnectFromDevice()
{
    m_exchangeInterface->disconnectFromDevice();

    QObject::disconnect(m_exchangeInterface, nullptr, this, nullptr);
}

void InterfaceManager::writeToDevice(QByteArray data)
{
    m_exchangeInterface->write(data);
}

void InterfaceManager::startScanning()
{
    QObject::connect(&m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::sgDeviceListUpdated);
    QObject::connect(&m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::sgDeviceListUpdated);

    m_usbInterface.startScan();
    m_bleInterface.startScan();
}

void InterfaceManager::stopScanning()
{
    m_usbInterface.stopScan();
    m_bleInterface.stopScan();
}

void InterfaceManager::slInterfaceError(QString errorDescription)
{
    disconnectFromDevice();
    emit sgInterfaceError(errorDescription);
}
