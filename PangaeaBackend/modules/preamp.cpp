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

void Preamp::setValues(bool enabled, quint8 volume, qint8 low, qint8 mid, qint8 high)
{
    m_moduleEnabled = enabled;
    m_volume->setControlValue(volume);

    m_low->setControlValue(low);
    m_mid->setControlValue(mid);
    m_high->setControlValue(high);

    emit dataChanged();
}

void Preamp::setValues(const preamp_cpmodern_t &preData)
{
    m_moduleEnabled = preData.on;

    m_volume->setControlValue(preData.volume);

    m_low->setControlValue((qint8)preData.low);
    m_mid->setControlValue((qint8)preData.mid);
    m_high->setControlValue((qint8)preData.high);

    emit dataChanged();
}

void Preamp::setValues(uint8_t enabled, const preamp_fx_t &preampData)
{
    m_moduleEnabled = enabled;

    m_gain->setControlValue(preampData.gain);
    m_volume->setControlValue(preampData.volume);
    m_low->setControlValue((qint8)preampData.low);
    m_mid->setControlValue((qint8)preampData.mid);
    m_high->setControlValue((qint8)preampData.high);

    emit dataChanged();
}
