#include "moduleslistmodel.h"

ModulesListModel::ModulesListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    // TODO small memory leak
    m_moduleList = new QList<AbstractModule*>; // dummy
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

void ModulesListModel::appendModule(AbstractModule *newModule)
{
    int pos = 0;
    if(!m_moduleList->isEmpty()) pos = m_moduleList->size()-1;

    insertModule(newModule, pos);
}

void ModulesListModel::insertModule(AbstractModule *newModule, int position)
{
    if(newModule == nullptr) return;

    beginInsertRows(QModelIndex(), position, position);
    newModule->setUsed(true);

    if(m_moduleList->isEmpty()) m_moduleList->append(newModule);
    else m_moduleList->insert(position, 1, newModule);

    endInsertRows();
    emit sgModulesReconfigured();
}

void ModulesListModel::removeModule(int position)
{
    if(position >= m_moduleList->size())
    {
        qWarning() << Q_FUNC_INFO << "Position is more than size:" << position << m_moduleList->size();
        return;
    }

    beginRemoveRows(QModelIndex(), position, position);
    qInfo() << position << m_moduleList->size();
    m_moduleList->at(position)->setUsed(false);
    m_moduleList->removeAt(position);
    qInfo() << m_moduleList->size();
    endRemoveRows();
    emit sgModulesReconfigured();
}

void ModulesListModel::removeModuleByType(ModuleType moduleType)
{
    for(int i = 0; i < m_moduleList->count(); i++)
    {
        if(m_moduleList->at(i)->moduleType() == moduleType)
        {
            removeModule(i);
            break;
        }
    }
}

void ModulesListModel::moveModule(int from, int to)
{
    if(from == to) return;

    qint32 indexToInModel = to;
    if(to > from) indexToInModel++;

    beginMoveRows(QModelIndex(), from, from, QModelIndex(), indexToInModel);
    m_moduleList->move(from, to);
    endMoveRows();
    emit sgModulesReconfigured();
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
        if(module) return QVariant::fromValue(module);
        else return{};
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
