#include <QMetaEnum>
#include <QStandardPaths>

#ifdef Q_OS_ANDROID
#include <QGeoPositionInfoSource>
#include <QtCore/private/qandroidextras_p.h>
#include "androidutils.h"
#endif

#include "bluetoothleuart.h"

BluetoothleUART::BluetoothleUART() :
    m_control{nullptr},
    m_service{nullptr},
    m_state(bluetoothleState::Idle)
{
#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings(QSettings::UserScope);
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
}

void BluetoothleUART::startScan()
{
#ifdef Q_OS_ANDROID
    QBluetoothLocalDevice device;

    if(device.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
        qDebug() << "Controller is powered off";
        emit sgLocalBluetoothNotReady("HostPoweredOff");
        device.powerOn();

        return;
    }

    bool result = AndroidUtils::checkPermission("android.permission.ACCESS_COARSE_LOCATION");
    if(!result)
    {
        result = AndroidUtils::requestPermission("android.permission.ACCESS_COARSE_LOCATION");
        if(!result)
        {
            qDebug() << "Geolocation permission denied";
            emit sgLocalBluetoothNotReady("GeolocationPermissionDenied");
            return;
        }
        else
        {
            if(QGeoPositionInfoSource::createDefaultSource(this)->supportedPositioningMethods() == QGeoPositionInfoSource::NoPositioningMethods)
            {
                qDebug() << "Geolocation is off";
                emit sgLocalBluetoothNotReady("GeolocationIsOff");
                return;
            }
        }
    }

#endif
    if(m_deviceDiscoveryAgent == nullptr)
    {
        m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

        connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                this, &BluetoothleUART::addDevice);

        connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
                this, &BluetoothleUART::deviceScanError);

        connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                this, &BluetoothleUART::scanTimeout);
    }
    #ifdef Q_OS_ANDROID
        m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);
        m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    #endif

    #ifdef Q_OS_WINDOWS
        m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);
        m_deviceDiscoveryAgent->start();
    #endif

    setState(bluetoothleState::Scanning);

    qDebug()<< "Searching for low energy devices..." ;
}

void BluetoothleUART::scanTimeout()
{
    qInfo() << "Scan timeout";
    checkDevicesAvaliabliy();
    updateBLEDevicesList();

    if((m_state == bluetoothleState::Scanning) | (m_state==bluetoothleState::UpdateDeviceList))
    {
        qDebug() << "Restart scan";
        startScan();
    }
    else
    {
        qDebug() << "Stop scan";
        m_deviceDiscoveryAgent->stop();
    }
}

void BluetoothleUART::addDevice(const QBluetoothDeviceInfo &device)
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
            if(isAutoconnectEnabled & (m_state == bluetoothleState::Scanning))
            {
              if(macAddress.indexOf(m_autoconnectDevicetMAC) == 0)
              {
                  qDebug() << "Autoconnecting";
                  doConnect(m_avaliableDevices.count()-1, macAddress);
              }
            }
            updateBLEDevicesList();
        }
    }
}

void BluetoothleUART::updateBLEDevicesList()
{
  m_qlFoundDevices.clear();

  for (const QBluetoothDeviceInfo &currentDevice : m_avaliableDevices)
  {
      QString uniqueName;
      QString strAddress = currentDevice.address().toString();
      QString strName = m_moduleUniqueNames.value(strAddress, "");
      if(m_moduleUniqueNames.contains(strAddress) && strName!="")
      {
        uniqueName = "-" + strName;
      }

      m_qlFoundDevices.append(currentDevice.name() + uniqueName
                              + "\r\n"
                              + strAddress);
  }
  setState(UpdateDeviceList);
}

void BluetoothleUART::checkDevicesAvaliabliy()
{
    for(int i=0; i < m_avaliableDevices.size(); i++)
    {
        QString macAddress = m_avaliableDevices.at(i).address().toString();
        const QBluetoothDeviceInfo* dev_ptr = getDeviceByMAC(macAddress);

        if(dev_ptr == nullptr) m_avaliableDevices.removeAt(i);
    }
}

void BluetoothleUART::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    switch(error)
    {
        case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
        {
            qDebug() << __FUNCTION__ << "The Bluetooth adapter is powered off, power it on before doing discovery.";
            emit sgLocalBluetoothNotReady("HostPoweredOff");
            break;
        }
        case QBluetoothDeviceDiscoveryAgent::UnknownError:
        {
            qDebug() << __FUNCTION__ << "An unknown error has occurred.";
            emit sgLocalBluetoothNotReady("UnknownBleError");
            break;
        }
        default:
        {
            QMetaEnum errorString = QMetaEnum::fromType<QBluetoothDeviceDiscoveryAgent::Error>();
            qDebug() << __FUNCTION__ << errorString.valueToKey(error);

            if(m_state != bluetoothleState::Connected)
            {
                checkDevicesAvaliabliy();
                updateBLEDevicesList();
                startScan();
            }
        }
    }
}

void BluetoothleUART::doConnect(qint8 numDev, QString address)
{
    if(numDev < m_avaliableDevices.count())
    {
        slStartConnect(address);
        emit sgConnect(numDev);
    }
    else qDebug() << "devNumber is less than avaliable devices";
}

void BluetoothleUART::slStartConnect(QString address)
{
    qDebug() << "Trying to connect";
    m_qvMeasurements.clear();

    const QBluetoothDeviceInfo* deviceToConnect = getDeviceByMAC(address);

    if(deviceToConnect == nullptr)
    {
        qDebug() << "Device is unavalibale";
        emit sgLocalBluetoothNotReady("DeviceUnavaliable");
        return;
    }

    m_autoconnectDevicetMAC = deviceToConnect->address().toString();

    appSettings->setFallbacksEnabled(false);
    appSettings->beginGroup("AutoconnectSettings");
    appSettings->setValue("MAC address", m_autoconnectDevicetMAC);
    appSettings->endGroup();

    m_moduleName = m_moduleUniqueNames.value(m_autoconnectDevicetMAC, "");

    setState(bluetoothleState::UpdateModuleName);

    if (m_control)
    {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = 0;
    }

    m_control = QLowEnergyController::createCentral(*deviceToConnect, this);
    m_control->setRemoteAddressType(QLowEnergyController::RandomAddress);

    connect(m_control, &QLowEnergyController::connected,
            this, &BluetoothleUART::deviceConnected);

    connect(m_control, &QLowEnergyController::disconnected,
            this, &BluetoothleUART::deviceDisconnected);

    connect(m_control, &QLowEnergyController::serviceDiscovered,
            this, &BluetoothleUART::serviceDiscovered);

    connect(m_control, &QLowEnergyController::discoveryFinished,
            this, &BluetoothleUART::serviceScanDone);

    connect(m_control, &QLowEnergyController::errorOccurred,
            this, &BluetoothleUART::controllerError);

    qDebug() << m_control->state();
    m_control->connectToDevice();
    setState(Connecting);
}

void BluetoothleUART::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if(gatt==QBluetoothUuid(QUuid(UARTSERVICEUUID)))
    {
        m_bFoundUARTService =true;
        qDebug() << "UART service found!";
    }
    qDebug() << __FUNCTION__ << __LINE__ << gatt;
}

void BluetoothleUART::serviceScanDone()
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

    connect(m_service, &QLowEnergyService::stateChanged,
            this, &BluetoothleUART::serviceStateChanged);
    connect(m_service, &QLowEnergyService::characteristicChanged,
            this, &BluetoothleUART::updateData);
    connect(m_service, &QLowEnergyService::descriptorWritten,
            this, &BluetoothleUART::confirmedDescriptorWrite);

    m_service->discoverDetails();
    setState(ServiceFound);
}

void BluetoothleUART::deviceConnected()
{
    qDebug() << "Device connected";

    m_control->discoverServices();
    setState(Connected);
}

void BluetoothleUART::doDisconnect()
{
    qDebug() << "Disconnecting";

    m_control->disconnectFromDevice();

    disconnect(m_control, nullptr, nullptr, nullptr);
    disconnect(m_service, nullptr, nullptr, nullptr);
    disconnect(this, &BluetoothleUART::newData, nullptr, nullptr);

    m_avaliableDevices.clear();

    startScan();
    updateBLEDevicesList();
}

void BluetoothleUART::deviceDisconnected()
{
    qDebug() << "UART service disconnected";
    qWarning() << "Remote device disconnected";

    emit sgErrorDisconnected();
    emit sgLocalBluetoothNotReady("DeviceUnavaliable");
}

void BluetoothleUART::controllerError(QLowEnergyController::Error error)
{
    qDebug() << "Cannot connect to remote device.";
    qWarning() << "Controller Error:" << error;
}


void BluetoothleUART::serviceStateChanged(QLowEnergyService::ServiceState s)
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
           setState(AcquireData);
       }
       break;
    }
    default:
        //nothing for now
        break;
    }
}

void BluetoothleUART::updateData(const QLowEnergyCharacteristic &c,const QByteArray &value)
{
    // ignore any other characteristic change
    if (c.uuid() != QBluetoothUuid(QUuid(TXUUID)))
           return;

    emit newData(value);
}

void BluetoothleUART::confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                                         const QByteArray &value)
{
    qDebug() << "Confirmed descriptor written, disconnet from device";

    if (d.isValid() && d == m_notificationDescTx && value == QByteArray("0000"))
    {
        //disabled notifications -> assume disconnect intent
        m_control->disconnectFromDevice();
        delete m_service;
        m_service = 0;
    }
}

void BluetoothleUART::writeData(QByteArray s)
{
    const QLowEnergyCharacteristic  RxChar = m_service->characteristic(QBluetoothUuid(QUuid(RXUUID)));

    QByteArray Data;
    Data.append(s);

    m_service->writeCharacteristic(RxChar, Data, QLowEnergyService::WriteWithoutResponse);
}

void BluetoothleUART::setState(BluetoothleUART::bluetoothleState newState)
{
    QMetaEnum enumStates = QMetaEnum::fromType<BluetoothleUART::bluetoothleState >();
    qDebug() << "State changed:" << enumStates.valueToKey(newState);

    if (m_state == newState)
            return;

    m_prevState = m_state;
    m_state = newState;
    emit changedState(newState);
}

BluetoothleUART::bluetoothleState BluetoothleUART::getState() const
{
    return m_state;
}

void BluetoothleUART::prevState()
{
    m_state = m_prevState;

    QMetaEnum enumStates = QMetaEnum::fromType<BluetoothleUART::bluetoothleState >();
    qDebug() << "State changed. Return previous state" << enumStates.valueToKey(m_state);
}

void BluetoothleUART::getDeviceList(QList<QString> &qlDevices)
{
        qlDevices = m_qlFoundDevices;
}

const QBluetoothDeviceInfo *BluetoothleUART::getDeviceByMAC(const QString &macAddress)
{
    for(int i=0; i< m_deviceDiscoveryAgent->discoveredDevices().size(); i++)
    {
        if(m_deviceDiscoveryAgent->discoveredDevices().at(i).address().toString() == macAddress)
            return &m_deviceDiscoveryAgent->discoveredDevices().at(i);
    }
    return nullptr;
}

void BluetoothleUART::setModuleName(QString name)
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

const QString &BluetoothleUART::moduleName() const
{
    return m_moduleName;
}
