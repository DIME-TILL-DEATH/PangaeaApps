#include "flanger.h"

Flanger::Flanger(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::FL, "FL", "fl_on"}
{
    m_processingTime = 50;
    m_fullModuleName = AbstractModule::tr("Flanger");

    m_mix = new ControlValue(this, "fl_mx", "Mix", "", 0, 127, -63, 64);
    m_lfo = new ControlValue(this, "fl_lf", "LFO");
    m_rate = new ControlValue(this, "fl_rt", "Rate", "",  0, 127, 0, 127);// "Hz",  0, 127, 0.024, 9.6); // Range: 0 (0.024 Hz) to 127 (9.6 Hz).
    m_width = new ControlValue(this, "fl_wd", "Width", "",  0, 127, 0, 127);
    m_delay = new ControlValue(this, "fl_dl", "Delay", "",  0, 127, 0, 127);//"ms",  0, 127, 0, 21);
    m_feedback = new ControlValue(this, "fl_fb", "Feedback", "",  0, 127, 0, 127);// "",  0, 127, 0, 21);
    m_hpf = new ControlValue(this, "fl_hp", "HPF",  "",  0, 127, 0, 127);//  "Hz", 0, 127, 20, 2000);
    m_position = new ControlValue(this, "fl_pp", "Position");
}

void Flanger::setValues(const preset_data_fx_t& flData)
{
    m_moduleEnabled = flData.switches.flanger;

    m_mix->setControlValue(flData.flanger.mix);
    m_lfo->setControlValue(flData.flanger.lfo);
    m_rate->setControlValue(flData.flanger.rate);
    m_width->setControlValue(flData.flanger.width);
    m_feedback->setControlValue(flData.flanger.feedback);
    m_delay->setControlValue(flData.flanger.delay);

    m_hpf->setControlValue(flData.hpf_flanger);

    m_position->setControlValue(flData.flanger_pre_post);

    emit dataChanged();
}
