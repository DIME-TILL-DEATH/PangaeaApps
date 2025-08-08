#include "filebrowsermodel.h"

FileBrowserModel::FileBrowserModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_fsObjectsList.append({"..", Dir});
    m_fsObjectsList.append({"MESA", Dir});
    m_fsObjectsList.append({"super_impuls.wav", File});

}

int FileBrowserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_fsObjectsList.size();
}

QHash<int, QByteArray> FileBrowserModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::FsObjectNameRole] = "name";
    roles[ListRoles::FsObjectTypeRole] = "type";
    return roles;
}

void FileBrowserModel::updateFsObjectsList(const QList<FsObject> &newList)
{
    beginResetModel();
    m_fsObjectsList = newList;
    endResetModel();
}

QVariant FileBrowserModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
    case ListRoles::FsObjectNameRole:
    {
        return QVariant::fromValue(m_fsObjectsList.at(index.row()).name);
    }

    case ListRoles::FsObjectTypeRole:
    {
        return QVariant::fromValue(m_fsObjectsList.at(index.row()).type);
    }
    default:
    {
        qWarning() << __FUNCTION__ << "no role";
        return true;
    }
    }
}


QString FileBrowserModel::currentFolder() const
{
    return m_currentFolder;
}

void FileBrowserModel::setCurrentFolder(const QString &newCurrentFolder)
{
    if (m_currentFolder == newCurrentFolder)
        return;
    m_currentFolder = newCurrentFolder;
    emit currentFolderChanged();
}
