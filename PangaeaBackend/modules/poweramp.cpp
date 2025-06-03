#include "poweramp.h"

PowerAmp::PowerAmp(AbstractDevice *owner, PaType paType)
    : AbstractModule{owner, ModuleType::PA, "PA", "ao"}
{
    m_fullModuleName = AbstractModule::tr("Power amp");



    switch(paType)
    {
    case PaType::FX:
    {
        m_processingTime = 100;

        m_commandOnOff = "pa_on";

        m_volume = new ControlValue(this, "pa_ms", "Volume", "", 0, 127, 0, 127); //VOLUME-Master
        m_presence = new ControlValue(this, "pa_ps", "Presence", "", 0, 127, 0, 127);
        m_slave = new ControlValue(this, "pa_lv", "Slave", "", 0, 127, 0, 127); //SLAVE-Level
        m_ampType = new ControlValue(this, "pa_tp", "Amp type", "", 0, 127, 0, 127);
        break;
    }
    case PaType::Classic:
    {
        m_processingTime = 90;
        m_volume = new ControlValue(this, "av", "Master"); //VOLUME
        m_presence = new ControlValue(this, "pv", "Presence");
        m_slave = new ControlValue(this, "as", "Level"); //SLAVE
        m_ampType = new ControlValue(this, "at", "Amp type");
        break;
    }
    }

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

void PowerAmp::setValues(const pa_cpmodern_t &paData)
{
    m_moduleEnabled = paData.on;
    m_volume->setControlValue(paData.volume);
    m_presence->setControlValue(paData.presence_vol);
    m_slave->setControlValue(paData.slave);

    m_ampType->setControlValue(paData.type);

    emit dataChanged();
}

void PowerAmp::setValues(uint8_t enabled, const pa_fx_t &paData, uint8_t presence)
{
    m_moduleEnabled = enabled;

    m_volume->setControlValue(paData.master);
    m_slave->setControlValue(paData.level);
    m_ampType->setControlValue(paData.type);

    m_presence->setControlValue(presence);

    emit dataChanged();
}

void PowerAmp::slPaEnabledChanged()
{
    // emit sgSetDeviceParameter(DeviceParameter::Type::PRESENCE_ON, m_moduleEnabled);
}


