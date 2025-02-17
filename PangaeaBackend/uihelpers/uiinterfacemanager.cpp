#include <QQmlApplicationEngine>

#include "uiinterfacemanager.h"

UiInterfaceManager::UiInterfaceManager(QObject *parent)
    : QObject{parent}
{
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "DevicesListModel", &m_devicesListModel);
}

UiInterfaceManager::~UiInterfaceManager()
{
}

void UiInterfaceManager::connectToDevice(DeviceDescription device)
{
    m_connectedInterface = device;
    emit connectedInterfaceChanged();
    emit sgConnectToDevice(device);
}

void UiInterfaceManager::updateDevicesList(DeviceConnectionType connectionType, QList<DeviceDescription> list)
{
    if(connectionType == DeviceConnectionType::BLE)
    {
        m_isBleAvaliable = true;
        emit isBleAvaliableChanged();
    }

    m_devicesListModel.updateInterfaceList(connectionType, list);
}

void UiInterfaceManager::slInterfaceUnavaliable(DeviceConnectionType senderType, QString reason)
{
    if(senderType == DeviceConnectionType::BLE)
    {
        m_isBleAvaliable = false;
        emit isBleAvaliableChanged();
    }

    emit sgInterfaceUnavaliable(senderType, reason);
}

bool UiInterfaceManager::isBleAvaliable()
{
    return m_isBleAvaliable;
}

const QString &UiInterfaceManager::moduleName() const
{
    return m_moduleName;
}

void UiInterfaceManager::setModuleName(const QString &newModuleName)
{
    if (m_moduleName == newModuleName)
        return;
    m_moduleName = newModuleName;
    emit sgModuleNameChanged(newModuleName);
}

qint16 UiInterfaceManager::bleRssi() const
{
    return m_bleRssi;
}

void UiInterfaceManager::setRssi(qint16 newBleRssi)
{
    if (m_bleRssi == newBleRssi)
        return;
    m_bleRssi = newBleRssi;
    emit bleRssiChanged();
}

DeviceDescription UiInterfaceManager::connectedInterface() const
{
    return m_connectedInterface;
}
