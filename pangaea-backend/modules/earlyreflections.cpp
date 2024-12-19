#include "earlyreflections.h"

#include "abstractdevice.h"

EarlyReflections::EarlyReflections(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::ER, "ER", "eo"}
{
    m_processingTime = 75; //mono 45

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

void EarlyReflections::setValues(const reverb_data_t &rvData)
{
    m_moduleEnabled = rvData.on;
    m_reflectionsVolume->setControlValue(rvData.volume);
    m_reflectionsType->setControlValue(rvData.type);

    emit dataChanged();
}
