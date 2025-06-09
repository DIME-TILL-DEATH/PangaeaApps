#include "hipassfilter.h"

HiPassFilter::HiPassFilter(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::HP, "HP", "ho"}
{
    m_processingTime = 5;
    m_fullModuleName = AbstractModule::tr("High-pass filter");

    m_hpf = new ControlValue(this, "hv", "Hi-pass", "Hz",
                                0, 255, 20, 1000);
}

void HiPassFilter::setValues(const preset_data_cplegacy_t& presetData)
{
    m_moduleEnabled = presetData.hp_on;
    m_hpf->setControlValue(presetData.hp_freq);

    emit dataChanged();
}
