#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>
#include <QDebug>

#include "moduletypeenum.h"
#include "hardwareclassicpreset.h"
#include "hardwarefxpreset.h"

#include <QtQml/qqmlregistration.h>

class AbstractDevice;

class AbstractModule : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(bool moduleEnabled READ moduleEnabled WRITE setModuleEnabled NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString moduleName READ moduleName CONSTANT)
    Q_PROPERTY(QString fullModuleName READ fullModuleName CONSTANT)
    Q_PROPERTY(ModuleType moduleType READ moduleType CONSTANT)

    Q_PROPERTY(bool used READ used WRITE setUsed NOTIFY usedChanged)
    Q_PROPERTY(quint16 processingTime READ processingTime CONSTANT)
public:

    explicit AbstractModule(AbstractDevice* owner, ModuleType moduleType, QString name, QString commandOnOff);
    // ~AbstractModule() {};

    static void registerTypestoQml();

    bool used() const {return m_used;};
    void setUsed(bool newValue);

    virtual quint16 processingTime() const {return m_processingTime;};

    bool moduleEnabled();
    virtual void setModuleEnabled(bool newEnabled);

    QString moduleName() const {return m_moduleName;};
    QString fullModuleName() const {return m_fullModuleName;};

    ModuleType moduleType() const {return m_moduleType;};

    virtual void sendDataToDevice(QByteArray data);

    virtual void setValues(const preset_data_cplegacy_t &prData) {};
    virtual void setValues(const preset_data_cpmodern_t &prData) {};
    virtual void setValues(const modules_data_fx_t &prData) {};

signals:
    void dataChanged();

    void positionChanged();

    void usedChanged();
    void userModifiedModuleParameters();

protected:
    AbstractDevice* m_owner;
    ModuleType m_moduleType;

    bool m_used{false};
    quint16 m_processingTime{0};

    bool* m_moduleEnabled{nullptr};
    QString m_moduleName;
    QString m_fullModuleName;

    QString m_commandOnOff;
};

#endif // ABSTRACTMODULE_H
