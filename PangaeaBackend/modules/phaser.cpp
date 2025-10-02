#include "phaser.h"

Phaser::Phaser(AbstractDevice *owner, preset_data_cpmodern_t *phData)
    : AbstractModule{owner, ModuleType::PH, "PH", "ph_on"}
{
    m_processingTime = 50;
    m_fullModuleName = AbstractModule::tr("Phaser");
    
    m_mix = new ControlValue(this, &phData->phaser.mix, "ph_mx", "Mix", "", 0, 63, 0, 63);
    m_rate = new ControlValue(this, &phData->phaser.rate, "ph_rt", "Rate", "",  0, 127, 0, 127);
    m_center = new ControlValue(this, &phData->phaser.center, "ph_cn", "Center", "",  0, 127, 0, 127);
    m_hpf = new ControlValue(this, &phData->phaser.hpf, "ph_hp", "HPF", "Hz",  0, 255, 20, 2000);
    m_width = new ControlValue(this, &phData->phaser.width, "ph_wd", "Width", "",  0, 127, 0, 127);
    m_feedback = new ControlValue(this, &phData->phaser.feedback, "ph_fb", "Feedback", "",  0, 127, 0, 127);
    m_stages = new ControlValue(this, &phData->phaser.stages, "ph_st", "Stages");

    m_moduleEnabled = (bool*)&phData->phaser.on;
}

Phaser::Phaser(AbstractDevice *owner, modules_data_fx_t *phData)
    : AbstractModule{owner, ModuleType::PH, "PH", "ph_on"}
{
    m_processingTime = 50;
    m_fullModuleName = AbstractModule::tr("Phaser");
    
    m_mix = new ControlValue(this, &phData->phaser.mix, "ph_mx", "Mix", "", 0, 127, -63, 64);
    m_rate = new ControlValue(this, &phData->phaser.rate, "ph_rt", "Rate", "",  0, 127, 0, 127); //0.048, 4.85); //Range: 0 (0.048 Hz) to 127 (4.85 Hz).
    m_center = new ControlValue(this, &phData->phaser.center, "ph_cn", "Center", "",  0, 127, 0, 127); // "Hz",  0, 127, 80, 12000); //Range: 0 (80 Hz) to 127 (12 kHz).
    m_hpf = new ControlValue(this, &phData->hpf_phaser, "ph_hp", "HPF", "",  0, 127, 0, 127);
    m_position = new ControlValue(this, &phData->phaser_pre_post, "ph_pp", "Position");
    m_width = new ControlValue(this, &phData->phaser.width, "ph_wd", "Width", "",  0, 127, 0, 127);
    m_feedback = new ControlValue(this, &phData->phaser.feedback, "ph_fb", "Feedback", "",  0, 127, 0, 127);
    m_stages = new ControlValue(this, &phData->phaser.type, "ph_st", "Stages");

    m_moduleEnabled = (bool*)&phData->switches.phaser;

    connect(m_position, &ControlValue::userModifiedValue, this, &AbstractModule::positionChanged);
}

void Phaser::setValues(const preset_data_cpmodern_t &phData)
{
    *m_moduleEnabled = phData.phaser.on;

    m_mix->setControlValue(phData.phaser.mix);
    m_rate->setControlValue(phData.phaser.rate);
    m_center->setControlValue(phData.phaser.center);
    m_width->setControlValue(phData.phaser.width);
    m_feedback->setControlValue(phData.phaser.feedback);
    m_stages->setControlValue(phData.phaser.stages);
    m_hpf->setControlValue(phData.phaser.hpf);

    emit dataChanged();
}

void Phaser::setValues(const modules_data_fx_t& phData)
{
    *m_moduleEnabled = phData.switches.phaser;

    m_mix->setControlValue(phData.phaser.mix);
    m_rate->setControlValue(phData.phaser.rate);
    m_center->setControlValue(phData.phaser.center);
    m_width->setControlValue(phData.phaser.width);
    m_feedback->setControlValue(phData.phaser.feedback);
    m_stages->setControlValue(phData.phaser.type);

    m_hpf->setControlValue(phData.hpf_phaser);

    m_position->setControlValue(phData.phaser_pre_post);

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

ControlValue *Phaser::position() const
{
    return m_position;
}
