#include <QDebug>

#include "presetlistmodel.h"

PresetListModel::PresetListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

QHash<int, QByteArray> PresetListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::PresetNameRole] = "presetName";
    roles[ListRoles::BankNumberRole] = "bankNumber";
    roles[ListRoles::ImpulseNameRole] = "impulseName";
    roles[ListRoles::ImpulseEnabledRole] = "isImpulseEnabled";

    return roles;
}

int PresetListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_presetList->size();
}

QVariant PresetListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
        case ListRoles::PresetNameRole:
        {
            return QVariant::fromValue("Preset " + QString().setNum(m_presetList->at(index.row()).presetNumber()));
        }

        case ListRoles::BankNumberRole:
        {
            return QVariant::fromValue(QString().setNum(m_presetList->at(index.row()).bankNumber()));
        }


        case ListRoles::ImpulseNameRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row()).impulseName());
        }

        case ListRoles::ImpulseEnabledRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row()).isIrEnabled());
        }

        default:
        {
            qWarning() << __FUNCTION__ << "no role";
            return true;
        }
    }
}

void PresetListModel::refreshModel(QList<Preset>* presetList)
{
    beginResetModel();

    m_presetList = presetList;

    endResetModel();
}
