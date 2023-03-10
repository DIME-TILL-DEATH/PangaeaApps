#include <QMetaEnum>
#include <QStandardPaths>

#ifdef Q_OS_ANDROID
#include <QGeoPositionInfoSource>
#include <QtCore/private/qandroidextras_p.h>
#include "androidutils.h"
#endif

#include "bleinterface.h"

BleInterface::BleInterface(QObject *parent)
    : AbstractInterface{parent},
    m_control{nullptr},
    m_service{nullptr}
{
#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings(QSettings::UserScope, this);
#endif
    appSettings->setFallbacksEnabled(false);
    appSettings->beginGroup("AutoconnectSettings");
    m_autoconnectDevicetMAC = appSettings->value("MAC address", 0).toString();
    appSettings->endGroup();

    int size = appSettings->beginReadArray("Unique module names");
    for(int i=0; i<size; ++i)
    {
        appSettings->setArrayIndex(i);

        QString MACAddress = appSettings->value("MAC address").toString();
        QString name = appSettings->value("Name").toString();
        m_moduleUniqueNames.insert(MACAddress, name);
    }
    appSettings->endArray();

    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(); // parent = this: Main COM uninit tried from another thread

    QBluetoothDeviceDiscoveryAgent::connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BleInterface::addDevice, Qt::QueuedConnection);

    QBluetoothDeviceDiscoveryAgent::connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &BleInterface::deviceScanError, Qt::QueuedConnection);

    QBluetoothDeviceDiscoveryAgent::connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BleInterface::scanTimeout, Qt::QueuedConnection);

//    QObject::connect(this, &BleInterface::sgStartSearch, m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::start, Qt::QueuedConnection);

    QObject::connect(this, &QObject::destroyed, m_deviceDiscoveryAgent, &QObject::deleteLater);

    m_description = "BLE";
    m_connectionType = DeviceConnectionType::BLE;
}

BleInterface::~BleInterface()
{
//    qDebug() << "BLE interface destructor";
}

void BleInterface::startScan()
{    
    if(state() == InterfaceState::Idle)
    {
        startDiscovering();
    }
}

void BleInterface::stopScan()
{
    if(m_deviceDiscoveryAgent)
    {
        m_deviceDiscoveryAgent->stop();
    }
    setState(InterfaceState::Idle);
}

void BleInterface::startDiscovering()
{
    QBluetoothLocalDevice device;

    if(device.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
//        qDebug() << "Controller is powered off";
        prevState();
        isAvaliable = false;

        m_qlFoundDevices.clear();
        emit sgDeviceListUpdated(DeviceConnectionType::BLE, m_qlFoundDevices);

        //emit sgLocalBluetoothNotReady("HostPoweredOff");
        emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "HostPoweredOff");
        device.powerOn();
        return;
    }

#ifdef Q_OS_ANDROID
    bool result = AndroidUtils::checkPermission("android.permission.ACCESS_COARSE_LOCATION");
    if(!result)
    {
        result = AndroidUtils::requestPermission("android.permission.ACCESS_COARSE_LOCATION");
        if(!result)
        {
            qDebug() << "Geolocation permission denied";
            isAvaliable = false;
            emit sgLocalBluetoothNotReady("GeolocationPermissionDenied");
            return;
        }
        else
        {
            if(QGeoPositionInfoSource::createDefaultSource(this)->supportedPositioningMethods() == QGeoPositionInfoSource::NoPositioningMethods)
            {
                qDebug() << "Geolocation is off";
                isAvaliable = false;
                emit sgLocalBluetoothNotReady("GeolocationIsOff");
                return;
            }
        }
    }

#endif


    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    #ifdef Q_OS_MACOS
        m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);
        m_deviceDiscoveryAgent->start();
    #endif

    isAvaliable = true;
    setState(InterfaceState::Scanning);
}

void BleInterface::scanTimeout()
{
//    qInfo() << "BLE scan timeout";
    checkDevicesAvaliabliy();
    updateBLEDevicesList();

    if(state() == InterfaceState::Scanning)
    {
        qDebug() << "Restart scan";
        startDiscovering();
    }
    else
    {
        qDebug() << "Stop scan";
        m_deviceDiscoveryAgent->stop();
    }
}

void BleInterface::addDevice(const QBluetoothDeviceInfo &device)
{
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        qInfo() << "Discovered LE Device name: " << device.name() << " Address: "
                   << device.address().toString();
    }

    bool isAutoconnectEnabled = appSettings->value("autoconnect_enable").toBool();

    if(device.name().indexOf("AMT PANGAEA") == 0 )
    {
        QString macAddress = device.address().toString();
        if(!m_avaliableDevices.contains(device))
        {
            m_avaliableDevices.append(device);
            if(isAutoconnectEnabled & (state() == InterfaceState::Scanning))
            {
              if(macAddress.indexOf(m_autoconnectDevicetMAC) == 0)
              {
                  qDebug() << "Autoconnecting";
                  m_connectedDevice = DeviceDescription(device.name(), macAddress, DeviceConnectionType::BLE);
                  setState(InterfaceState::Connecting);

                  slStartConnect(macAddress);
                  emit sgConnectionStarted();
              }
            }
            updateBLEDevicesList();
        }
    }
}

void BleInterface::updateBLEDevicesList()
{
  m_qlFoundDevices.clear();

  if(!isAvaliable) return;

  for (const QBluetoothDeviceInfo &currentDevice : m_avaliableDevices)
  {
      QString uniqueName;
      QString strAddress = currentDevice.address().toString();
      QString strName = m_moduleUniqueNames.value(strAddress, "");
      if(m_moduleUniqueNames.contains(strAddress) && strName!="")
      {
        uniqueName = "-" + strName;
      }

      DeviceDescription currentDescription{currentDevice.name() + uniqueName, strAddress, DeviceConnectionType::BLE};
      m_qlFoundDevices.append(currentDescription);
  }
  emit sgDeviceListUpdated(DeviceConnectionType::BLE, m_qlFoundDevices);
}

void BleInterface::checkDevicesAvaliabliy()
{
    for(int i=0; i < m_avaliableDevices.size(); i++)
    {
        QString macAddress = m_avaliableDevices.at(i).address().toString();
        const QBluetoothDeviceInfo* dev_ptr = getDeviceByMAC(macAddress);

        if(dev_ptr == nullptr) m_avaliableDevices.removeAt(i);
    }
}

void BleInterface::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    switch(error)
    {
        case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
        {
            qDebug() << __FUNCTION__ << "The Bluetooth adapter is powered off, power it on before doing discovery.";
            emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "HostPoweredOff");
            //emit sgLocalBluetoothNotReady("HostPoweredOff");
            break;
        }
        case QBluetoothDeviceDiscoveryAgent::UnknownError:
        {
            qDebug() << __FUNCTION__ << "An unknown error has occurred.";
            emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "UnknownBleError");
//            emit sgLocalBluetoothNotReady("UnknownBleError");
            break;
        }
        default:
        {
            QMetaEnum errorString = QMetaEnum::fromType<QBluetoothDeviceDiscoveryAgent::Error>();
            qDebug() << __FUNCTION__ << errorString.valueToKey(error);

            if(state() != InterfaceState::Connected)
            {
                checkDevicesAvaliabliy();
                updateBLEDevicesList();
                startScan();
            }
        }
    }
}

QList<DeviceDescription> BleInterface::discoveredDevicesList()
{
    return m_qlFoundDevices;
}

bool BleInterface::connect(DeviceDescription device)
{
//    if(state() == InterfaceState::AcquireData)
//        return true;

//    if(state() == InterfaceState::Scanning)
//    {
        m_connectedDevice = device;
     //   doConnect(0, device.address());
        setState(InterfaceState::Connecting);

        slStartConnect(device.address());
        emit sgConnectionStarted();
//    }

//    return false;
        return true;
}

//void BleInterface::doConnect(qint8 numDev, QString address)
//{
////    if(numDev < m_avaliableDevices.count())
////    {
//        slStartConnect(address);
//       // emit sgConnect(numDev);
////    }
////    else qDebug() << "devNumber is less than avaliable devices";
//}

void BleInterface::slStartConnect(QString address)
{
    qDebug() << "Trying to connect";

    const QBluetoothDeviceInfo* deviceToConnect = getDeviceByMAC(address);

    if(deviceToConnect == nullptr)
    {
        qDebug() << "Device is unavalibale";
        emit sgDeviceUnavaliable(DeviceConnectionType::BLE, "nullPointer");
        prevState();
        startScan();
//        emit sgLocalBluetoothNotReady("DeviceUnavaliable");
        return;
    }

    m_autoconnectDevicetMAC = deviceToConnect->address().toString();

    appSettings->setFallbacksEnabled(false);
    appSettings->beginGroup("AutoconnectSettings");
    appSettings->setValue("MAC address", m_autoconnectDevicetMAC);
    appSettings->endGroup();

    m_moduleName = m_moduleUniqueNames.value(m_autoconnectDevicetMAC, "");

    setState(InterfaceState::UpdateModuleName);

    if (m_control)
    {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = 0;
    }

    m_control = QLowEnergyController::createCentral(*deviceToConnect, this);
    m_control->setRemoteAddressType(QLowEnergyController::RandomAddress);

    QBluetoothDeviceDiscoveryAgent::connect(m_control, &QLowEnergyController::connected,
            this, &BleInterface::deviceConnected);

    QBluetoothDeviceDiscoveryAgent::connect(m_control, &QLowEnergyController::disconnected,
            this, &BleInterface::deviceDisconnected);

    QBluetoothDeviceDiscoveryAgent::connect(m_control, &QLowEnergyController::serviceDiscovered,
            this, &BleInterface::serviceDiscovered);

    QBluetoothDeviceDiscoveryAgent::connect(m_control, &QLowEnergyController::discoveryFinished,
            this, &BleInterface::serviceScanDone);

    QBluetoothDeviceDiscoveryAgent::connect(m_control, &QLowEnergyController::errorOccurred,
            this, &BleInterface::controllerError);

    qDebug() << m_control->state();
    m_control->connectToDevice();
    setState(Connecting);
}

void BleInterface::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if(gatt==QBluetoothUuid(QUuid(UARTSERVICEUUID)))
    {
        m_bFoundUARTService =true;
        qDebug() << "UART service found!";
    }
    qDebug() << __FUNCTION__ << gatt;
}

void BleInterface::serviceScanDone()
{
    delete m_service;
    m_service=0;

    if(m_bFoundUARTService)
    {
        m_service = m_control->createServiceObject(QBluetoothUuid(QUuid(UARTSERVICEUUID)),this);
    }

    if(!m_service)
    {
        qDebug() <<"UART service not found";
        return;
    }

    QBluetoothDeviceDiscoveryAgent::connect(m_service, &QLowEnergyService::stateChanged,
            this, &BleInterface::serviceStateChanged);
    QBluetoothDeviceDiscoveryAgent::connect(m_service, &QLowEnergyService::characteristicChanged,
            this, &BleInterface::updateData);
    QBluetoothDeviceDiscoveryAgent::connect(m_service, &QLowEnergyService::descriptorWritten,
            this, &BleInterface::confirmedDescriptorWrite);

    m_service->discoverDetails();
    setState(ServiceFound);
}

void BleInterface::deviceConnected()
{
    setState(Connected);
    m_control->discoverServices();
}

void BleInterface::deviceDisconnected()
{
    qWarning() << "Remote device disconnected(UART service disconnected)";

  //  emit sgErrorDisconnected();
  //  emit sgLocalBluetoothNotReady("DeviceUnavaliable");
    emit sgInterfaceDisconnected(m_connectedDevice);
}

void BleInterface::controllerError(QLowEnergyController::Error error)
{
    qDebug() << "Cannot connect to remote device.";
    qWarning() << "Controller Error:" << error;
    emit sgInterfaceError("Controller error");
}


void BleInterface::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch(s)
    {
    case QLowEnergyService::RemoteServiceDiscovered:
    {
       const QLowEnergyCharacteristic TxChar = m_service->characteristic(QBluetoothUuid(QUuid(TXUUID)));
       const QLowEnergyCharacteristic  RxChar = m_service->characteristic(QBluetoothUuid(QUuid(RXUUID)));

       const QLowEnergyDescriptor m_notificationDescTx = TxChar.clientCharacteristicConfiguration();
       if (m_notificationDescTx.isValid())
       {
           m_service->writeDescriptor(m_notificationDescTx, QByteArray::fromHex("0100"));           
       }
       break;
    }
    default:
        //nothing for now
        break;
    }
}

void BleInterface::updateData(const QLowEnergyCharacteristic &c,const QByteArray &value)
{
    // ignore any other characteristic change
    if (c.uuid() != QBluetoothUuid(QUuid(TXUUID)))
           return;

    emit sgNewData(value);
}

void BleInterface::confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                                         const QByteArray &value)
{
    if (d.isValid() && d == m_notificationDescTx && value == QByteArray("0000"))
    {
        m_control->disconnectFromDevice();

        if(m_service != nullptr)
        {
            delete m_service;
            m_service = 0;
        }
    }
    setState(AcquireData);
    emit sgInterfaceConnected(m_connectedDevice);
}

void BleInterface::write(QByteArray data)
{
    const QLowEnergyCharacteristic  RxChar = m_service->characteristic(QBluetoothUuid(QUuid(RXUUID)));

    QByteArray Data;
    Data.append(data);

    m_service->writeCharacteristic(RxChar, Data, QLowEnergyService::WriteWithoutResponse);
}

void BleInterface::disconnectFromDevice()
{
    qDebug() << "BLE disconnecting from device";

    if (m_control)
    {
        m_control->disconnectFromDevice();
        QBluetoothDeviceDiscoveryAgent::disconnect(m_control, nullptr, nullptr, nullptr);
    }

    if(m_service)
    {
        QBluetoothDeviceDiscoveryAgent::disconnect(m_service, nullptr, nullptr, nullptr);
    }
   // QBluetoothDeviceDiscoveryAgent::disconnect(this, &BleInterface::sgNewData, nullptr, nullptr);

    setState(InterfaceState::Idle);
    m_avaliableDevices.clear();
}

const QBluetoothDeviceInfo *BleInterface::getDeviceByMAC(const QString &macAddress)
{
    for(int i=0; i< m_deviceDiscoveryAgent->discoveredDevices().size(); i++)
    {
        if(m_deviceDiscoveryAgent->discoveredDevices().at(i).address().toString() == macAddress)
            return &m_deviceDiscoveryAgent->discoveredDevices().at(i);
    }
    return nullptr;
}

void BleInterface::setModuleName(QString name)
{
    if(name != m_moduleName)
    {
        m_moduleName = name;

        m_moduleUniqueNames.insert(m_autoconnectDevicetMAC, m_moduleName);

#ifdef Q_OS_ANDROID
        QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                           + "/settings.conf", QSettings::NativeFormat);
#else
        QSettings settings(QSettings::UserScope);
#endif
        settings.setFallbacksEnabled(false);
        int i=0;

        settings.beginWriteArray("Unique module names");
        for(QHash<QString, QString>::iterator it=m_moduleUniqueNames.begin(); it!=m_moduleUniqueNames.end(); ++it)
        {
            settings.setArrayIndex(i);
            settings.setValue("MAC address", it.key());
            settings.setValue("Name", it.value());
            i++;
        }
        settings.endArray();
        settings.sync();
    }
}

const QString &BleInterface::moduleName() const
{
    return m_moduleName;
}
