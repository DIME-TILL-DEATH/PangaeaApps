#include "chorus.h"

Chorus::Chorus(AbstractDevice *owner, preset_data_cpmodern_t *chData)
    : AbstractModule{owner, ModuleType::CH, "CH", "ch_on"}
{
    m_fullModuleName = AbstractModule::tr("Chorus");

    m_processingTime = 40;

    m_mix = new ControlValue(this, &chData->chorus.mix, "ch_mx", "Mix", "", 0, 63, 0, 63);
    m_rate = new ControlValue(this, &chData->chorus.mix, "ch_rt", "Rate", "",  0, 63, 0, 63);
    m_width = new ControlValue(this, &chData->chorus.mix, "ch_wd", "Width", "",  0, 63, 0, 63);
    m_hpf = new ControlValue(this, &chData->chorus.mix, "ch_hp", "HPF", "Hz",  0, 255, 20, 2000);

    m_moduleEnabled = (bool*)&chData->chorus.on;
}

Chorus::Chorus(AbstractDevice *owner, modules_data_fx_t *chData)
    : AbstractModule{owner, ModuleType::CH, "CH", "ch_on"}
{
    m_fullModuleName = AbstractModule::tr("Chorus");

    m_processingTime = 50;

    m_mix = new ControlValue(this, &chData->chorus.mix, "ch_mx", "Mix", "", 0, 127, -63, 64);
    m_rate = new ControlValue(this, &chData->chorus.rate, "ch_rt", "Rate", "", 0, 127, 0, 127);//"Hz",  0, 127, 0.024, 9.6);
    m_width = new ControlValue(this, &chData->chorus.width, "ch_wd", "Width", "", 0, 127, 0, 127); //, "ms",  0, 127, 0, 21);
    m_delay = new ControlValue(this, &chData->chorus.delay, "ch_dl", "Delay", "", 0, 127, 0, 127);//, "ms",  0, 127, 0, 21);
    m_type = new ControlValue(this, &chData->chorus.type, "ch_tp", "Type");
    m_hpf = new ControlValue(this, &chData->hpf_chorus, "ch_hp", "HPF", "", 0, 127, 0, 127);//"Hz",  0, 127, 20, 2000);

    m_moduleEnabled = (bool*)&chData->switches.chorus;
}

void Chorus::setValues(const preset_data_cpmodern_t &chData)
{
    *m_moduleEnabled = chData.chorus.on;

    m_mix->setControlValue(chData.chorus.mix);
    m_rate->setControlValue(chData.chorus.rate);
    m_width->setControlValue(chData.chorus.width);
    m_hpf->setControlValue(chData.chorus.hpf);

    emit dataChanged();
}

void Chorus::setValues(const modules_data_fx_t& chData)
{
    *m_moduleEnabled = chData.switches.chorus;

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
