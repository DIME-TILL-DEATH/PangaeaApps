#include "mastereq.h"

#include <QtEndian>

MasterEq::MasterEq(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::MEQ, "MEQ", "meq_on"}
{
    m_lowGain = new ControlValue(this, nullptr, "meq_lg", "Low", "dB",  -24, 24, -12, +12);
    m_midGain = new ControlValue(this, nullptr, "meq_mg", "Mid", "dB",  -24, 24, -12, +12);
    m_midFrequency = new ControlValue(this, nullptr, "meq_mf", "Mid. freq", "Hz",  20, 4000, 20, 4000);
    m_highGain = new ControlValue(this, nullptr, "meq_hg", "High", "dB",  -24, 24, -12, +12);

    m_moduleEnabled = new bool;
}

MasterEq::~MasterEq()
{
    delete(m_moduleEnabled);
}

void MasterEq::setValues(const TSystemSettingsFx &sysSettings)
{
    *m_moduleEnabled = sysSettings.masterEqOn;

    m_lowGain->setControlValue(static_cast<int8_t>(sysSettings.masterEqLow));
    m_midGain->setControlValue(static_cast<int8_t>(sysSettings.masterEqMid));
    m_highGain->setControlValue(static_cast<int8_t>(sysSettings.masterEqHigh));

    m_midFrequency->setControlValue(qFromBigEndian(sysSettings.masterEqFreq));

    emit dataChanged();
}

ControlValue *MasterEq::lowGain() const
{
    return m_lowGain;
}

ControlValue *MasterEq::midGain() const
{
    return m_midGain;
}

ControlValue *MasterEq::midFrequency() const
{
    return m_midFrequency;
}

ControlValue *MasterEq::highGain() const
{
    return m_highGain;
}
