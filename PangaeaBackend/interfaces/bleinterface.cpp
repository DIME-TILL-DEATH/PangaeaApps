#include <QMetaEnum>
#include <QStandardPaths>
#include <QPermission>

#include <QCoreApplication>
#include <QGeoPositionInfoSource>

#include <QThread>


#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#include "androidutils.h"
#endif

#include "bleinterface.h"

BleInterface::BleInterface(QObject *parent)
    : AbstractInterface{parent},
    m_control{nullptr},
    m_service{nullptr}
{
    QCoreApplication *app = QCoreApplication::instance();
    if(app)
    {
       app->requestPermission(QBluetoothPermission{}, [](const QPermission &permission)
       {
           if(permission.status() == Qt::PermissionStatus::Granted){
               qDebug() << "Bluetooth permission granted";
           }
           else{
               qWarning() << "Bluetooth permission not granted!";
           }
       });
    }

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);

        appSettings->setFallbacksEnabled(false);
        appSettings->beginGroup("AutoconnectSettings");
//        m_autoconnectDevicetMAC = appSettings->value("MAC address", 0).toString();
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
#elif defined(Q_OS_IOS)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.plist", QSettings::NativeFormat);

    appSettings->setFallbacksEnabled(false);
    appSettings->beginGroup("AutoconnectSettings");
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
#else
    appSettings = new QSettings(QSettings::UserScope, this);
#endif

    qDebug() << "BLE thread" << thread();
    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(); // parent = this: Main COM uninit tried from another thread
    qDebug() << "Discovery agent thread" << m_deviceDiscoveryAgent->thread();

    QBluetoothDeviceDiscoveryAgent::connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BleInterface::addDevice);

    QBluetoothDeviceDiscoveryAgent::connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &BleInterface::deviceScanError);

    QBluetoothDeviceDiscoveryAgent::connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BleInterface::scanTimeout);


    QObject::connect(this, &QObject::destroyed, m_deviceDiscoveryAgent, &QObject::deleteLater);

    m_description = "BLE";
    m_connectionType = DeviceConnectionType::BLE;

    rssiUpdateTimer = new QTimer(this);
    qDebug() << "rssi Timer thread" << rssiUpdateTimer->thread();
    rssiUpdateTimer->setInterval(1000);
    QObject::connect(rssiUpdateTimer, &QTimer::timeout, this, &BleInterface::requestRssi);
}

BleInterface::~BleInterface()
{

}

void BleInterface::startScan()
{
    QCoreApplication *app = QCoreApplication::instance();
    if(app){
       if(app->checkPermission(QBluetoothPermission{}) != Qt::PermissionStatus::Granted){
           qWarning() << "Bluetooth permission not granted!";
           return;
       }
    }
    
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
#ifndef Q_OS_IOS
    QBluetoothLocalDevice device;

    if(!device.isValid())
    {
        qInfo() << "Bluetooth is not valid";
        emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "Device is unavaliable");
        prevState();
        return;
    }

    QGeoPositionInfoSource* geoSource =  QGeoPositionInfoSource::createDefaultSource(this);
    if(geoSource == nullptr)
    {
        qWarning() << "Geo backend unavaliable!";
        return;
    }
    else
    {
        geoSource->requestUpdate(0);

        if(geoSource->error() == QGeoPositionInfoSource::ClosedError)
        {
            qWarning() << "Geolocation is off!";
            emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "GeolocationIsOff");
            delete(geoSource);
            return;
        }
    }

    if(device.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
        prevState();
        isAvaliable = false;

        m_qlFoundDevices.clear();
        emit sgDeviceListUpdated(DeviceConnectionType::BLE, m_qlFoundDevices);
        emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "HostPoweredOff");
        device.powerOn();
        return;
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
    QString address;

#ifdef Q_OS_MAC
    address = device.deviceUuid().toString();
#else
    address = device.address().toString();
#endif

    if(device.name().indexOf("AMT PANGAEA") == 0 )
    {
        if(!m_avaliableDevices.contains(device))
        {
            qInfo() << "Finded BLE Device name: " << device.name() << " Address: " << address;
            m_avaliableDevices.append(device);
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
      QString strAddress;
#ifdef Q_OS_MAC
      strAddress = currentDevice.deviceUuid().toString();
#else
     strAddress = currentDevice.address().toString();
#endif

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
        const QBluetoothDeviceInfo* dev_ptr = getDeviceByAddress(macAddress);

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
            break;
        }
        case QBluetoothDeviceDiscoveryAgent::UnknownError:
        {
            qDebug() << __FUNCTION__ << "An unknown error has occurred.";
            emit sgInterfaceUnavaliable(DeviceConnectionType::BLE, "UnknownBleError");
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
        m_connectedDevice = device;
        setState(InterfaceState::Connecting);

        slStartConnect(device.address());
        // emit sgConnectionStarted();

        return true;
}

void BleInterface::slStartConnect(QString address)
{
    qDebug() << "Trying to connect, address: " << address;

    const QBluetoothDeviceInfo* deviceToConnect = getDeviceByAddress(address);

    if(deviceToConnect == nullptr)
    {
        qDebug() << "Device is unavalibale";
        emit sgDeviceUnavaliable(DeviceConnectionType::BLE, "nullPointer");
        prevState();
        startScan();
        return;
    }

    emit sgConnectionStarted();

#ifdef Q_OS_MAC
    m_currentDeviceAddress = deviceToConnect->deviceUuid().toString();
#else
    m_currentDeviceAddress = deviceToConnect->address().toString();
#endif

    appSettings->setFallbacksEnabled(false);
    appSettings->beginGroup("AutoconnectSettings");
    appSettings->setValue("BLE_address", m_currentDeviceAddress);
    appSettings->endGroup();
    appSettings->sync();

    m_moduleName = m_moduleUniqueNames.value(m_currentDeviceAddress, "");

    setState(InterfaceState::UpdateModuleName);
    emit sgModuleNameUpdated(m_moduleName);

    if (m_control)
    {
        m_control->disconnectFromDevice();
        disconnect(m_control, &QLowEnergyController::rssiRead, this, &BleInterface::rssiReaded);
        disconnect(m_control, &QLowEnergyController::connectionUpdated, this, &BleInterface::connectionParametersUpdated);
        delete m_control;
        m_control = 0;
    }

    m_control = QLowEnergyController::createCentral(*deviceToConnect, this);
    m_control->setRemoteAddressType(QLowEnergyController::RandomAddress);

    qDebug() << "BLE thread" << thread();
    qDebug() << "control thread" << m_control->thread();

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

void BleInterface::deviceConnected()
{
    QLowEnergyConnectionParameters connectionParams;
    connectionParams.setIntervalRange(20, 75);  //10, 25
    m_control->requestConnectionUpdate(connectionParams);
    QObject::connect(m_control, &QLowEnergyController::connectionUpdated, this, &BleInterface::connectionParametersUpdated);

#if defined(Q_OS_ANDROID) | defined(Q_OS_IOS) | defined(Q_OS_MACOS)
    m_control->readRssi();
    QObject::connect(m_control, &QLowEnergyController::rssiRead, this, &BleInterface::rssiReaded);
#endif

    dataToWrite.clear();

    setState(Connected);
    m_control->discoverServices();
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
    QBluetoothDeviceDiscoveryAgent::connect(m_service, &QLowEnergyService::errorOccurred,
            this, &BleInterface::serviceError);
    QBluetoothDeviceDiscoveryAgent::connect(m_service, &QLowEnergyService::characteristicWritten,
            this, &BleInterface::characteristicWritten);

    m_service->discoverDetails();
    setState(ServiceFound);
}


void BleInterface::deviceDisconnected()
{
    qWarning() << "Remote device disconnected(UART service disconnected)";

    rssiUpdateTimer->stop();
    emit sgInterfaceDisconnected(m_connectedDevice);
    emit sgInterfaceError("Remote device disconnected");
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

void BleInterface::connectionParametersUpdated(const QLowEnergyConnectionParameters &newParameters)
{
    qInfo() << "Connection parameters updated, intervals:"
            << newParameters.minimumInterval()
            << newParameters.maximumInterval()
            << " supervision timeout:" << newParameters.supervisionTimeout()
            << " latency" << newParameters.latency();
}

void BleInterface::write(const QByteArray &data)
{
#if defined(Q_OS_IOS)
    if(dataToWrite.empty()){
        const QLowEnergyCharacteristic rxChar = m_service->characteristic(QBluetoothUuid(QUuid(RXUUID)));
        m_service->writeCharacteristic(rxChar, data, QLowEnergyService::WriteWithResponse);
    }

    dataToWrite.enqueue(data);

    if(data.size() >= 100) QThread::msleep(150);
#else
    const QLowEnergyCharacteristic rxChar = m_service->characteristic(QBluetoothUuid(QUuid(RXUUID)));
    m_service->writeCharacteristic(rxChar, data, QLowEnergyService::WriteWithResponse);
#endif
}

void BleInterface::characteristicWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue)
{
#if defined(Q_OS_IOS)
    if(!dataToWrite.empty()) dataToWrite.dequeue();

    if(!dataToWrite.empty()){
        // qInfo() << "dataToWrite not empty" << dataToWrite.size();
        const QLowEnergyCharacteristic rxChar = m_service->characteristic(QBluetoothUuid(QUuid(RXUUID)));
        m_service->writeCharacteristic(rxChar, dataToWrite.head(), QLowEnergyService::WriteWithResponse);
    }
#endif
}

void BleInterface::serviceError(QLowEnergyService::ServiceError newError)
{
    qWarning() << "UART BLE service error: " << newError;
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

    emit sgInterfaceDisconnected(m_connectedDevice);
    setState(InterfaceState::Idle);
    m_avaliableDevices.clear();
}

const QBluetoothDeviceInfo *BleInterface::getDeviceByAddress(const QString &address)
{
    for(int i=0; i< m_deviceDiscoveryAgent->discoveredDevices().size(); i++)
    {
        QString devAddress;
#ifdef Q_OS_MAC
        devAddress = m_deviceDiscoveryAgent->discoveredDevices().at(i).deviceUuid().toString();
#else
        devAddress = m_deviceDiscoveryAgent->discoveredDevices().at(i).address().toString();
#endif
        if(devAddress == address)
            return &m_deviceDiscoveryAgent->discoveredDevices().at(i);
    }
    return nullptr;
}

void BleInterface::setModuleName(QString name)
{
    qInfo() << "Settling unique module name:" << name << " to MAC:" << m_currentDeviceAddress;
    if(name != m_moduleName)
    {
        m_moduleName = name;

        m_moduleUniqueNames.insert(m_currentDeviceAddress, m_moduleName);

#if  defined(Q_OS_ANDROID)
        QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                           + "/settings.conf", QSettings::NativeFormat);
#elif defined(Q_OS_IOS)
        QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                        + "/settings.plist", QSettings::NativeFormat);
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

void BleInterface::rssiMeasuring(bool isEnabled)
{
    if(isEnabled) rssiUpdateTimer->start();
    else rssiUpdateTimer->stop();
}

void BleInterface::requestRssi()
{
#if defined(Q_OS_ANDROID) | defined(Q_OS_IOS) | defined(Q_OS_MACOS)
    if(m_control)
        m_control->readRssi();
#endif
}
