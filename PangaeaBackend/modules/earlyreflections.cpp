#include "earlyreflections.h"

#include "abstractdevice.h"


EarlyReflections::EarlyReflections(AbstractDevice *owner, preset_data_cplegacy_t *erData)
    : AbstractModule{owner, ModuleType::ER_MONO, "ER", "eo"}
{
    m_processingTime = 80; //mono 50
    m_fullModuleName = AbstractModule::tr("Early reflections");

    m_reflectionsVolume = new ControlValue(this, &erData->early_volume, "ev", "Volume");
    m_reflectionsType = new ControlValue(this, &erData->early_type, "et", "ER Type");

    m_moduleEnabled = (bool*)&erData->early_on;
}

EarlyReflections::EarlyReflections(AbstractDevice *owner, preset_data_cpmodern_t *erData)
    : AbstractModule{owner, ModuleType::ER_MONO, "ER", "eo"}
{
    m_processingTime = 80; //mono 50
    m_fullModuleName = AbstractModule::tr("Early reflections");

    m_reflectionsVolume = new ControlValue(this, &erData->reverb.volume, "ev", "Volume");
    m_reflectionsType = new ControlValue(this, &erData->reverb.type, "et", "ER Type");

    m_moduleEnabled = (bool*)&erData->reverb.on;
}

EarlyReflections::EarlyReflections(AbstractDevice *owner, modules_data_fx_t *erData)
    : AbstractModule{owner, ModuleType::ER_MONO, "ER", "er_on"}
{
    m_processingTime = 80; //mono 50
    m_fullModuleName = AbstractModule::tr("Early reflections");

    m_reflectionsVolume = new ControlValue(this, &erData->early_reflections.volume, "er_mx", "Mix", "",  0, 127, -63, 64);
    m_reflectionsType = new ControlValue(this, &erData->early_reflections.type, "er_sz", "Size", "",  0, 127, 0, 127);

    m_moduleEnabled = (bool*)&erData->switches.early_reflections;
}

void EarlyReflections::setValues(const preset_data_cplegacy_t &erData)
{
    *m_moduleEnabled = erData.early_on;
    m_reflectionsVolume->setControlValue(erData.early_volume);
    m_reflectionsType->setControlValue(erData.early_type);

    emit dataChanged();
}

void EarlyReflections::setValues(const preset_data_cpmodern_t &erData)
{
    *m_moduleEnabled = erData.reverb.on;
    m_reflectionsVolume->setControlValue(erData.reverb.volume);
    m_reflectionsType->setControlValue(erData.reverb.type);

    emit dataChanged();
}

void EarlyReflections::setValues(const modules_data_fx_t& erData)
{
    *m_moduleEnabled = erData.switches.early_reflections;

    m_reflectionsVolume->setControlValue(erData.early_reflections.volume);
    m_reflectionsType->setControlValue(erData.early_reflections.type);

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
