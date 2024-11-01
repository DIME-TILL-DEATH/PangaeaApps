#ifndef PRESETLISTMODEL_H
#define PRESETLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "preset.h"

class PresetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PresetListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void refreshModel(QList<Preset>* presetList);

    void updatePreset(const Preset &newPreset);

private:
    QList<Preset>* m_presetList;

    enum ListRoles{
        PresetNameRole = Qt::UserRole + 1,
        BankNumberRole,
        PresetNumberRole,
        PresetDeviceIndexRole,
        PresetMapIndexRole,
        ImpulseEnabledRole,
        ImpulseNameRole,
        isImpulseEmptyRole
    };
};

#endif // PRESETLISTMODEL_H
