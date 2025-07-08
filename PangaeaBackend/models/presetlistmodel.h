#ifndef PRESETLISTMODEL_H
#define PRESETLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "presetabstract.h"

class PresetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PresetListModel(QObject *parent = nullptr);
    ~PresetListModel();;

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void refreshModel(QList<PresetAbstract*>* presetList);

    void updatePreset(PresetAbstract *newPreset);

private:
    QList<PresetAbstract*>* m_presetList;

    enum ListRoles{
        PresetNameRole = Qt::UserRole + 1,
        PresetCommentRole,
        BankNumberRole,
        PresetNumberRole,
        PresetDeviceIndexRole,
        PresetMapIndexRole,
        ImpulseEnabledRole,
        ImpulseNameRole,
        isImpulseEmptyRole,
        Impulse1NameRole,
        Impulse2nameRole,
        ActiveModulesRole
    };
};

#endif // PRESETLISTMODEL_H
