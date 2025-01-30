#ifndef DEVICESLISTMODEL_H
#define DEVICESLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include "devicedescription.h"

class DevicesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DevicesListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void updateInterfaceList(DeviceConnectionType connectionType, QList<DeviceDescription> &list);

private:
    QList<DeviceDescription> m_devicesList;

    void appendNewDevices(QList<DeviceDescription> &list);
    void removeMissingDevices(DeviceConnectionType connectionType, QList<DeviceDescription>& list);

    void appendDevice(DeviceDescription& device);
    void removeDevice(DeviceDescription& device);

    enum ListRoles{
        DeviceDescriptionRole = Qt::UserRole + 1
    };
};

#endif // DEVICESLISTMODEL_H
