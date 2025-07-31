#include "delay.h"

#include <QDebug>

Delay::Delay(AbstractDevice *owner, preset_data_cpmodern_t *data)
    : AbstractModule{owner, ModuleType::DELAY, "DL", "dl_on"}
{
    m_fullModuleName = AbstractModule::tr("Delay");

    m_moduleEnabled = (bool*)&data->delay.on;

    m_processingTime = 50;
    m_mixFirst = new ControlValue(this, &data->delay.mix, "dl_mx", "Mix", "", 0, 63, 0, 63);
    m_time = new ControlValue(this, &data->delay.time, "dl_tm", "Time", "ms",  50, 500, 50, 500);
    m_feedback = new ControlValue(this, &data->delay.feedback, "dl_fb", "Feedback", "",  0, 63, 0, 63);
    m_hpf = new ControlValue(this, &data->delay.hpf, "dl_hp", "HPF", "Hz",  0, 127, 20, 1000);
    m_lpf = new ControlValue(this, &data->delay.lpf, "dl_lp", "LPF", "Hz",  0, 127, 1000, 10000);
}

Delay::Delay(AbstractDevice *owner, modules_data_fx_t *data)
    : AbstractModule{owner, ModuleType::DELAY, "DL", "dl_on"}
{
    m_fullModuleName = AbstractModule::tr("Delay");

    m_moduleEnabled = (bool*)&data->switches.delay;

    m_processingTime = 70;
    m_mixFirst = new ControlValue(this, &data->delay.volumeFirst, "dl_m1", "Mix", "", 0, 127, -63, 64);
    m_mixSecond = new ControlValue(this, &data->delay.volumeSecond, "dl_m2", "D2 vol", "", 0, 127, 0, 127);
    m_panFirst = new ControlValue(this, &data->delay.panFirst, "dl_p1", "D1 pan", "", 0, 127, -63, 63);
    m_panSecond = new ControlValue(this, &data->delay.panSecond, "dl_p2", "D2 pan", "", 0, 127, -63, 63);
    m_time = new ControlValue(this, &data->delay_time, "dl_tm", "Time", "ms",  10, 2730, 10, 2730);
    m_feedback = new ControlValue(this, &data->delay.feedback, "dl_fb", "Feedback", "",  0, 127, 0, 127);
    m_hpf = new ControlValue(this, &data->delay.hpf, "dl_hp", "HPF",  "", 0, 127, 0, 127);//"Hz", 0, 127, 20, 1000);
    m_lpf = new ControlValue(this, &data->delay.lpf, "dl_lp", "LPF", "", 0, 127, 0, 127); //"Hz",127, 1000, 10000);

    m_offset = new ControlValue(this, &data->delay.offset, "dl_os", "D->D2", "",  0, 127, 0, 127);
    m_modulation = new ControlValue(this, &data->delay.modulation, "dl_md", "Modulation", "",  0, 127, 0, 127);
    m_rate = new ControlValue(this, &data->delay.rate, "dl_rt", "Rate", "",  0, 127, 0, 127);

    m_direction = new ControlValue(this, &data->delay.direction, "dl_dr", "Direction");
    m_tap = new ControlValue(this, &data->delay_tap, "dl_tp", "TAP");
    m_tail = new ControlValue(this, &data->delay_tail, "dl_tl", "Tail");
}

void Delay::setValues(const preset_data_cpmodern_t &dlData)
{
    *m_moduleEnabled = dlData.delay.on;

    m_mixFirst->setControlValue(dlData.delay.mix);
    m_time->setControlValue(dlData.delay.time);
    m_feedback->setControlValue(dlData.delay.feedback);
    m_hpf->setControlValue(dlData.delay.hpf);
    m_lpf->setControlValue(dlData.delay.lpf);

    emit dataChanged();
}

void Delay::setValues(const modules_data_fx_t& dlData)
{
    *m_moduleEnabled = dlData.switches.delay;

    m_time->setControlValue(dlData.delay_time);
    m_tap->setControlValue(dlData.delay_tap);
    m_tail->setControlValue(dlData.delay_tail);

    m_mixFirst->setControlValue(dlData.delay.volumeFirst);
    m_mixSecond->setControlValue(dlData.delay.volumeSecond);
    m_panFirst->setControlValue(dlData.delay.panFirst);
    m_panSecond->setControlValue(dlData.delay.panSecond);
    m_feedback->setControlValue(dlData.delay.feedback);
    m_hpf->setControlValue(dlData.delay.hpf);
    m_lpf->setControlValue(dlData.delay.lpf);

    m_offset->setControlValue(dlData.delay.offset);
    m_modulation->setControlValue(dlData.delay.modulation);
    m_rate->setControlValue(dlData.delay.rate);
    m_direction->setControlValue(dlData.delay.direction);

    emit dataChanged();
}

