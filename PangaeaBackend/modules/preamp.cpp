#include "preamp.h"

Preamp::Preamp(AbstractDevice *owner, PreampType preampType)
    : AbstractModule{owner, ModuleType::PR, "PR", "pro"}
{

    m_fullModuleName = AbstractModule::tr("Preamp");

    switch(preampType)
    {
    case PreampType::FX:
    {
        m_processingTime = 10;

        m_commandOnOff = "pr_on";

        m_gain = new ControlValue(this, "pr_gn", "Gain", "", 0, 127, 0, 127);
        m_volume = new ControlValue(this, "pr_vl", "Volume", "", 0, 127, 0, 127);

        m_low = new ControlValue(this, "pr_lo", "Low", "", 0, 127, 0, 127);
        m_mid = new ControlValue(this, "pr_mi", "Mid", "", 0, 127, 0, 127);
        m_high = new ControlValue(this, "pr_hi", "High", "", 0, 127, 0, 127);

        break;
    }
    case PreampType::Classic:
    {
        m_processingTime = 20;
        m_gain = new ControlValue(this, "prg", "Gain");
        m_volume = new ControlValue(this, "prv", "Volume");

        m_low = new ControlValue(this, "prl", "Low", "", -64, 63, 0, 127);
        m_mid = new ControlValue(this, "prm", "Mid", "", -64, 63, 0, 127);
        m_high = new ControlValue(this, "prh", "High", "", -64, 63, 0, 127);

        break;
    }
    }
}

void Preamp::setValues(const preset_data_cplegacy_t &prData)
{
    m_moduleEnabled = prData.preamp_on;
    m_volume->setControlValue(prData.preamp_volume);

    m_low->setControlValue(static_cast<int8_t>(prData.preamp_low));
    m_mid->setControlValue(static_cast<int8_t>(prData.preamp_mid));
    m_high->setControlValue(static_cast<int8_t>(prData.preamp_high));

    emit dataChanged();
}

void Preamp::setValues(const preset_data_cpmodern_t &preData)
{
    m_moduleEnabled = preData.preamp.on;

    m_volume->setControlValue(preData.preamp.volume);

    m_low->setControlValue((qint8)preData.preamp.low);
    m_mid->setControlValue((qint8)preData.preamp.mid);
    m_high->setControlValue((qint8)preData.preamp.high);

    emit dataChanged();
}

void Preamp::setValues(const preset_data_fx_t& prData)
{
    m_moduleEnabled = prData.switches.preamp;

    m_gain->setControlValue(prData.preamp.gain);
    m_volume->setControlValue(prData.preamp.volume);
    m_low->setControlValue((qint8)prData.preamp.low);
    m_mid->setControlValue((qint8)prData.preamp.mid);
    m_high->setControlValue((qint8)prData.preamp.high);

    emit dataChanged();
}
