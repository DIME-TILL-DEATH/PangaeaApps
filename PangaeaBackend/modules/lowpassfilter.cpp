#include "lowpassfilter.h"

#include <QDebug>

LowPassFilter::LowPassFilter(AbstractDevice *owner, preset_data_cplegacy_t *lpData)
    : AbstractModule{owner, ModuleType::LP, "LP", "lo"}
{
    m_processingTime = 5;
    m_fullModuleName = AbstractModule::tr("Low pass filter");

    m_lpf = new ControlValue(this, &lpData->lp_freq, "lv", "Low-pass", "Hz",
                             // 195, 0, 20000, 1000);
                             195, 0, 1000, 20000);
    
    m_moduleEnabled = (bool*)&lpData->lp_on;
}

void LowPassFilter::setValues(const preset_data_cplegacy_t& lpData)
{
    *m_moduleEnabled = lpData.lp_on;
    m_lpf->setControlValue(lpData.lp_freq);

    emit dataChanged();
}
