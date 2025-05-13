#include "hipassfilter.h"

HiPassFilter::HiPassFilter(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::HP, "HP", "ho"}
{
    m_processingTime = 5;
    m_fullModuleName = AbstractModule::tr("High-pass filter");

    m_hpf = new ControlValue(this, "hv", "Hi-pass", "Hz",
                                0, 255, 20, 1000);
}

void HiPassFilter::setValues(bool enabled, quint8 value)
{
    m_moduleEnabled = enabled;
    m_hpf->setControlValue(value);

    emit dataChanged();
}

void HiPassFilter::setValues(const eq_cpmodern_t &eq)
{
    m_moduleEnabled = eq.hp_on;
    m_hpf->setControlValue(eq.hp_freq);

    emit dataChanged();
}
