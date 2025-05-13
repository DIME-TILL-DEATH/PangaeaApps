#include "delay.h"

#include <QDebug>

Delay::Delay(AbstractDevice *owner, DelayType delayType)
    : AbstractModule{owner, ModuleType::DELAY, "DL", "dl_on"}
{
    m_fullModuleName = AbstractModule::tr("Delay");



    switch(delayType)
    {
    case DelayType::FX:
    {
        m_processingTime = 70;
        m_mixFirst = new ControlValue(this, "dl_m1", "Mix", "", -63, 64, -63, 64);
        m_mixSecond = new ControlValue(this, "dl_m2", "D2 vol", "", 0, 127, 0, 127);
        m_panFirst = new ControlValue(this, "dl_p1", "D1 pan", "", -63, 63, -63, 63);
        m_panSecond = new ControlValue(this, "dl_p2", "D2 pan", "", -63, 63, -63, 63);
        m_time = new ControlValue(this, "dl_tm", "Time", "ms",  0, 127, 10, 2730);
        m_feedback = new ControlValue(this, "dl_fb", "Feedback", "",  0, 127, 0, 127);
        m_hpf = new ControlValue(this, "dl_hp", "HPF", "Hz",  0, 127, 20, 1000);
        m_lpf = new ControlValue(this, "dl_lp", "LPF", "Hz",  0, 127, 1000, 10000);

        m_offset = new ControlValue(this, "dl_os", "D->D2", "",  0, 127, 0, 127);
        m_modulation = new ControlValue(this, "dl_md", "Modulation", "",  0, 127, 0, 127);
        m_rate = new ControlValue(this, "dl_rt", "Rate", "",  0, 127, 0, 127);

        m_direction = new ControlValue(this, "dl_dr", "Direction");

        m_tap = new ControlValue(this, "dl_tp", "TAP");
        break;
    }
    case DelayType::Classic:
    {
        m_processingTime = 50;
        m_mixFirst = new ControlValue(this, "dl_mx", "Mix", "", 0, 63, 0, 63);
        m_time = new ControlValue(this, "dl_tm", "Time", "ms",  50, 500, 50, 500);
        m_feedback = new ControlValue(this, "dl_fb", "Feedback", "",  0, 63, 0, 63);
        m_hpf = new ControlValue(this, "dl_hp", "HPF", "Hz",  0, 127, 20, 1000);
        m_lpf = new ControlValue(this, "dl_lp", "LPF", "Hz",  0, 127, 1000, 10000);
        break;
    }
    }
}

void Delay::setValues(const delay_cpmodern_t &dlData)
{
    m_moduleEnabled = dlData.on;

    m_mixFirst->setControlValue(dlData.mix);
    m_time->setControlValue(dlData.time);
    m_feedback->setControlValue(dlData.feedback);
    m_hpf->setControlValue(dlData.hpf);
    m_lpf->setControlValue(dlData.lpf);

    emit dataChanged();
}

void Delay::setValues(uint8_t enabled, const delay_fx_t &dlData, uint16_t time, uint8_t tap, uint8_t tail)
{
    m_moduleEnabled = enabled;

    m_time->setControlValue(time);  //?
    m_tap->setControlValue(tap);

    m_mixFirst->setControlValue(dlData.volumeFirst);
    m_mixSecond->setControlValue(dlData.volumeSecond);
    m_panFirst->setControlValue(dlData.panFirst);
    m_panSecond->setControlValue(dlData.panSecond);
    m_feedback->setControlValue(dlData.feedback);
    m_hpf->setControlValue(dlData.hpf);
    m_lpf->setControlValue(dlData.lpf);

    m_offset->setControlValue(dlData.offset);
    m_modulation->setControlValue(dlData.modulation);
    m_rate->setControlValue(dlData.rate);
    m_direction->setControlValue(dlData.direction);

    emit dataChanged();
}

