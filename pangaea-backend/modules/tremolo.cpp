#include "tremolo.h"

Tremolo::Tremolo(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::TR, "TR", "tr_on"}
{
    m_processingTime = 20;
    m_fullModuleName = AbstractModule::tr("Tremolo");

    m_depth = new ControlValue(this, "tr_dp", "Depth", "", 0, 127, 0, 127);
    m_rate = new ControlValue(this, "tr_rt", "Rate", "",  0, 127, 0, 127);
    m_form = new ControlValue(this, "tr_fm", "Form");
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

void Tremolo::setValues(const tremolo_t &trData)
{
    m_moduleEnabled = trData.on;
    m_depth->setControlValue(trData.depth);
    m_rate->setControlValue(trData.rate);
    m_form->setControlValue(trData.type);

    emit dataChanged();
}
