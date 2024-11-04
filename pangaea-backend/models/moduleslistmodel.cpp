#include "moduleslistmodel.h"

#include "poweramp.h"

#include <QtQuick/QQuickView>

ModulesListModel::ModulesListModel(QObject *parent)
    : QAbstractListModel{parent}
{
     this->moveToThread(QGuiApplication::instance()->thread());
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
        case ModuleTypeEnum::BYPASS:
        case ModuleTypeEnum::CM:
        case ModuleTypeEnum::PR:
        case ModuleTypeEnum::IR:
        case ModuleTypeEnum::HP:
        case ModuleType::PA:
        {
            PowerAmp* paModule = dynamic_cast<PowerAmp*>(module);
            if(paModule)return QVariant::fromValue(paModule);
        }
        case ModuleTypeEnum::EQ: break;
        case ModuleTypeEnum::LP:
        case ModuleTypeEnum::NG:
            break;
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
