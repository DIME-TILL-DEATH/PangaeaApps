#ifndef PRESETLISTMODEL_H
#define PRESETLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "preset.h"

#if defined(PANGAEA_LIBRARY)
#  define PANGAEA_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define PANGAEA_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

class PANGAEA_LIBRARY_EXPORT PresetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PresetListModel(QObject *parent = nullptr);


public:
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
