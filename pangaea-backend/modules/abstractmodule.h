#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>

#include "moduletypeenum.h"
#include "hardwarepreset.h"


class AbstractDevice;

class AbstractModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool moduleEnabled READ moduleEnabled WRITE setModuleEnabled NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString moduleName READ moduleName CONSTANT)

    Q_PROPERTY(bool assignable READ assignable CONSTANT)
    Q_PROPERTY(quint16 processingTime READ processingTime CONSTANT)
public:

    explicit AbstractModule(AbstractDevice* owner, ModuleType moduleType, QString name, QString commandOnOff);

    static void registerTypestoQml();

    bool assignable() const {return m_assignable;};
    quint16 processingTime() const {return m_processingTime;};

    bool moduleEnabled() const {return m_moduleEnabled;};
    virtual void setModuleEnabled(bool newEnabled);

    QString moduleName() const {return m_moduleName;};
    ModuleType moduleType() const {return m_moduleType;};

    virtual void sendDataToDevice(const QByteArray& data);

signals:
    void dataChanged();

    void userModifiedModuleParameters();

protected:
    AbstractDevice* m_owner;
    ModuleType m_moduleType;

    bool m_assignable{true};
    quint16 m_processingTime{0};

    bool m_moduleEnabled;
    QString m_moduleName;

    QString m_commandOnOff;

};

#endif // ABSTRACTMODULE_H
