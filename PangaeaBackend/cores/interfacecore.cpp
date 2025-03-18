#include <QDebug>
#include <QThread>
#include <QStandardPaths>

#include "interfacecore.h"

InterfaceCore::InterfaceCore(QObject *parent)
                : QObject{parent}
{
#ifndef Q_OS_IOS
    m_usbInterface = new UsbInterface(this);
#endif
    m_bleInterface = new BleInterface(this);
    m_offlineInterface = new OfflineInterface(this);

    connect(this, &InterfaceCore::sgNewData, this, &InterfaceCore::slNewDataArrived);
    connect(m_bleInterface, &BleInterface::rssiReaded, this, &InterfaceCore::sgRssiReaded);

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#elif defined(Q_OS_IOS)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                           + "/settings.plist", QSettings::NativeFormat);
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
        case DeviceConnectionType::USB:
        {
            #ifndef Q_OS_IOS
            qDebug() << "Settling USB interface";
            m_exchangeInterface = m_usbInterface;
            #endif
            break;
        }
        case DeviceConnectionType::Offline:
        {
            qDebug() << "Settling virtual interface";
            m_exchangeInterface = m_offlineInterface;
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
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgModuleNameUpdated, this, &InterfaceCore::sgModuleNameUpdated);

    m_connectedDeviceDescription = device;

    return m_exchangeInterface->connect(device);
}

void InterfaceCore::disconnectFromDevice()
{
    if(m_exchangeInterface != nullptr)
    {
        DeviceDescription disconnectedDevice = m_exchangeInterface->connectedDevice();
        m_exchangeInterface->disconnectFromDevice();
        QObject::disconnect(m_exchangeInterface, nullptr, this, nullptr);
        m_exchangeInterface = nullptr;


        // clear old devices
        QList<DeviceDescription> emptyList;
        emit sgDeviceListUpdated(DeviceConnectionType::BLE, emptyList);
        emit sgDeviceListUpdated(DeviceConnectionType::USB, emptyList);
        emit sgInterfaceDisconnected(disconnectedDevice);
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

void InterfaceCore::startScanning(DeviceConnectionType connectionType)
{
    qDebug() << __FUNCTION__ << "recieve start scanning" << "connection type:" << connectionType;
    switch(connectionType)
    {
    case DeviceConnectionType::BLE:
    {
        QObject::connect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated, Qt::UniqueConnection);
        QObject::connect(m_bleInterface, &AbstractInterface::sgInterfaceUnavaliable, this, &InterfaceCore::slInterfaceUnavaliable, Qt::UniqueConnection);
        m_bleInterface->startScan();
        break;
    }
    case DeviceConnectionType::USB:
    {
        #ifndef Q_OS_IOS
        QObject::connect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated, Qt::UniqueConnection);
        m_usbInterface->startScan();
        #endif
        break;
    }
    case DeviceConnectionType::Offline:
    {
        QObject::connect(m_offlineInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated, Qt::UniqueConnection);
        m_offlineInterface->startScan();
        break;
    }
    default:{}
    }
}

void InterfaceCore::stopScanning()
{
#ifndef Q_OS_IOS
    QObject::disconnect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated);
    m_usbInterface->stopScan();
#endif
    QObject::disconnect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated);
    QObject::disconnect(m_offlineInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceCore::slDeviceListUpdated);

    m_bleInterface->stopScan();
}

void InterfaceCore::setModuleName(QString name)
{
    if(m_bleInterface)
        m_bleInterface->setModuleName(name);
}

void InterfaceCore::rssiMeasuring(bool isEnabled)
{
    if(m_bleInterface)
        m_bleInterface->rssiMeasuring(isEnabled);
}

void InterfaceCore::slNewDataArrived(QByteArray data)
{
    qInfo() << "->" << __FUNCTION__ << ":" << data;
}

void InterfaceCore::slInterfaceError(QString errorDescription)
{
    emit sgErrorDisconnect();
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
        // if(!list.empty())
        // {
        //     connectToDevice(list.first());
        // }
        foreach(DeviceDescription device, list)
        {
            if(device.connectionType() == DeviceConnectionType::Offline)
            {
                continue;
            }
            else
            {
                connectToDevice(device);
                break;
            }
        }
    }
}
