#include "poweramp.h"

PowerAmp::PowerAmp(Core* core, QObject *parent)
    : AbstractModule{core, "PA", DeviceParameter::Type::AMP_ON, parent}
{
    m_volume = new ControlValue(this, (DeviceParameter::Type)((quint8)DeviceParameter::Type::AMP_VOLUME), "VOLUME");
    m_presence = new ControlValue(this, (DeviceParameter::Type)((quint8)DeviceParameter::Type::PRESENCE_VOLUME), "PRESENCE");
    m_slave = new ControlValue(this, (DeviceParameter::Type)((quint8)DeviceParameter::Type::AMP_SLAVE), "SLAVE");

    connect(this, &AbstractModule::moduleEnabledChanged, this, &PowerAmp::slPaEnabledChanged);
}

void PowerAmp::slPaEnabledChanged()
{
    emit sgSetDeviceParameter(DeviceParameter::Type::PRESENCE_ON, m_moduleEnabled);
}


