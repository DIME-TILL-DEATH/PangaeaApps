#include "presence.h"

Presence::Presence(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::PS, "PS", "po"}
{
    m_presenceVolume = new ControlValue(this, "pv", "Presence");
    m_fullModuleName = AbstractModule::tr("Presence");
}

void Presence::setValues(const preset_data_cplegacy_t& presetData)
{
    m_moduleEnabled = presetData.presence_on;
    m_presenceVolume->setControlValue(presetData.presence_vol);

    emit dataChanged();
}
