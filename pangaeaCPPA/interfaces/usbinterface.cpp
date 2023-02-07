#include <qdebug.h>
#include <QMetaEnum>

#include "usbinterface.h"

UsbInterface::UsbInterface(QObject *parent)
    : AbstractInterface{parent}
{
    loadSettings();

    m_port = new QSerialPort(this);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();
    QIODevice::connect(m_port,  &QSerialPort::readyRead, this, &UsbInterface::slReadyRead);

    //TODO обработчики ошибок
    QIODevice::connect(m_port,  &QSerialPort::errorOccurred, this, &UsbInterface::slError);
    QIODevice::connect(m_port,  &QSerialPort::destroyed, this, &UsbInterface::slDestroyed);

    QTimer::connect(m_timer, &QTimer::timeout, this, &UsbInterface::slPortTimer);
}

void UsbInterface::discoverDevices()
{
    setState(InterfaceState::Scanning);
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
            qDebug()<<"vendor:"<<info.vendorIdentifier()<<" product: " <<info.productIdentifier() << " location: "<<info.systemLocation();

            m_discoveredDevices.append(DeviceDescription(QString().setNum(info.productIdentifier(), 16), info.systemLocation(), DeviceConnectionType::USBAuto));
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
        emit sgInterfaceConnected();
        setState(InterfaceState::AcquireData);
    }
    else
    {
        prevState();
    }
    return isPortOpened;
}

bool UsbInterface::isConnected()
{
    return m_port->isOpen();
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
    // legacy костыль. Так до меня ловили ошибку при отключении USB. Не убирать
    m_port->setBaudRate(9600);
}

void UsbInterface::disconnect()
{
    if(m_port->isOpen())
    {
        m_port->close();
        qDebug()<<"CLOSE PORT";
        setState(InterfaceState::Idle);
    }
}

QString UsbInterface::connectionDescription()
{
    return m_port->portName();
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

void UsbInterface::slPortTimer()
{
    if(!isConnected())
    {
        qDebug()<<"Serial port search timeout";

        discoverDevices();
        if(discoveredDevicesList().size() != 0)
        {
            emit sgDeviceListUpdated();
        }
    }
    else
    {
        checkConnection();
    }
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
