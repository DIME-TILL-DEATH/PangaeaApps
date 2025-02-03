#include "cabsim.h"

CabSim::CabSim(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::IR, "IR", "ce"}
{
    m_fullModuleName = AbstractModule::tr("Cabinet simulator");

    m_send = new ControlValue(this, "ir_sn", "Send", "", 0, 31, 0, 31);
}

QString CabSim::impulseName() const
{
    return m_impulseName;
}

void CabSim::setImpulseName(const QString &impulseName)
{
    if(impulseName == m_impulseName) return;

    m_impulseName = impulseName;
    emit dataChanged();
}

void CabSim::setEnabled(bool isEnabled)
{
    m_moduleEnabled = isEnabled;
    emit dataChanged();
}

void CabSim::setSendLevel(quint8 sendLevel)
{
    m_send->setControlValue(sendLevel);
}
