#include "poweramp.h"

PowerAmp::PowerAmp(AbstractDevice *owner, preset_data_cplegacy_t *paData)
    : AbstractModule{owner, ModuleType::PA, "PA", "ao"}
{
    m_fullModuleName = AbstractModule::tr("Power amp");

    m_processingTime = 90;

    m_volume = new ControlValue(this, &paData->amp_volume, "av", "Master"); //VOLUME
    m_presence = new ControlValue(this, &paData->presence_vol, "pv", "Presence");
    m_slave = new ControlValue(this, &paData->amp_slave, "as", "Level"); //SLAVE
    m_ampType = new ControlValue(this, &paData->amp_type, "at", "Amp type");
    
    connect(this, &AbstractModule::dataChanged, this, &PowerAmp::slPaEnabledChanged);

    m_moduleEnabled = (bool*)&paData->amp_on;
}

PowerAmp::PowerAmp(AbstractDevice *owner, preset_data_cpmodern_t *paData)
    : AbstractModule{owner, ModuleType::PA, "PA", "ao"}
{
    m_fullModuleName = AbstractModule::tr("Power amp");

    m_processingTime = 100;

    m_volume = new ControlValue(this, &paData->power_amp.volume, "av", "Master"); //VOLUME
    m_presence = new ControlValue(this, &paData->power_amp.presence_vol, "pv", "Presence");
    m_slave = new ControlValue(this, &paData->power_amp.slave, "as", "Level"); //SLAVE
    m_ampType = new ControlValue(this, &paData->power_amp.type, "at", "Amp type");

    m_moduleEnabled = (bool*)&paData->power_amp.on;
}

PowerAmp::PowerAmp(AbstractDevice *owner, modules_data_fx_t *paData)
    : AbstractModule{owner, ModuleType::PA, "PA", "ao"}
{
    m_fullModuleName = AbstractModule::tr("Power amp");

    m_processingTime = 100;

    m_commandOnOff = "pa_on";

    m_volume = new ControlValue(this, &paData->pa.master, "pa_ms", "Volume", "", 0, 127, 0, 127); //VOLUME-Master
    m_presence = new ControlValue(this, &paData->presence, "pa_ps", "Presence", "", 0, 127, 0, 127);
    m_slave = new ControlValue(this, &paData->pa.level, "pa_lv", "Slave", "", 0, 127, 0, 127); //SLAVE-Level
    m_ampType = new ControlValue(this, &paData->pa.type, "pa_tp", "Amp type", "", 0, 127, 0, 127);

    m_moduleEnabled = (bool*)&paData->switches.amp;
}

void PowerAmp::setValues(const preset_data_cplegacy_t &paData)
{
    *m_moduleEnabled = paData.amp_on;
    m_volume->setControlValue(paData.amp_volume);
    m_presence->setControlValue(paData.presence_vol);
    m_slave->setControlValue(paData.amp_slave);
    m_ampType->setControlValue(paData.amp_type);

    emit dataChanged();
}

void PowerAmp::setValues(const preset_data_cpmodern_t& paData)
{
    *m_moduleEnabled = paData.power_amp.on;
    m_volume->setControlValue(paData.power_amp.volume);
    m_presence->setControlValue(paData.power_amp.presence_vol);
    m_slave->setControlValue(paData.power_amp.slave);

    m_ampType->setControlValue(paData.power_amp.type);

    emit dataChanged();
}

void PowerAmp::setValues(const modules_data_fx_t& paData)
{
    *m_moduleEnabled = paData.switches.amp;

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


