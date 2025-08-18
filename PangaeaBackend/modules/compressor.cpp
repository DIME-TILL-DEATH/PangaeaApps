#include "compressor.h"

Compressor::Compressor(AbstractDevice *owner, preset_data_cplegacy_t *cmData)
    : AbstractModule{owner, ModuleType::CM, "CM", "co"}
{
    m_fullModuleName = AbstractModule::tr("Compressor");

    m_sustain = new ControlValue(this, &cmData->compressor_sustain, "cs", "Sustain");
    m_volume = new ControlValue(this, &cmData->compressor_volume,"cv", "Volume");

    m_moduleEnabled = (bool*)&cmData->compressor_on;
}

Compressor::Compressor(AbstractDevice *owner, preset_data_cpmodern_t *cmData)
    : AbstractModule{owner, ModuleType::CM, "CM", "co"}
{
    m_processingTime = 70;
    m_fullModuleName = AbstractModule::tr("Compressor");

    m_sustain = new ControlValue(this, &cmData->compressor.sustain, "cs", "Sustain");
    m_volume = new ControlValue(this, &cmData->compressor.volume, "cv", "Volume");

    m_moduleEnabled = (bool*)&cmData->compressor.on;
}

Compressor::Compressor(AbstractDevice *owner, modules_data_fx_t *cmData)
    : AbstractModule{owner, ModuleType::CM, "CM", "cm_on"}
{
    m_processingTime = 70;
    m_fullModuleName = AbstractModule::tr("Compressor");

    m_threshold = new ControlValue(this, &cmData->compressor.threshold, "cm_th", "Threshold", "", 0, 127, 0, 127); //"dB", 0, 127, 0, -37);
    m_ratio = new ControlValue(this, &cmData->compressor.ratio, "cm_rt", "Ratio", "", 0, 127, 0, 127); //0, 127, 2, 24);
    m_volume = new ControlValue(this, &cmData->compressor.volume, "cm_vl", "Volume", "", 0, 127, 0, 127);
    m_attack = new ControlValue(this, &cmData->compressor.attack, "cm_at", "Attack", "", 0, 127, 0, 127); //"ms", 0, 127, 0, 30);
    m_knee = new ControlValue(this, &cmData->compressor.knee, "cm_rl", "Release", "", 0, 127, 0, 127);

    m_moduleEnabled = (bool*)&cmData->switches.compressor;
}

void Compressor::setValues(const preset_data_cplegacy_t& cmData)
{
    *m_moduleEnabled = cmData.compressor_on;
    m_sustain->setControlValue(cmData.compressor_sustain);
    m_volume->setControlValue(cmData.compressor_volume);

    emit dataChanged();
}

void Compressor::setValues(const preset_data_cpmodern_t &cmData)
{
    *m_moduleEnabled = cmData.compressor.on;

    m_sustain->setControlValue(cmData.compressor.sustain);
    m_volume->setControlValue(cmData.compressor.volume);

    emit dataChanged();
}

void Compressor::setValues(const modules_data_fx_t& cmData)
{
    *m_moduleEnabled = cmData.switches.compressor;

    m_threshold->setControlValue(cmData.compressor.threshold);
    m_ratio->setControlValue(cmData.compressor.ratio);
    m_volume->setControlValue(cmData.compressor.volume);
    m_attack->setControlValue(cmData.compressor.attack);
    m_knee->setControlValue(cmData.compressor.knee);

    emit dataChanged();
}
