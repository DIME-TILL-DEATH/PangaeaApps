#include "chorus.h"

Chorus::Chorus(AbstractDevice *owner, ChorusType chorusType)
    : AbstractModule{owner, ModuleType::CH, "CH", "ch_on"}
{
    m_fullModuleName = AbstractModule::tr("Chorus");

    switch(chorusType)
    {
    case ChorusType::FX:
    {
        m_processingTime = 50;

        m_mix = new ControlValue(this, "ch_mx", "Mix", "", 0, 127, -63, 64);
        m_rate = new ControlValue(this, "ch_rt", "Rate", "", 0, 127, 0, 127);//"Hz",  0, 127, 0.024, 9.6);
        m_width = new ControlValue(this, "ch_wd", "Width", "", 0, 127, 0, 127); //, "ms",  0, 127, 0, 21);
        m_delay = new ControlValue(this, "ch_dl", "Delay", "", 0, 127, 0, 127);//, "ms",  0, 127, 0, 21);
        m_type = new ControlValue(this, "ch_tp", "Type");
        m_hpf = new ControlValue(this, "ch_hp", "HPF", "", 0, 127, 0, 127);//"Hz",  0, 127, 20, 2000);
        break;
    }
    case ChorusType::Classic:
    {
        m_processingTime = 40;

        m_mix = new ControlValue(this, "ch_mx", "Mix", "", 0, 63, 0, 63);
        m_rate = new ControlValue(this, "ch_rt", "Rate", "",  0, 63, 0, 63);
        m_width = new ControlValue(this, "ch_wd", "Width", "",  0, 63, 0, 63);
        m_hpf = new ControlValue(this, "ch_hp", "HPF", "Hz",  0, 255, 20, 2000);
        break;
    }
    }
}

void Chorus::setValues(const preset_data_cpmodern_t &chData)
{
    m_moduleEnabled = chData.chorus.on;

    m_mix->setControlValue(chData.chorus.mix);
    m_rate->setControlValue(chData.chorus.rate);
    m_width->setControlValue(chData.chorus.width);
    m_hpf->setControlValue(chData.chorus.hpf);

    emit dataChanged();
}

void Chorus::setValues(const preset_data_fx_t& chData)
{
    m_moduleEnabled = chData.switches.chorus;

    m_mix->setControlValue(chData.chorus.mix);
    m_rate->setControlValue(chData.chorus.rate);
    m_width->setControlValue(chData.chorus.width);

    m_delay->setControlValue(chData.chorus.delay);
    m_type->setControlValue(chData.chorus.type);

    m_hpf->setControlValue(chData.hpf_chorus);

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

ControlValue *Chorus::delay() const
{
    return m_delay;
}

ControlValue *Chorus::type() const
{
    return m_type;
}
