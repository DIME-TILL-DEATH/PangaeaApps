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

    Q_INVOKABLE void appendModule(AbstractModule* newModule);
    Q_INVOKABLE void insertModule(AbstractModule* newModule, int position);
    Q_INVOKABLE void removeModule(int position);
    Q_INVOKABLE void moveModule(int from, int to);

    void refreshModel(QList<AbstractModule *> *newModulesList);
signals:
    void sgModulesReconfigured();

private:
    QList<AbstractModule*>* m_moduleList;

    enum ListRoles{
        ModuleTypeRole = Qt::UserRole + 1,
        ModuleInstanceRole
    };
};

#endif // MODULESLISTMODEL_H
