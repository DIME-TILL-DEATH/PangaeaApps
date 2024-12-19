#include "compressor.h"

Compressor::Compressor(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::CM, "CM", "co"}
{
    m_processingTime = 70;

    m_sustain = new ControlValue(this, "cs", "Sustain");
    m_volume = new ControlValue(this, "cv", "Volume");
}

void Compressor::setValues(bool enabled, quint8 sustain, quint8 volume)
{
    m_moduleEnabled = enabled;
    m_sustain->setControlValue(sustain);
    m_volume->setControlValue(volume);

    emit dataChanged();
}

void Compressor::setValues(const compressor_data_t &cmData)
{
    m_moduleEnabled = cmData.on;
    m_sustain->setControlValue(cmData.sustain);
    m_volume->setControlValue(cmData.volume);

    emit dataChanged();
}
