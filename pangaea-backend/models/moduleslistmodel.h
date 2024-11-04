#ifndef MODULESLISTMODEL_H
#define MODULESLISTMODEL_H

#include <QAbstractListModel>

#include "abstractmodule.h"

class ModulesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ModulesListModel(QObject *parent = nullptr);
    ~ModulesListModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void insertModule(AbstractModule* newModule, int position);

    void refreshModel(const QList<AbstractModule *> &newModulesList);
private:
    QList<AbstractModule*> m_moduleList;

    enum ListRoles{
        ModuleTypeRole = Qt::UserRole + 1,
        ModuleInstanceRole
    };
};

#endif // MODULESLISTMODEL_H
