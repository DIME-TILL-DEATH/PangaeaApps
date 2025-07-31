#include "preamp.h"


Preamp::Preamp(AbstractDevice *owner, preset_data_cplegacy_t* prData)
    : AbstractModule{owner, ModuleType::PR, "PR", "pro"}
{
    m_fullModuleName = AbstractModule::tr("Preamp");

    m_moduleEnabled = (bool*)&prData->preamp_on;

    m_processingTime = 20;
    m_volume = new ControlValue(this, &prData->preamp_volume, "prv", "Volume");

    m_low = new ControlValue(this, &prData->preamp_low, "prl", "Low", "", -64, 63, 0, 127);
    m_mid = new ControlValue(this, &prData->preamp_mid, "prm", "Mid", "", -64, 63, 0, 127);
    m_high = new ControlValue(this, &prData->preamp_high, "prh", "High", "", -64, 63, 0, 127);
}

Preamp::Preamp(AbstractDevice *owner, preset_data_cpmodern_t* prData)
    : AbstractModule{owner, ModuleType::PR, "PR", "pro"}
{
    m_fullModuleName = AbstractModule::tr("Preamp");

    m_moduleEnabled = (bool*)&prData->preamp.on;

    m_processingTime = 20;
    m_volume = new ControlValue(this, &prData->preamp.volume, "prv", "Volume");

    m_low = new ControlValue(this, &prData->preamp.low, "prl", "Low", "", -64, 63, 0, 127);
    m_mid = new ControlValue(this, &prData->preamp.mid, "prm", "Mid", "", -64, 63, 0, 127);
    m_high = new ControlValue(this, &prData->preamp.high, "prh", "High", "", -64, 63, 0, 127);
}

Preamp::Preamp(AbstractDevice *owner, modules_data_fx_t* prData)
    : AbstractModule{owner, ModuleType::PR, "PR", "pro"}
{
    m_fullModuleName = AbstractModule::tr("Preamp");

    m_processingTime = 10;

    m_moduleEnabled = (bool*)&prData->switches.preamp;

    m_commandOnOff = "pr_on";

    m_gain = new ControlValue(this, &prData->preamp.gain, "pr_gn", "Gain", "", 0, 127, 0, 127);
    m_volume = new ControlValue(this, &prData->preamp.volume, "pr_vl", "Volume", "", 0, 127, 0, 127);

    m_low = new ControlValue(this, &prData->preamp.low, "pr_lo", "Low", "", 0, 127, 0, 127);
    m_mid = new ControlValue(this, &prData->preamp.mid, "pr_mi", "Mid", "", 0, 127, 0, 127);
    m_high = new ControlValue(this, &prData->preamp.high, "pr_hi", "High", "", 0, 127, 0, 127);
}

void Preamp::setValues(const preset_data_cplegacy_t &prData)
{
    *m_moduleEnabled = prData.preamp_on;
    m_volume->setControlValue(prData.preamp_volume);

    m_low->setControlValue(static_cast<int8_t>(prData.preamp_low));
    m_mid->setControlValue(static_cast<int8_t>(prData.preamp_mid));
    m_high->setControlValue(static_cast<int8_t>(prData.preamp_high));

    emit dataChanged();
}

void Preamp::setValues(const preset_data_cpmodern_t &preData)
{
    *m_moduleEnabled = preData.preamp.on;

    m_volume->setControlValue(preData.preamp.volume);

    m_low->setControlValue(static_cast<int8_t>(preData.preamp.low));
    m_mid->setControlValue(static_cast<int8_t>(preData.preamp.mid));
    m_high->setControlValue(static_cast<int8_t>(preData.preamp.high));

    emit dataChanged();
}

void Preamp::setValues(const modules_data_fx_t& prData)
{
    *m_moduleEnabled = prData.switches.preamp;

    m_gain->setControlValue(prData.preamp.gain);
    m_volume->setControlValue(prData.preamp.volume);
    m_low->setControlValue(static_cast<int8_t>(prData.preamp.low));
    m_mid->setControlValue(static_cast<int8_t>(prData.preamp.mid));
    m_high->setControlValue(static_cast<int8_t>(prData.preamp.high));

    emit dataChanged();
}
