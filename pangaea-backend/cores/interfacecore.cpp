#include <QDebug>
#include <QThread>
#include <QStandardPaths>

#include "interfacecore.h"

InterfaceCore::InterfaceCore(QObject *parent)
                : QObject{parent}
{
    m_usbInterface = new UsbInterface(this);
    m_bleInterface = new BleInterface(this);

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings(QSettings::UserScope);
#endif
}

InterfaceCore::~InterfaceCore()
{
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
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgNewData, this, &InterfaceCore::sgNewData, Qt::UniqueConnection);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgConnectionStarted, this, &InterfaceCore::sgConnectionStarted, Qt::UniqueConnection);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgDeviceUnavaliable, this, &InterfaceCore::sgDeviceUnavaliable, Qt::UniqueConnection);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceError, this, &InterfaceCore::slInterfaceError, Qt::UniqueConnection);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceConnected, this, &InterfaceCore::sgInterfaceConnected, Qt::UniqueConnection);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceDisconnected, this, &InterfaceCore::sgInterfaceDisconnected, Qt::UniqueConnection);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgModuleNameUpdated, this, &InterfaceCore::sgModuleNameUpdated, Qt::UniqueConnection);

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


        // clear old devices
        QList<DeviceDescription> emptyList;
        emit sgDeviceListUpdated(DeviceConnectionType::BLE, emptyList);
        emit sgDeviceListUpdated(DeviceConnectionType::USBAuto, emptyList);
    }
}

void InterfaceCore::writeToDevice(QByteArray data, bool logCommand)
{
    if(m_exchangeInterface)
    {
        if(logCommand) qInfo() << "<- writeData:" << data << "hex:" << data.toHex() << "lenght:" << data.length();

        m_exchangeInterface->write(data);
    }
}

void InterfaceCore::startScanning()
{
    QObject::connect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated, Qt::UniqueConnection);
    QObject::connect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated, Qt::UniqueConnection);

    QObject::connect(m_bleInterface, &AbstractInterface::sgInterfaceUnavaliable, this, &InterfaceCore::slInterfaceUnavaliable, Qt::UniqueConnection);

    m_usbInterface->startScan();
    m_bleInterface->startScan();
}

void InterfaceCore::stopScanning()
{
    m_usbInterface->stopScan();
    m_bleInterface->stopScan();
}

void InterfaceCore::setModuleName(QString name)
{
    if(m_bleInterface)
        m_bleInterface->setModuleName(name);
}

// в таком варианте иногда вызывает crash на ошибки буффера
//void InterfaceCore::slNewData(QByteArray data)
//{
//    qDebug() << "->" << __FUNCTION__ << ":" << data;
//    emit sgNewData(data);
//}

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

    bool makeAutoconnect = appSettings->value("autoconnect_enable", false).toBool();

    if(m_exchangeInterface == nullptr && makeAutoconnect)
    {
        if(!list.empty())
        {
            connectToDevice(list.first());
        }
    }
}
