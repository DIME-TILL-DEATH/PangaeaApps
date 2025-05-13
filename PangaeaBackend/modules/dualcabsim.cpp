#include "dualcabsim.h"

DualCabSim::DualCabSim(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::IR_STEREO, "IR", "ir_on"}
{
    m_processingTime = 100;
    m_fullModuleName = AbstractModule::tr("Cab sim");
}
