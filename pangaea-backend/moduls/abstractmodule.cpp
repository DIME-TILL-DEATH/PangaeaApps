#include "abstractmodule.h"

AbstractModule::AbstractModule(DeviceParameter::Type parameterTypeModuleOn, QObject *parent)
    : QObject{parent},
    m_parameterTypeModuleOn{parameterTypeModuleOn}
{}

bool AbstractModule::moduleEnabled() const
{
    return m_moduleEnabled;
}

void AbstractModule::setModuleEnabled(bool newEnabled)
{
    if (m_moduleEnabled == newEnabled)
        return;
    m_moduleEnabled = newEnabled;
    emit moduleEnabledChanged();
    emit sgSetDeviceParameter(m_parameterTypeModuleOn, newEnabled);
}

void AbstractModule::slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value)
{
    if(deviceParameterType == m_parameterTypeModuleOn)
    {
        m_moduleEnabled = value;
        emit moduleEnabledChanged();
    }
    emit sgSetUiDeviceParameter(deviceParameterType, value);
}
