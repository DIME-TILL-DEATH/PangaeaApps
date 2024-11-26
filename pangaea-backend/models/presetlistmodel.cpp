#include <QDebug>

#include "presetlistmodel.h"

PresetListModel::PresetListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_presetList = new QList<Preset>; // dummy
}

QHash<int, QByteArray> PresetListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::PresetNameRole] = "presetName";
    roles[ListRoles::BankNumberRole] = "bankNumber";
    roles[ListRoles::PresetNumberRole] = "presetNumber";
    roles[ListRoles::PresetDeviceIndexRole] = "presetDeviceIndex";
    roles[ListRoles::PresetMapIndexRole] = "presetMapIndex";
    roles[ListRoles::ImpulseNameRole] = "impulseName";
    roles[ListRoles::isImpulseEmptyRole] = "isImpulseEmpty";
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
            // return QVariant::fromValue("Preset " + QString().setNum(m_presetList->at(index.row()).presetNumber()) + ": " + m_presetList->at(index.row()).presetName());
            return QVariant::fromValue(m_presetList->at(index.row()).presetName());
        }

        case ListRoles::BankNumberRole:
        {
            return QVariant::fromValue(QString().setNum(m_presetList->at(index.row()).bankNumber()));
        }

        case ListRoles::PresetNumberRole:
        {
            return QVariant::fromValue(QString().setNum(m_presetList->at(index.row()).presetNumber()));
        }

        // case ListRoles::PresetDeviceIndexRole:
        // {
        //     return QVariant::fromValue(QString().setNum(m_presetList->at(index.row()).getPresetFlatIndex()));
        // }

        case ListRoles::PresetMapIndexRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row()).getPresetFlatNumber());
        }

        case ListRoles::ImpulseNameRole:
        {
            QString impulseName = m_presetList->at(index.row()).impulseName();
            return QVariant::fromValue(impulseName);
        }

        case ListRoles::isImpulseEmptyRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row()).impulseName() == "");
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

void PresetListModel::updatePreset(const Preset& newPreset)
{
    quint8 index = newPreset.getPresetFlatNumber();

    if(m_presetList->size() <= index)
    {
        return;
    }

    // qDebug() << "Updating preset, b:" << newPreset.bankNumber() << "p: " << newPreset.presetNumber() << "impulse name:" << newPreset.impulseName();
    m_presetList->replace(index, newPreset);

    emit dataChanged(createIndex(0, 0), createIndex(m_presetList->size(), 0));
}
