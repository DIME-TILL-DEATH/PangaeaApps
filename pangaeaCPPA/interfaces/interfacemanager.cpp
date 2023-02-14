#include <QDebug>
#include <QThread>

#include "interfacemanager.h"

InterfaceManager::InterfaceManager(QObject *parent)
                : QObject{parent}
{
    m_usbInterface = new UsbInterface(this);
    m_bleInterface = new BleInterface(this);

    QObject::connect(m_bleInterface, &AbstractInterface::sgInterfaceUnavaliable, this, &InterfaceManager::slInterfaceUnavaliable);

    QObject::connect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::sgDeviceListUpdated);
    QObject::connect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::sgDeviceListUpdated);
}

InterfaceManager::~InterfaceManager()
{
//    qDebug() << "Interface manager destructor" << this->thread();

    if(m_exchangeInterface)
        m_exchangeInterface->disconnectFromDevice();
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
            m_exchangeInterface = m_bleInterface;
            break;
        }
        case DeviceConnectionType::USBAuto:
        {
            qDebug() << "Settling USB interface";
            m_exchangeInterface = m_usbInterface;
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
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgDeviceUnavaliable, this, &InterfaceManager::sgDeviceUnavaliable);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceError, this, &InterfaceManager::slInterfaceError);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceConnected, this, &InterfaceManager::sgInterfaceConnected);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceDisconnected, this, &InterfaceManager::sgInterfaceDisconnected);

    return m_exchangeInterface->connect(device);
}

void InterfaceManager::disconnectFromDevice()
{
    m_exchangeInterface->disconnectFromDevice();
  //  emit sgInterfaceDisconnected();

    QObject::disconnect(m_exchangeInterface, nullptr, this, nullptr);
    m_exchangeInterface = nullptr;

}

void InterfaceManager::writeToDevice(QByteArray data)
{
    m_exchangeInterface->write(data);
}

void InterfaceManager::startScanning()
{
    m_usbInterface->startScan();
    m_bleInterface->startScan();
}

void InterfaceManager::stopScanning()
{
    m_usbInterface->stopScan();
    m_bleInterface->stopScan();
}

void InterfaceManager::slInterfaceError(QString errorDescription)
{
    disconnectFromDevice();
    emit sgInterfaceError(errorDescription);
}

void InterfaceManager::slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason)
{
#ifdef PANGAEA_DESKTOP

    AbstractInterface* interface_ptr = dynamic_cast<AbstractInterface*>(QObject::sender());
    if(!interface_ptr)
    {
        qWarning() << "Sender dynamic cast failed";
        return;
    }

    if(interface_ptr->connectionType() == DeviceConnectionType::BLE)
    {
        QTimer::singleShot(2500, m_bleInterface, &BleInterface::startScan);
    }
#endif
    emit sgInterfaceUnavaliable(senderType, reason);
}
