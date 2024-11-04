#include "poweramp.h"

PowerAmp::PowerAmp(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::PA, "PA", "ao"}
{
    m_volume = new ControlValue(this, "av", "Master"); //VOLUME
    m_presence = new ControlValue(this, "pv", "Presence");
    m_slave = new ControlValue(this, "as", "Level"); //SLAVE

    m_ampType = new ControlValue(this, "at", "Amp type");

    connect(this, &AbstractModule::dataChanged, this, &PowerAmp::slPaEnabledChanged);
}

void PowerAmp::setValues(bool enabled, quint8 volume, quint8 presence, quint8 slave, quint8 ampType)
{
    m_moduleEnabled = enabled;
    m_volume->setControlValue(volume);
    m_presence->setControlValue(presence);
    m_slave->setControlValue(slave);

    m_ampType->setControlValue(ampType);

    emit dataChanged();
}

void PowerAmp::setValues(const pa_data_t &paData)
{
    m_moduleEnabled = paData.on;
    m_volume->setControlValue(paData.volume);
    m_presence->setControlValue(paData.presence_vol);
    m_slave->setControlValue(paData.slave);

    m_ampType->setControlValue(paData.type);

    emit dataChanged();
}

void PowerAmp::slPaEnabledChanged()
{
    // emit sgSetDeviceParameter(DeviceParameter::Type::PRESENCE_ON, m_moduleEnabled);
}


