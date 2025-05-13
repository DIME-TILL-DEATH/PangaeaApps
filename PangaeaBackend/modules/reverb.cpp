#include "reverb.h"

Reverb::Reverb(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::RV, "RV", "rv_on"}
{
    m_processingTime = 100;
    m_fullModuleName = AbstractModule::tr("Reverb");

    m_mix = new ControlValue(this, "rv_mx", "Mix", "", -63, 64, -63, 64);
    m_time = new ControlValue(this, "rv_tm", "Time", "", 0, 127, 0, 127);
    m_size = new ControlValue(this, "rv_sz", "Size", "", 0, 127, 0, 127);
    m_damping = new ControlValue(this, "rv_dp", "Damping", "", 0, 127, 0, 127);
    m_hpf = new ControlValue(this, "rv_hp", "HPF", "Hz", 0, 127, 20, 2000);
    m_lpf = new ControlValue(this, "rv_lp", "LPF", "Hz", 0, 127, 2000, 20000);
    m_detune = new ControlValue(this, "rv_dt", "Detune", "", 0, 127, 0, 127);
}

void Reverb::setValues(uint8_t enabled, const reverb_fx_t &rvData)
{
    m_moduleEnabled = enabled;

    m_mix->setControlValue(rvData.mix);
    m_time->setControlValue(rvData.time);
    m_size->setControlValue(rvData.size);
    m_damping->setControlValue(rvData.damping);
    m_lpf->setControlValue(rvData.lpf);
    m_hpf->setControlValue(rvData.hpf);
    m_detune->setControlValue(rvData.detune);

    emit dataChanged();
}
