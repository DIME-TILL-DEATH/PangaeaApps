#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>

#include "moduletypeenum.h"

class AbstractDevice;

class AbstractModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool moduleEnabled READ moduleEnabled WRITE setModuleEnabled NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString moduleName READ moduleName CONSTANT)
public:

    explicit AbstractModule(AbstractDevice* owner, ModuleType moduleType, QString name, QString commandOnOff);

    static void registerTypestoQml();

    bool moduleEnabled() const {return m_moduleEnabled;};
    virtual void setModuleEnabled(bool newEnabled);

    QString moduleName() const {return m_moduleName;};
    ModuleType moduleType() const {return m_moduleType;};

    void sendDataToDevice(const QByteArray& data);

signals:
    void dataChanged();

    void userModifiedModuleParameters();

protected:
    AbstractDevice* m_owner;
    ModuleType m_moduleType;

    bool m_moduleEnabled;
    QString m_moduleName;

    QString m_commandOnOff;

};

#endif // ABSTRACTMODULE_H
