#include "tremolo.h"

Tremolo::Tremolo(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::TR, "TR", "tr_on"}
{
    m_processingTime = 20;
    m_fullModuleName = AbstractModule::tr("Tremolo");

    m_depth = new ControlValue(this, "tr_dp", "Depth", "", 0, 127, 0, 127);
    m_rate = new ControlValue(this, "tr_rt", "Rate", "",  0, 127, 0, 127);  // 0,048-14,5 Hz
    m_form = new ControlValue(this, "tr_fm", "Form", "", 0, 127, 0, 127);

    m_lfoMod = new ControlValue(this, "tr_lm", "LFO mod.", "", 0, 127, 0, 127);

    m_ms = new ControlValue(this, "tr_ms", "Mono-St.");
    m_tap = new ControlValue(this, "tr_tp", "TAP");
}

ControlValue *Tremolo::depth() const
{
    return m_depth;
}

ControlValue *Tremolo::rate() const
{
    return m_rate;
}

ControlValue *Tremolo::form() const
{
    return m_form;
}

ControlValue *Tremolo::lfoMod() const
{
    return m_lfoMod;
}

ControlValue *Tremolo::ms() const
{
    return m_ms;
}

ControlValue *Tremolo::tap() const
{
    return m_tap;
}

void Tremolo::setValues(const preset_data_cpmodern_t &trData)
{
    m_moduleEnabled = trData.tremolo.on;

    m_depth->setControlValue(trData.tremolo.depth);
    m_rate->setControlValue(trData.tremolo.rate);
    m_form->setControlValue(trData.tremolo.type);

    emit dataChanged();
}

void Tremolo::setValues(const modules_data_fx_t& trData)
{
    m_moduleEnabled = trData.switches.tremolo;

    m_depth->setControlValue(trData.tremolo.depth);
    m_rate->setControlValue(trData.tremolo.rate);
    m_lfoMod->setControlValue(trData.tremolo.lfo);
    m_ms->setControlValue(trData.tremolo.ms);

    m_form->setControlValue(trData.tremolo_lfo_type);
    m_tap->setControlValue(trData.tremolo_tap);

    emit dataChanged();
}
