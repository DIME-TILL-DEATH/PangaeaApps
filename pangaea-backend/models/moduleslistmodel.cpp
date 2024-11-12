#include "moduleslistmodel.h"

ModulesListModel::ModulesListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ModulesListModel::~ModulesListModel()
{

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
    m_moduleList->insert(position, 1, newModule);
    endInsertRows();
}

void ModulesListModel::moveModule(int from, int to)
{
    qint32 indexToInModel = to;
    if(to > from) indexToInModel++;
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), indexToInModel);
    m_moduleList->move(from, to);
    endMoveRows();
}

int ModulesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(m_moduleList) return m_moduleList->size();
    else return 0;
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
        return QVariant::fromValue(m_moduleList->at(index.row())->moduleType());
    }

    case ListRoles::ModuleInstanceRole:
    {
        AbstractModule* module = m_moduleList->at(index.row());
        return QVariant::fromValue(module);
    }

    default:
    {
        qWarning() << __FUNCTION__ << "no role";
        return true;
    }
    }
}


void ModulesListModel::refreshModel(QList<AbstractModule *> *newModulesList)
{
    beginResetModel();

    m_moduleList = newModulesList;

    endResetModel();
}
