#include "volume.h"

Volume::Volume(AbstractDevice *owner, VolumeType volumeType, void *pointer)
    : AbstractModule{owner, ModuleType::MASTER, "MASTER", ""}
{
    switch (volumeType)
    {
    case VolumeType::PresetClassic: m_volume = new ControlValue(this, pointer, "mv", "Preset volume");
        break;
    case VolumeType::PresetFx: m_volume = new ControlValue(this, pointer, "vl_pr", "Preset volume", "", 0, 127, 0, 127);
        break;
    case VolumeType::MasterFx: m_volume = new ControlValue(this, nullptr, "vl_ms", "Master", "", 0, 127, 0, 127);
        break;
    case VolumeType::PhonesFx: m_volume = new ControlValue(this, nullptr, "vl_ph", "Phones", "", 0, 127, 0, 127);
        break;
    case VolumeType::AttenuatorFx:
    {
        m_source = new ControlValue(this, nullptr, "vl_at_mode", "Att. src");
        m_volume = new ControlValue(this, nullptr, "vl_at", "Attenuator", "dB", 0, 127, 0, 127);
        break;
    }
    }
}

void Volume::setValue(quint8 val, quint8 src)
{
    m_volume->setControlValue(val);
    if(m_source)
    {
        m_source->setControlValue(src);
    }

    emit dataChanged();
}
