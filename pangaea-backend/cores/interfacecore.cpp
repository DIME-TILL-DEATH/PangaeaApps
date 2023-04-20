#include <QDebug>
#include <QThread>
#include <QSettings>

#include "interfacecore.h"

InterfaceCore::InterfaceCore(QObject *parent)
                : QObject{parent}
{
    m_usbInterface = new UsbInterface(this);
    m_bleInterface = new BleInterface(this);

    QObject::connect(m_bleInterface, &AbstractInterface::sgInterfaceUnavaliable, this, &InterfaceCore::slInterfaceUnavaliable);

    QObject::connect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated);
    QObject::connect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated);
}

InterfaceCore::~InterfaceCore()
{
//    qDebug() << "Interface manager destructor" << this->thread();

    if(m_exchangeInterface)
        m_exchangeInterface->disconnectFromDevice();
}

bool InterfaceCore::connectToDevice(DeviceDescription device)
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
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgNewData, this, &InterfaceCore::sgNewData);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgConnectionStarted, this, &InterfaceCore::sgConnectionStarted);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgDeviceUnavaliable, this, &InterfaceCore::sgDeviceUnavaliable);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceError, this, &InterfaceCore::slInterfaceError);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceConnected, this, &InterfaceCore::sgInterfaceConnected);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceDisconnected, this, &InterfaceCore::sgInterfaceDisconnected);

    m_connectedDeviceDescription = device;

    return m_exchangeInterface->connect(device);
}

void InterfaceCore::disconnectFromDevice()
{
    if(m_exchangeInterface != nullptr)
    {
        m_exchangeInterface->disconnectFromDevice();
        QObject::disconnect(m_exchangeInterface, nullptr, this, nullptr);
        m_exchangeInterface = nullptr;
    }
}

void InterfaceCore::writeToDevice(QByteArray data)
{
    m_exchangeInterface->write(data);
}

void InterfaceCore::startScanning()
{
    m_usbInterface->startScan();
    m_bleInterface->startScan();
}

void InterfaceCore::stopScanning()
{
    m_usbInterface->stopScan();
    m_bleInterface->stopScan();
}

void InterfaceCore::slInterfaceError(QString errorDescription)
{
    disconnectFromDevice();
    emit sgInterfaceError(errorDescription);
}

void InterfaceCore::slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason)
{
#ifndef Q_OS_ANDROID
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

void InterfaceCore::slDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list)
{
    emit sgDeviceListUpdated(connectionType, list);

    bool makeAutoconnect = QSettings(QSettings::UserScope).value("autoconnect_enable", false).toBool();

    if(m_exchangeInterface == nullptr && makeAutoconnect)
    {
        if(!list.empty())
        {
            connectToDevice(list.first());
        }
    }
}
