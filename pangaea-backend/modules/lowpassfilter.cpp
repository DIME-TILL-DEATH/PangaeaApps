#include "lowpassfilter.h"

LowPassFilter::LowPassFilter(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::LP, "LP", "lo"}
{
    m_lpf = new ControlValue(this, "lv", "Low-pass", "Hz",
                195, 0, 20000, 1000);
}

void LowPassFilter::setValues(bool enabled, quint8 value)
{
    m_moduleEnabled = enabled;
    m_lpf->setControlValue(value);

    emit dataChanged();
}

void LowPassFilter::setValues(const eq_t &eq)
{
    m_moduleEnabled = eq.lp_on;
    m_lpf->setControlValue(eq.lp_freq);

    emit dataChanged();
}