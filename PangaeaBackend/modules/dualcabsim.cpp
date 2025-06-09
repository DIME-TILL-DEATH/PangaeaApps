#include "dualcabsim.h"

DualCabSim::DualCabSim(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::IR_STEREO, "IR", "ir_on"}
{
    m_processingTime = 100;
    m_fullModuleName = AbstractModule::tr("Cab sim");

    m_firstCabVolume = new ControlValue(this, "ir_v1", "Volume", "", 0, 127, 0, 127);
    m_firstCabPan = new ControlValue(this, "ir_p1", "Pan", "", 0, 127, -63, 64);
    m_firstCabDelay = new ControlValue(this, "ir_d1", "Delay", "", 0, 127, 0, 127);
    m_secondCabVolume = new ControlValue(this, "ir_v2", "Volume", "", 0, 127, 0, 127);
    m_secondCabPan = new ControlValue(this, "ir_p2", "Pan", "", 0, 127, -63, 64);
    m_secondCabDelay = new ControlValue(this, "ir_d2", "Delay", "", 0, 127, 0, 127);
}

void DualCabSim::setValues(const preset_data_fx_t &csData)
{
    m_moduleEnabled = csData.switches.cab;

    m_firstCabVolume->setControlValue(csData.cab1.volume);
    m_firstCabPan->setControlValue(csData.cab1.pan);
    m_firstCabDelay->setControlValue(csData.cab1.delay);
    m_secondCabVolume->setControlValue(csData.cab2.volume);
    m_secondCabPan->setControlValue(csData.cab2.pan);
    m_secondCabDelay->setControlValue(csData.cab2.delay);
}
