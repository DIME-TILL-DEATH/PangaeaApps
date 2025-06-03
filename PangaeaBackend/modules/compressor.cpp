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

void Compressor::setValues(bool enabled, quint8 sustain, quint8 volume)
{
    m_moduleEnabled = enabled;
    m_sustain->setControlValue(sustain);
    m_volume->setControlValue(volume);

    emit dataChanged();
}

void Compressor::setValues(const compressor_cpmodern_t &cmData)
{
    m_moduleEnabled = cmData.on;

    m_sustain->setControlValue(cmData.sustain);
    m_volume->setControlValue(cmData.volume);

    emit dataChanged();
}

void Compressor::setValues(uint8_t enabled, const compressor_fx_t &cmData)
{
    m_moduleEnabled = enabled;

    m_threshold->setControlValue(cmData.threshold);
    m_ratio->setControlValue(cmData.ratio);
    m_volume->setControlValue(cmData.volume);
    m_attack->setControlValue(cmData.attack);
    m_knee->setControlValue(cmData.knee);

    emit dataChanged();
}
