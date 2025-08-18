#include "resonancefilter.h"

ResonanceFilter::ResonanceFilter(AbstractDevice *owner, modules_data_fx_t *rfData)
    : AbstractModule{owner, ModuleType::RF, "RF", "rf_on"}
{
    m_fullModuleName = AbstractModule::tr("Resonance filter");

    m_mix = new ControlValue(this, &rfData->resonance_filter.mix, "rf_mx", "Mix", "", 0, 127, -63, 64); // TODO new knob type
    m_fType = new ControlValue(this, &rfData->resonance_filter.f_type, "rf_ft", "Filter");
    m_fMod = new ControlValue(this, &rfData->resonance_filter.f_mod, "rf_fm", "F mod");
    m_lfo = new ControlValue(this, &rfData->resonance_filter.lfo_rate, "rf_lr", "LFO rate", "", 0, 127, 0, 127);
    m_loFreq = new ControlValue(this, &rfData->resonance_filter.lpf, "rf_lp", "Lo freq", "", 0, 127, 0, 127); //"Hz", 0, 127, 26, 560);
    m_hiFreq = new ControlValue(this, &rfData->resonance_filter.hpf, "rf_hp", "Hi freq", "", 0, 127, 0, 127); //"Hz", 0, 127, 560, 6545);
    m_res = new ControlValue(this, &rfData->resonance_filter.resonance, "rf_rs", "Resonance", "Hz", 0, 127, 0, 127);
    m_dynTh = new ControlValue(this, &rfData->resonance_filter.dyn_threshold, "rf_dt", "Dyn thresh", "", 0, 127, 0, 127);
    m_dynAtt = new ControlValue(this, &rfData->resonance_filter.dyn_attack, "rf_da", "Dyn att.", "", 0, 127, 0, 127);
    m_dynRel = new ControlValue(this, &rfData->resonance_filter.dyn_release, "rf_dr", "Dyn rel.", "", 0, 127, 0, 127);
    m_volume = new ControlValue(this, &rfData->resonance_filter.volume, "rf_vl", "Volume", "", 0, 127, 0, 127);

    m_lfoType = new ControlValue(this, &rfData->resonance_filter_gen_type, "rf_lt", "LFO type");

    m_moduleEnabled = (bool*)&rfData->switches.resonance_filter;
}

void ResonanceFilter::setValues(const modules_data_fx_t& rfData)
{
    *m_moduleEnabled = rfData.switches.resonance_filter;

    m_mix->setControlValue(rfData.resonance_filter.mix);
    m_fType->setControlValue(rfData.resonance_filter.f_type);
    m_fMod->setControlValue(rfData.resonance_filter.f_mod);
    m_lfo->setControlValue(rfData.resonance_filter.lfo_rate);
    m_loFreq->setControlValue(rfData.resonance_filter.lpf);
    m_hiFreq->setControlValue(rfData.resonance_filter.hpf);
    m_res->setControlValue(rfData.resonance_filter.resonance);
    m_dynTh->setControlValue(rfData.resonance_filter.dyn_threshold);
    m_dynAtt->setControlValue(rfData.resonance_filter.dyn_attack);
    m_dynRel->setControlValue(rfData.resonance_filter.dyn_release);
    m_volume->setControlValue(rfData.resonance_filter.volume);

    m_lfoType->setControlValue(rfData.resonance_filter_gen_type);

    emit dataChanged();
}
