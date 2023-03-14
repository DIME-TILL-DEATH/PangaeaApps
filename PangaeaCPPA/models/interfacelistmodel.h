#ifndef INTERFACELISTMODEL_H
#define INTERFACELISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "devicedescription.h"

class InterfaceListModel : public QAbstractListModel
{
public:
    explicit InterfaceListModel(QObject *parent = nullptr);

public:
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
        DeviceDescriptionRole = Qt::UserRole + 1,
    };
};

#endif // INTERFACELISTMODEL_H
