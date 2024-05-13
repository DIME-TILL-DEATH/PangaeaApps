#include "abstractmodule.h"

AbstractModule::AbstractModule(DeviceParameter::Type parameterTypeModuleOn, QObject *parent)
    : QObject{parent},
    m_parameterTypeModuleOn{parameterTypeModuleOn}
{}

bool AbstractModule::enabled() const
{
    return m_enabled;
}

void AbstractModule::setEnabled(bool newEnabled)
{
    if (m_enabled == newEnabled)
        return;
    m_enabled = newEnabled;
    emit enabledChanged();
}

void AbstractModule::slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value)
{
    if(deviceParameterType == m_parameterTypeModuleOn)
    {
        setEnabled(value);
    }
    emit sgSetUiDeviceParameter(deviceParameterType, value);
}
