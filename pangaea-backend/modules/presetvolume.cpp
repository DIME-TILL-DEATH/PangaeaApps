#include "presetvolume.h"

PresetVolume::PresetVolume(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::MASTER, "MASTER", ""}
{
    m_moduleEnabled = true;
    m_master = new ControlValue(this, "mv", "Preset volume");
}

void PresetVolume::setValue(quint8 val)
{
    m_master->setControlValue(val);
    emit dataChanged();
}
