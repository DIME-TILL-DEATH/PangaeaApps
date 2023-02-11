#include <QQmlApplicationEngine>

#include "uiinterfacemanager.h"

UiInterfaceManager::UiInterfaceManager(QObject *parent)
    : QObject{parent}
{
    qmlRegisterSingletonInstance("CppObjects", 1, 0, "InterfaceListModel", &m_interfaceListModel);
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

void UiInterfaceManager::updateDevicesList(DeviceConnectionType connectionType, QList<DeviceDescription> list)
{
    m_interfaceListModel.updateInterfaceList(connectionType, list);
}
