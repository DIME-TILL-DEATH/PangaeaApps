#include "cabsim.h"

CabSim::CabSim(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::IR, "IR", "ce"}
{


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
