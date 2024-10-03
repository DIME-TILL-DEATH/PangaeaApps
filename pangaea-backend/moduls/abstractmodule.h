#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>

#include "core.h"

class AbstractModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool moduleEnabled READ moduleEnabled WRITE setModuleEnabled NOTIFY moduleEnabledChanged FINAL)
    Q_PROPERTY(QString moduleName READ moduleName CONSTANT)
public:
    explicit AbstractModule(Core* core, QString name, DeviceParameter::Type parameterTypeModuleOn, QObject *parent = nullptr);

    bool moduleEnabled() const;
    void setModuleEnabled(bool newEnabled);

    QString moduleName() const;

signals:
    void sgSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetAppParameter(Core::AppParameter appParameterType, QVariant content);

    void moduleEnabledChanged();

public slots:
    void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void slSetAppParameter(Core::AppParameter appParameterType, QVariant content);

protected:
    bool m_moduleEnabled;
    QString m_moduleName;
    DeviceParameter::Type m_parameterTypeModuleOn;
};

#endif // ABSTRACTMODULE_H
