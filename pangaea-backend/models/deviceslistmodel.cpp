#include "deviceslistmodel.h"

DevicesListModel::DevicesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int DevicesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_devicesList.size();
}

QHash<int, QByteArray> DevicesListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::DeviceDescriptionRole] = "deviceDescription";

    return roles;
}

QVariant DevicesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
        case ListRoles::DeviceDescriptionRole:
        {
            return QVariant::fromValue(m_devicesList.at(index.row()));
        }

        default:
        {
            qWarning() << __FUNCTION__ << "no role";
            return true;
        }
    }
}

void DevicesListModel::updateInterfaceList(DeviceConnectionType connectionType, QList<DeviceDescription>& list)
{
    appendNewDevices(list);
    removeMissingDevices(connectionType, list);
}

void DevicesListModel::appendNewDevices(QList<DeviceDescription>& list)
{
    for(auto& newDevice : list)
    {
        auto findResult = std::find(m_devicesList.begin(), m_devicesList.end(), newDevice);

        if(findResult == m_devicesList.end())
        {
            qDebug() << "New device found: " << newDevice.name() << newDevice.address();
            appendDevice(newDevice);
        }
    }
}

void DevicesListModel::removeMissingDevices(DeviceConnectionType connectionType, QList<DeviceDescription>& list)
{
    for(auto& checkingDevice : m_devicesList)
    {
        if(checkingDevice.connectionType() != connectionType) continue;

        if(list.isEmpty())
        {
            removeDevice(checkingDevice);
        }

        auto findResult = std::find(list.begin(), list.end(), checkingDevice);

        if(findResult == list.end())
        {
            qDebug() << "Device doesn't avaliable anymore, remove: " << checkingDevice.name() << checkingDevice.address();
            removeDevice(checkingDevice);
        }
    }
}

void DevicesListModel::appendDevice(DeviceDescription &device)
{
    beginInsertRows(QModelIndex(), m_devicesList.size(), m_devicesList.size());
    m_devicesList.append(device);
    endInsertRows();
}

void DevicesListModel::removeDevice(DeviceDescription &device)
{
    qint16 index = m_devicesList.indexOf(device);

    if(index == -1) return;

    beginRemoveRows(QModelIndex(), index, index);
    m_devicesList.removeAll(device);
    endRemoveRows();
}
