#include "phaser.h"

Phaser::Phaser(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::PH, "PH", "ph_on"}
{
    m_processingTime = 50;

    m_mix = new ControlValue(this, "ph_mx", "Mix", "", 0, 63, 0, 63);
    m_rate = new ControlValue(this, "ph_rt", "Rate", "",  0, 127, 0, 127);
    m_width = new ControlValue(this, "ph_wd", "Width", "",  0, 127, 0, 127);
    m_center = new ControlValue(this, "ph_cn", "Center", "",  0, 127, 0, 127);
    m_feedback = new ControlValue(this, "ph_fb", "Feedback", "",  0, 127, 0, 127);
    m_stages = new ControlValue(this, "ph_st", "Stages", "",  0, 3, 0, 3);
    m_hpf = new ControlValue(this, "ph_hp", "HPF", "Hz",  0, 255, 20, 2000);
}

void Phaser::setValues(const phaser_t &phData)
{
    m_moduleEnabled = phData.on;

    m_mix->setControlValue(phData.mix);
    m_rate->setControlValue(phData.rate);
    m_center->setControlValue(phData.center);
    m_width->setControlValue(phData.width);
    m_feedback->setControlValue(phData.feedback);
    m_stages->setControlValue(phData.stages);
    m_hpf->setControlValue(phData.hpf);
    emit dataChanged();
}

ControlValue *Phaser::mix() const
{
    return m_mix;
}

ControlValue *Phaser::rate() const
{
    return m_rate;
}

ControlValue *Phaser::center() const
{
    return m_center;
}

ControlValue *Phaser::width() const
{
    return m_width;
}

ControlValue *Phaser::feedback() const
{
    return m_feedback;
}

ControlValue *Phaser::stages() const
{
    return m_stages;
}

ControlValue *Phaser::hpf() const
{
    return m_hpf;
}
