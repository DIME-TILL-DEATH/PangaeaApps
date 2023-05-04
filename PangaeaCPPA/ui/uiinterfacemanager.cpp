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

void UiInterfaceManager::startScanning()
{
    emit sgStartScanning();
}

void UiInterfaceManager::connectToDevice(DeviceDescription device)
{
    emit sgConnectToDevice(device);
}

void UiInterfaceManager::disconnectFromDevice()
{
    emit sgDisconnectFromDevice();
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
