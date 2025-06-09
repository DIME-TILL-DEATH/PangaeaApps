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

void PowerAmp::setValues(const preset_data_cplegacy_t &paData)
{
    m_moduleEnabled = paData.amp_on;
    m_volume->setControlValue(paData.amp_volume);
    m_presence->setControlValue(paData.presence_vol);
    m_slave->setControlValue(paData.amp_slave);
    m_ampType->setControlValue(paData.amp_type);

    emit dataChanged();
}

void PowerAmp::setValues(const preset_data_cpmodern_t& paData)
{
    m_moduleEnabled = paData.power_amp.on;
    m_volume->setControlValue(paData.power_amp.volume);
    m_presence->setControlValue(paData.power_amp.presence_vol);
    m_slave->setControlValue(paData.power_amp.slave);

    m_ampType->setControlValue(paData.power_amp.type);

    emit dataChanged();
}

void PowerAmp::setValues(const preset_data_fx_t& paData)
{
    m_moduleEnabled = paData.switches.amp;

    m_volume->setControlValue(paData.pa.master);
    m_slave->setControlValue(paData.pa.level);
    m_ampType->setControlValue(paData.pa.type);

    m_presence->setControlValue(paData.presence);

    emit dataChanged();
}

void PowerAmp::slPaEnabledChanged()
{
    // emit sgSetDeviceParameter(DeviceParameter::Type::PRESENCE_ON, m_moduleEnabled);
}


