#include "cabsim.h"

CabSim::CabSim(AbstractDevice *owner, preset_data_cplegacy_t *csData)
    : AbstractModule{owner, ModuleType::IR, "IR", "ce"}
{
    m_fullModuleName = AbstractModule::tr("Cab. sim.");

    m_moduleEnabled = (bool*)&csData->cab_on;
}

CabSim::CabSim(AbstractDevice *owner, preset_data_cpmodern_t *csData)
    : AbstractModule{owner, ModuleType::IR, "IR", "ce"}
{
    m_fullModuleName = AbstractModule::tr("Cab. sim.");

    m_send = new ControlValue(this, &csData->ir_send_level ,"ir_sn", "Send", "", 0, 31, 0, 31);

    m_moduleEnabled = (bool*)&csData->cab_sim_on;
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
    *m_moduleEnabled = csData.cab_on;

    emit dataChanged();
}

void CabSim::setValues(const preset_data_cpmodern_t &csData)
{
    *m_moduleEnabled = csData.cab_sim_on;
    m_send->setControlValue(csData.ir_send_level);

    emit dataChanged();
}
