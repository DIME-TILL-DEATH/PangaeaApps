#include "presence.h"

Presence::Presence(AbstractDevice *owner, preset_data_cplegacy_t *presetData)
    : AbstractModule{owner, ModuleType::PS, "PS", "po"}
{
    m_presenceVolume = new ControlValue(this, &presetData->presence_vol, "pv", "Presence");
    m_fullModuleName = AbstractModule::tr("Presence");

    m_moduleEnabled = (bool*)&presetData->presence_on;
}


void Presence::setValues(const preset_data_cplegacy_t& presetData)
{
    *m_moduleEnabled = presetData.presence_on;
    m_presenceVolume->setControlValue(presetData.presence_vol);

    emit dataChanged();
}
