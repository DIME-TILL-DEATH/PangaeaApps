#include "reverb.h"

Reverb::Reverb(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::RV, "RV", "rv_on"}
{
    m_processingTime = 100;
    m_fullModuleName = AbstractModule::tr("Reverb");

    m_mix = new ControlValue(this, "rv_mx", "Mix", "", 0, 127, -63, 64);
    m_type = new ControlValue(this, "rv_tp", "Type");
    m_time = new ControlValue(this, "rv_tm", "Time", "", 0, 127, 0, 127);
    m_size = new ControlValue(this, "rv_sz", "Size", "", 0, 127, 0, 127);
    m_damping = new ControlValue(this, "rv_dp", "Damping", "", 0, 127, 0, 127);
    m_hpf = new ControlValue(this, "rv_hp", "HPF", "", 0, 127, 0, 127); //"Hz", 0, 127, 20, 2000);
    m_lpf = new ControlValue(this, "rv_lp", "LPF", "", 0, 127, 0, 127); //"Hz", 0, 127, 2000, 20000);
    m_detune = new ControlValue(this, "rv_dt", "Detune", "", 0, 127, 0, 127);
    m_diffusion = new ControlValue(this, "rv_df", "Diffusion", "", 0, 127, 0, 127);
    m_preDelay = new ControlValue(this, "rv_pd", "preDelay", "", 0, 127, 0, 127);
    m_tail = new ControlValue(this, "rv_tl", "Tail");
}

void Reverb::setValues(const preset_data_fx_t &rvData)
{
    m_moduleEnabled = rvData.switches.reverb;

    m_mix->setControlValue(rvData.reverb.mix);
    m_type->setControlValue(rvData.reverb_type);
    m_time->setControlValue(rvData.reverb.time);
    m_size->setControlValue(rvData.reverb.size);
    m_damping->setControlValue(rvData.reverb.damping);
    m_lpf->setControlValue(rvData.reverb.lpf);
    m_hpf->setControlValue(rvData.reverb.hpf);
    m_detune->setControlValue(rvData.reverb.detune);
    m_diffusion->setControlValue(rvData.reverb_diffusion);
    m_preDelay->setControlValue(rvData.reverb_predelay);
    m_tail->setControlValue(rvData.reverb_tail);

    emit dataChanged();
}
