#include "chorus.h"

Chorus::Chorus(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::CH, "CH", "ch_on"}
{
    m_processingTime = 40;

    m_mix = new ControlValue(this, "ch_mx", "Mix", "", 0, 63, 0, 63);
    m_rate = new ControlValue(this, "ch_rt", "Rate", "",  0, 63, 0, 63);
    m_width = new ControlValue(this, "ch_wd", "Width", "",  0, 63, 0, 63);
    m_hpf = new ControlValue(this, "ch_hp", "HPF", "Hz",  0, 255, 20, 2000);
}

void Chorus::setValues(const chorus_t &chData)
{
    m_moduleEnabled = chData.on;

    m_mix->setControlValue(chData.mix);
    m_rate->setControlValue(chData.rate);
    m_width->setControlValue(chData.width);
    m_hpf->setControlValue(chData.hpf);

    emit dataChanged();
}

ControlValue *Chorus::mix() const
{
    return m_mix;
}

ControlValue *Chorus::rate() const
{
    return m_rate;
}

ControlValue *Chorus::width() const
{
    return m_width;
}

ControlValue *Chorus::hpf() const
{
    return m_hpf;
}
