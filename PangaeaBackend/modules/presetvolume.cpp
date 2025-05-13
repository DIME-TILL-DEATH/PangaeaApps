#include "presetvolume.h"

PresetVolume::PresetVolume(AbstractDevice *owner, VolumeType volumeType)
    : AbstractModule{owner, ModuleType::MASTER, "MASTER", ""}
{
    m_moduleEnabled = true;

    switch (volumeType)
    {
    case VolumeType::Classic: m_master = new ControlValue(this, "mv", "Preset volume");
        break;
    case VolumeType::FX: m_master = new ControlValue(this, "mv", "Preset volume", 0, 127, 0, 127);
        break;
    }

}

void PresetVolume::setValue(quint8 val)
{
    m_master->setControlValue(val);
    emit dataChanged();
}
