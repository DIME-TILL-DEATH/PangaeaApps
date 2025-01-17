#include "delay.h"

#include <QDebug>

Delay::Delay(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::DELAY, "DL", "dl_on"}
{
    m_processingTime = 45;

    m_mix = new ControlValue(this, "dl_mx", "Mix", "", 0, 63, 0, 63);
    m_time = new ControlValue(this, "dl_tm", "Time", "ms",  50, 500, 50, 500);
    m_feedback = new ControlValue(this, "dl_fb", "Feedback", "",  0, 63, 0, 127);
    m_hpf = new ControlValue(this, "dl_hp", "HPF", "Hz",  0, 127, 20, 1000);
    m_lpf = new ControlValue(this, "dl_lp", "LPF", "Hz",  0, 127, 1000, 10000);
}

void Delay::setValues(const delay_t &dlData)
{
    m_moduleEnabled = dlData.on;

    m_mix->setControlValue(dlData.mix);
    m_time->setControlValue(dlData.time);
    m_feedback->setControlValue(dlData.feedback);
    m_hpf->setControlValue(dlData.hpf);
    m_lpf->setControlValue(dlData.lpf);

    emit dataChanged();
}

ControlValue *Delay::mix() const
{
    return m_mix;
}

ControlValue *Delay::time() const
{
    return m_time;
}

ControlValue *Delay::feedback() const
{
    return m_feedback;
}

ControlValue *Delay::hpf() const
{
    return m_hpf;
}

ControlValue *Delay::lpf() const
{
    return m_lpf;
}
