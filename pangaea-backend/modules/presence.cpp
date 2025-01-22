#include "presence.h"

Presence::Presence(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::PS, "PS", "po"}
{
    m_presenceVolume = new ControlValue(this, "pv", "Presence");
    m_fullModuleName = AbstractModule::tr("Presence");
}

void Presence::setValues(bool enabled, quint8 value)
{
    m_moduleEnabled = enabled;
    m_presenceVolume->setControlValue(value);

    emit dataChanged();
}
