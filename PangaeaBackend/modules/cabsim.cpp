#include "cabsim.h"

CabSim::CabSim(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::IR, "IR", "ce"}
{
    m_fullModuleName = AbstractModule::tr("Cab. sim.");

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

void CabSim::setValues(const preset_data_cplegacy_t &csData)
{
    m_moduleEnabled = csData.cab_on;
}

void CabSim::setValues(const preset_data_cpmodern_t &csData)
{
    m_moduleEnabled = csData.cab_sim_on;
    m_send->setControlValue(csData.ir_send_level);

    emit dataChanged();
}
