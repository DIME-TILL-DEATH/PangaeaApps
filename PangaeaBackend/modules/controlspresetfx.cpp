#include "controlspresetfx.h"

ControlsPresetFx::ControlsPresetFx(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::MASTER, "MASTER", ""}
{
    m_masterVolume = new ControlValue(this, nullptr, "vl_ms", "Master", "", 0, 127, 0, 127);
    m_phonesVolume = new ControlValue(this, nullptr, "vl_ph", "Phones", "", 0, 127, 0, 127);

    m_presetVolume = new ControlValue(this, nullptr, "vl_pr", "Preset volume", "", 0, 127, 0, 127);
    m_presetCtrlVolume = new ControlValue(this, nullptr, "vl_pr_cntrl", "Control");
    m_cntrlPcOut = new ControlValue(this, nullptr, "cntrl_pc", "PC Out");
    m_cntrlSet = new ControlValue(this, nullptr, "cntrl_set", "Set");
}

void ControlsPresetFx::setMasterValues(quint8 masterVolume, quint8 phonesVolume)
{
    m_masterVolume->setControlValue(masterVolume);
    m_phonesVolume->setControlValue(phonesVolume);
}

void ControlsPresetFx::setPresetValues(const preset_data_fx_t &presetData)
{
    m_presetCtrlVolume->setControlValue(presetData.modules.volume_control);
    m_cntrlPcOut->setControlValue(presetData.pcOut);
    m_cntrlSet->setControlValue(presetData.set);
}
