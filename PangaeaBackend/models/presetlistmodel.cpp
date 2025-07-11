#include <QDebug>

#include "presetlistmodel.h"
#include "presetfx.h"

PresetListModel::PresetListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    // TODO small memory leak
    m_presetList = new QList<PresetAbstract*>; // dummy
}

PresetListModel::~PresetListModel()
{
}

QHash<int, QByteArray> PresetListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::PresetNameRole] = "presetName";
    roles[ListRoles::PresetCommentRole] = "presetComment";
    roles[ListRoles::BankNumberRole] = "bankNumber";
    roles[ListRoles::PresetNumberRole] = "presetNumber";
    roles[ListRoles::PresetDeviceIndexRole] = "presetDeviceIndex";
    roles[ListRoles::PresetMapIndexRole] = "presetMapIndex";
    roles[ListRoles::ImpulseNameRole] = "impulseName";
    roles[ListRoles::isImpulseEmptyRole] = "isImpulseEmpty";
    roles[ListRoles::ImpulseEnabledRole] = "isImpulseEnabled";
    roles[ListRoles::ActiveModulesRole] = "activeModules";

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
            return QVariant::fromValue(m_presetList->at(index.row())->presetName());
        }

        case ListRoles::PresetCommentRole:
        {
            PresetFx* presetFx = dynamic_cast<PresetFx*>(m_presetList->at(index.row()));

            if(presetFx) return QVariant::fromValue(presetFx->presetComment());
            else return "";
        }

        case ListRoles::BankNumberRole:
        {
            return QVariant::fromValue(QString().setNum(m_presetList->at(index.row())->bankNumber()));
        }

        case ListRoles::PresetNumberRole:
        {
            return QVariant::fromValue(QString().setNum(m_presetList->at(index.row())->presetNumber()));
        }

        case ListRoles::PresetMapIndexRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row())->getPresetFlatNumber());
        }

        case ListRoles::ImpulseNameRole:
        {
            QString impulseName = m_presetList->at(index.row())->irName();
            return QVariant::fromValue(impulseName);
        }

        case ListRoles::isImpulseEmptyRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row())->irName() == "");
        }

        case ListRoles::ImpulseEnabledRole:
        {
            return QVariant::fromValue(m_presetList->at(index.row())->isIrEnabled());
        }

        case ListRoles::ActiveModulesRole:
        {
            PresetFx* presetFx = dynamic_cast<PresetFx*>(m_presetList->at(index.row()));

            if(presetFx) return presetFx->strActiveModules();
            else return "";
        }

        default:
        {
            qWarning() << __FUNCTION__ << "no role";
            return true;
        }
    }
}

void PresetListModel::refreshModel(QList<PresetAbstract*> *presetList)
{
    beginResetModel();

    m_presetList = presetList;

    endResetModel();
}

void PresetListModel::updatePreset(PresetAbstract* newPreset)
{
    quint8 index = newPreset->getPresetFlatNumber();

    if(m_presetList->size() <= index)
    {
        return;
    }

    PresetAbstract** listData = m_presetList->data();
    (*listData[index]) = *newPreset;

    emit dataChanged(createIndex(0, 0), createIndex(m_presetList->size(), 0));
}
