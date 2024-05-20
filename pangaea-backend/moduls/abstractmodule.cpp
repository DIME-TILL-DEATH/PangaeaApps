#include "abstractmodule.h"

AbstractModule::AbstractModule(Core *core, QString name, DeviceParameter::Type parameterTypeModuleOn, QObject *parent)
    : QObject{parent},
    m_moduleName{name},
    m_parameterTypeModuleOn{parameterTypeModuleOn}
{
    connect(this, &AbstractModule::sgSetDeviceParameter, core, &Core::slSetDeviceParameter);
    connect(core, &Core::sgRecieveDeviceParameter, this, &AbstractModule::slSetUiDeviceParameter);
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

bool AbstractModule::moduleEnabled() const
{
    return m_moduleEnabled;
}

QString AbstractModule::moduleName() const
{
    return m_moduleName;
}
