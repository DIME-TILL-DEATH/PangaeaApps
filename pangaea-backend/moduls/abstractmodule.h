#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>

#include "core.h"

class AbstractModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool moduleEnabled READ moduleEnabled WRITE setModuleEnabled NOTIFY moduleEnabledChanged FINAL)
public:
    explicit AbstractModule(Core* core, DeviceParameter::Type parameterTypeModuleOn, QObject *parent = nullptr);

    bool moduleEnabled() const;
    void setModuleEnabled(bool newEnabled);

signals:
    void sgSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetAppParameter(Core::AppParameter appParameterType, QVariant content);

    void moduleEnabledChanged();

public slots:
    void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);

private:
    bool m_moduleEnabled;
    DeviceParameter::Type m_parameterTypeModuleOn;
};

#endif // ABSTRACTMODULE_H
