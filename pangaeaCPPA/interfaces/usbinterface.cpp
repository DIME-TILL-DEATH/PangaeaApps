#include <qdebug.h>
#include <QMetaEnum>
#include <QThread>

#include "usbinterface.h"

UsbInterface::UsbInterface(QObject *parent)
    : AbstractInterface{parent}
{
    loadSettings();

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);


    m_port = new QSerialPort(this);
    QIODevice::connect(m_port,  &QSerialPort::readyRead, this, &UsbInterface::slReadyRead);
    QIODevice::connect(m_port,  &QSerialPort::errorOccurred, this, &UsbInterface::slError);
    QIODevice::connect(m_port,  &QSerialPort::destroyed, this, &UsbInterface::slDestroyed);

    QTimer::connect(m_timer, &QTimer::timeout, this, &UsbInterface::slPortTimer);

    m_description = "USB";
    m_connectionType = DeviceConnectionType::USBAuto;
}

void UsbInterface::startScan()
{
    if(state() != InterfaceState::Scanning)
    {
        qDebug() << "Start USB scanning";
        m_timer->start();
        setState(InterfaceState::Scanning);
    }
}

void UsbInterface::stopScan()
{
    m_timer->stop();
    setState(InterfaceState::Idle);
}

void UsbInterface::slPortTimer()
{
    if(!m_port->isOpen())
    {
        discoverDevices();
        emit sgDeviceListUpdated(DeviceConnectionType::USBAuto, m_discoveredDevices);
    }
    else
    {
        checkConnection();
    }
}

void UsbInterface::discoverDevices()
{
    m_discoveredDevices.clear();

    if(isManualConnectAllowed)
    {
        m_discoveredDevices.append(DeviceDescription("Manual connection", manualConnectionPortName, DeviceConnectionType::USBManual));
    }

    auto portList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, portList)
    {
        if(     ((info.vendorIdentifier()==0xff00)&&(info.productIdentifier()==0x0009)) ||
                ((info.vendorIdentifier()==0x0483)&&(info.productIdentifier()==0xA24B)) ||
                ((info.vendorIdentifier()==0x0483)&&(info.productIdentifier()==0x5740))
          )
        {
            //qDebug()<<"vendor:"<<info.vendorIdentifier()<<" product: " <<info.productIdentifier() << " location: "<<info.systemLocation();

            m_discoveredDevices.append(DeviceDescription("AMT pid:" + QString().setNum(info.productIdentifier(), 16) + " " + info.description(),
                                                         info.systemLocation(),
                                                         DeviceConnectionType::USBAuto));
        }
    }
}

bool UsbInterface::connect(DeviceDescription device)
{
    setState(InterfaceState::Connecting);
    qDebug() << "Connecting to device:" << device.name() << " on port: " << device.address();

    if(m_port->isOpen())
    {
        m_port->close();
        qDebug() << __FUNCTION__<<__LINE__<<"Port is already opened, trying to close";
    }

    m_port->setPortName(device.address());
    m_port->setBaudRate(9600);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setFlowControl(QSerialPort::HardwareControl);

    bool isPortOpened = m_port->open(QIODevice::ReadWrite);

    if(isPortOpened)
    {
        qDebug() << __FUNCTION__<< "Serial port is opened";
        m_connectedDevice = device;
        emit sgInterfaceConnected(device);
        setState(InterfaceState::AcquireData);
    }
    else
    {
        prevState();
    }
    return isPortOpened;
}

void UsbInterface::write(QByteArray data)
{
    m_port->readAll();
    m_port->write(data);
}

QList<DeviceDescription> UsbInterface::discoveredDevicesList()
{
    return m_discoveredDevices;
}

void UsbInterface::checkConnection()
{
    // legacy ??????????????. ?????? ???? ???????? ???????????? ???????????? ?????? ???????????????????? USB. ???? ??????????????
    m_port->setBaudRate(9600);
}

void UsbInterface::disconnectFromDevice()
{
    if(m_port->isOpen())
    {
        m_port->close();
        m_timer->stop();
        qDebug() << "Close serial port";
    }
    emit sgInterfaceDisconnected(m_connectedDevice);
    setState(InterfaceState::Idle);
}

void UsbInterface::slReadyRead()
{
    emit sgNewData(m_port->readAll());
}

void UsbInterface::slError(QSerialPort::SerialPortError error)
{
    if(m_port->isOpen())
    {
        m_port->close();

        QMetaEnum errorDescriptionType = QMetaEnum::fromType<QSerialPort::SerialPortError>();
        emit sgInterfaceDisconnected(m_connectedDevice);
        emit sgInterfaceError(errorDescriptionType.valueToKey(error));
    }
}

void UsbInterface::slDestroyed(QObject *obj)
{
    Q_UNUSED(obj)

    QObject *ob;
    ob = qobject_cast<QObject*>(m_port);
    if (ob)
    {
        if(m_port->isOpen())
        {
            m_port->close();
        }
    }
    emit sgInterfaceError("port destroyed");
}

void UsbInterface::loadSettings()
{
    QFile* settingFile = new QFile("local_settings.json");

    if(settingFile->open(QIODevice::ReadOnly))
    {
        QByteArray baFile = settingFile->readAll();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(baFile);
        QJsonObject jsonRoot = jsonDocument.object();

        if(jsonRoot.contains("manual_connection") && jsonRoot["manual_connection"].isObject())
        {
            QJsonObject jsonDeviceObject = jsonRoot["manual_connection"].toObject();

            if(jsonDeviceObject.contains("allowed") && jsonDeviceObject["allowed"].isBool())
            {
                isManualConnectAllowed = jsonDeviceObject["allowed"].toBool();
            }

            if(jsonDeviceObject.contains("port") && jsonDeviceObject["port"].isString())
            {
                manualConnectionPortName = jsonDeviceObject["port"].toString();
            }
        }
    }
    else qDebug() << "Local settings file not found";

    qDebug() << "Manual connection allowed:" << isManualConnectAllowed << ", port: " << manualConnectionPortName;
}
