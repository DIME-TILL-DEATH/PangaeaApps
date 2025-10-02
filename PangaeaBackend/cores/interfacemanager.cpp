#include <QDebug>
#include <QThread>
#include <QStandardPaths>

#include "interfacemanager.h"

InterfaceManager::InterfaceManager(QObject *parent)
                : QObject{parent}
{
#ifndef Q_OS_IOS
    m_usbInterface = new UsbInterface(this);
#endif
    m_bleInterface = new BleInterface(this);
    m_offlineInterface = new OfflineInterface(this);

    connect(this, &InterfaceManager::sgNewData, this, &InterfaceManager::slNewDataArrived);
    connect(m_bleInterface, &BleInterface::rssiReaded, this, &InterfaceManager::sgRssiReaded);

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

InterfaceManager::~InterfaceManager()
{
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
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgNewData, this, &InterfaceManager::sgNewData);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgConnectionStarted, this, &InterfaceManager::sgConnectionStarted);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgDeviceUnavaliable, this, &InterfaceManager::sgDeviceUnavaliable);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceError, this, &InterfaceManager::slInterfaceError);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceConnected, this, &InterfaceManager::sgInterfaceConnected);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgInterfaceDisconnected, this, &InterfaceManager::sgInterfaceDisconnected);
    QObject::connect(m_exchangeInterface, &AbstractInterface::sgModuleNameUpdated, this, &InterfaceManager::sgModuleNameUpdated);

    m_connectedDeviceDescription = device;

    return m_exchangeInterface->connect(device);
}

void InterfaceManager::disconnectFromDevice()
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

void InterfaceManager::writeToDevice(QByteArray data)
{
    if(m_exchangeInterface)
    {
        if(m_logCommand) qInfo() << "<- writeData:" << data << "hex:" << data.toHex() << "lenght:" << data.length();

        m_exchangeInterface->write(data);
    }
}

void InterfaceManager::slNewDataArrived(QByteArray data)
{
    if(m_logCommand) qInfo() << "->" << __FUNCTION__ << ":" << data;
}

void InterfaceManager::startScanning(DeviceConnectionType connectionType)
{
    qDebug() << __FUNCTION__ << "recieve start scanning" << "connection type:" << connectionType;
    switch(connectionType)
    {
    case DeviceConnectionType::BLE:
    {
        QObject::connect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::slDeviceListUpdated, Qt::UniqueConnection);
        QObject::connect(m_bleInterface, &AbstractInterface::sgInterfaceUnavaliable, this, &InterfaceManager::slInterfaceUnavaliable, Qt::UniqueConnection);
        m_bleInterface->startScan();
        break;
    }
    case DeviceConnectionType::USB:
    {
        #ifndef Q_OS_IOS
        QObject::connect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::slDeviceListUpdated, Qt::UniqueConnection);
        m_usbInterface->startScan();
        #endif
        break;
    }
    case DeviceConnectionType::Offline:
    {
        QObject::connect(m_offlineInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::slDeviceListUpdated, Qt::UniqueConnection);
        m_offlineInterface->startScan();
        break;
    }
    default:{}
    }
}

void InterfaceManager::stopScanning()
{
#ifndef Q_OS_IOS
    QObject::disconnect(m_usbInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::slDeviceListUpdated);
    m_usbInterface->stopScan();
#endif
    QObject::disconnect(m_bleInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::slDeviceListUpdated);
    QObject::disconnect(m_offlineInterface, &AbstractInterface::sgDeviceListUpdated, this, &InterfaceManager::slDeviceListUpdated);

    m_bleInterface->stopScan();
}

void InterfaceManager::setModuleName(QString name)
{
    if(m_bleInterface)
        m_bleInterface->setModuleName(name);
}

void InterfaceManager::rssiMeasuring(bool isEnabled)
{
    if(m_bleInterface)
        m_bleInterface->rssiMeasuring(isEnabled);
}

void InterfaceManager::slInterfaceError(QString errorDescription)
{
    emit sgErrorDisconnect();
    emit sgInterfaceError(errorDescription);
}

void InterfaceManager::slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason)
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

void InterfaceManager::slDeviceListUpdated(DeviceConnectionType connectionType, QList<DeviceDescription> list)
{
    emit sgDeviceListUpdated(connectionType, list);

    bool makeAutoconnect = appSettings->value("autoconnect_enable", false).toBool();

    if(m_exchangeInterface == nullptr && makeAutoconnect)
    {
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

void InterfaceManager::setLogEnadled(bool logEnabled)
{
    m_logCommand = logEnabled;
}
