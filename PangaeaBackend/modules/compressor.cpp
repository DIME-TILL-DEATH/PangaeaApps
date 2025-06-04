#include "compressor.h"

Compressor::Compressor(AbstractDevice *owner, CompressorType compressorType)
    : AbstractModule{owner, ModuleType::CM, "CM", "co"}
{
    m_processingTime = 70;
    m_fullModuleName = AbstractModule::tr("Compressor");

    switch(compressorType)
    {
    case CompressorType::FX:
    {
        m_processingTime = 100;

        m_commandOnOff = "cm_on";

        m_threshold = new ControlValue(this, "cm_th", "Threshold", "", 0, 127, 0, 127); //"dB", 0, 127, 0, -37);
        m_ratio = new ControlValue(this, "cm_rt", "Ratio", "", 0, 127, 0, 127); //0, 127, 2, 24);
        m_volume = new ControlValue(this, "cm_vl", "Volume", "", 0, 127, 0, 127);
        m_attack = new ControlValue(this, "cm_at", "Attack", "", 0, 127, 0, 127); //"ms", 0, 127, 0, 30);
        m_knee = new ControlValue(this, "cm_kn", "Knee", "", 0, 127, 0, 127);
        break;
    }
    case CompressorType::Classic:
    {
        m_processingTime = 70;
        m_sustain = new ControlValue(this, "cs", "Sustain");
        m_volume = new ControlValue(this, "cv", "Volume");
        break;
    }
    }
}

void Compressor::setValues(const preset_data_cplegacy_t& presetData)
{
    m_moduleEnabled = presetData.compressor_on;
    m_sustain->setControlValue(presetData.compressor_sustain);
    m_volume->setControlValue(presetData.compressor_volume);

    emit dataChanged();
}

void Compressor::setValues(const preset_data_cpmodern_t &cmData)
{
    m_moduleEnabled = cmData.compressor.on;

    m_sustain->setControlValue(cmData.compressor.sustain);
    m_volume->setControlValue(cmData.compressor.volume);

    emit dataChanged();
}

void Compressor::setValues(const preset_data_fx_t& cmData)
{
    m_moduleEnabled = cmData.switches.compressor;

    m_threshold->setControlValue(cmData.compressor.threshold);
    m_ratio->setControlValue(cmData.compressor.ratio);
    m_volume->setControlValue(cmData.compressor.volume);
    m_attack->setControlValue(cmData.compressor.attack);
    m_knee->setControlValue(cmData.compressor.knee);

    emit dataChanged();
}
