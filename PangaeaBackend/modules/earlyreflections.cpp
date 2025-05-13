#include "earlyreflections.h"

#include "abstractdevice.h"

EarlyReflections::EarlyReflections(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::ER_MONO, "ER", "eo"}
{
    m_processingTime = 80; //mono 50
    m_fullModuleName = AbstractModule::tr("Early reflections");

    m_reflectionsVolume = new ControlValue(this, "ev", "Volume");
    m_reflectionsType = new ControlValue(this, "et", "ER Type");
}

void EarlyReflections::setValues(bool enabled, quint8 volume, quint8 type)
{
    m_moduleEnabled = enabled;
    m_reflectionsVolume->setControlValue(volume);
    m_reflectionsType->setControlValue(type);

    emit dataChanged();
}

void EarlyReflections::setValues(const early_cpmodern_t &rvData)
{
    m_moduleEnabled = rvData.on;
    m_reflectionsVolume->setControlValue(rvData.volume);
    m_reflectionsType->setControlValue(rvData.type);

    emit dataChanged();
}

void EarlyReflections::setValues(uint8_t enabled, const early_fx_t &earlyData)
{
    m_moduleEnabled = enabled;

    m_reflectionsVolume->setControlValue(earlyData.volume);
    m_reflectionsType->setControlValue(earlyData.type);

    emit dataChanged();
}

quint16 EarlyReflections::processingTime() const
{
    switch(m_reverbType)
    {
    case ModuleType::ER_MONO: return m_processingTimeMono;
    case ModuleType::ER_STEREO: return m_processingTimeStereo;
    default: return 0;
    }
}

ModuleType EarlyReflections::reverbType() const
{
    return m_reverbType;
}

void EarlyReflections::setReverbType(const ModuleType &newReverbType)
{
    if (m_reverbType == newReverbType)
        return;

    m_used = (newReverbType == ModuleType::ER_MONO) ||  (newReverbType == ModuleType::ER_STEREO);
    emit usedChanged();

    m_reverbType = newReverbType;
    emit reverbTypeChanged();
}
