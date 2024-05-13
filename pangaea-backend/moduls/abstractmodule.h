#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QObject>

#include "core.h"

class AbstractModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
public:
    explicit AbstractModule(DeviceParameter::Type parameterTypeModuleOn, QObject *parent = nullptr);

    bool enabled() const;
    void setEnabled(bool newEnabled);

signals:
    void sgSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);
    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetAppParameter(Core::AppParameter appParameterType, QVariant content);

    void enabledChanged();

public slots:
    void slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);

private:
    bool m_enabled;
    DeviceParameter::Type m_parameterTypeModuleOn;
};

#endif // ABSTRACTMODULE_H
