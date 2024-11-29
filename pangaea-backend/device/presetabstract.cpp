#include <QDebug>

#include "presetabstract.h"

PresetAbstract::PresetAbstract() {}

PresetAbstract &PresetAbstract::operator=(const PresetAbstract &preset)
{
    m_bankNumber = preset.m_bankNumber;
    m_presetNumber = preset.m_presetNumber;
    return *this;
}

void PresetAbstract::setBankPreset(quint8 newBankNumber, quint8 newPresetNumber)
{
    m_bankNumber = newBankNumber;
    m_presetNumber = newPresetNumber;
}

