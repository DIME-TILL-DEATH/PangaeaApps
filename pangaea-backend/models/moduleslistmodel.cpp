#include "moduleslistmodel.h"

#include "noisegate.h"
#include "poweramp.h"
#include "cabsim.h"
#include "eqparametric.h"

#include <QtQuick/QQuickView>

ModulesListModel::ModulesListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ModulesListModel::~ModulesListModel()
{
    qDebug() << "Delete module list model";
    qDeleteAll(m_moduleList);
}

QHash<int, QByteArray> ModulesListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::ModuleTypeRole] = "moduleType";
    roles[ListRoles::ModuleInstanceRole] = "moduleInstance";

    return roles;
}

void ModulesListModel::insertModule(AbstractModule *newModule, int position)
{
    beginInsertRows(QModelIndex(), position, position);
    m_moduleList.insert(position, 1, newModule);
    endInsertRows();
}

int ModulesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_moduleList.size();
}

QVariant ModulesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
    case ListRoles::ModuleTypeRole:
    {
        return QVariant::fromValue(m_moduleList.at(index.row())->moduleType());
    }

    case ListRoles::ModuleInstanceRole:
    {
        AbstractModule* module = m_moduleList.at(index.row());

        switch(module->moduleType())
        {
        case ModuleTypeEnum::BYPASS: break;
        case ModuleTypeEnum::CM:
        case ModuleTypeEnum::PR:
        case ModuleTypeEnum::IR:
        {
            CabSim* irModule = dynamic_cast<CabSim*>(module);
            if(irModule)return QVariant::fromValue(irModule);
            break;
        }
        case ModuleTypeEnum::HP:
        case ModuleType::PA:
        {
            PowerAmp* paModule = dynamic_cast<PowerAmp*>(module);
            if(paModule)return QVariant::fromValue(paModule);
        }
        case ModuleTypeEnum::EQ:
        {
            EqParametric* eqModule = dynamic_cast<EqParametric*>(module);
            if(eqModule)return QVariant::fromValue(eqModule);
            break;
        }
        case ModuleTypeEnum::LP:
        case ModuleTypeEnum::NG:
        {
            NoiseGate* ngModule = dynamic_cast<NoiseGate*>(module);
            if(ngModule)return QVariant::fromValue(ngModule);
            break;
        }
        case ModuleTypeEnum::MASTER: break;
        }
    }

    default:
    {
        qWarning() << __FUNCTION__ << "no role";
        return true;
    }
    }
}


void ModulesListModel::refreshModel(const QList<AbstractModule *>& newModulesList)
{
    beginResetModel();

    m_moduleList = newModulesList;

    endResetModel();
}
