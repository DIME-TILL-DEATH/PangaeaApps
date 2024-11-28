#include "presetabstract.h"

PresetAbstract::PresetAbstract() {}

void PresetAbstract::setBankPreset(quint8 newBankNumber, quint8 newPresetNumber)
{
    m_bankNumber = newBankNumber;
    m_presetNumber = newPresetNumber;
}

