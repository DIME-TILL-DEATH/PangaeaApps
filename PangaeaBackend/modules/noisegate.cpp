#include "noisegate.h"

NoiseGate::NoiseGate(AbstractDevice *owner, GateType gateType)
    : AbstractModule{owner, ModuleType::NG, "NG", "go"}
{
    m_fullModuleName = AbstractModule::tr("Noise gate");

    switch(gateType)
    {
    case GateType::FX:
    {
        m_processingTime = 10;

        m_commandOnOff = "ng_on";

        m_threshold = new ControlValue(this, "ng_th", "Threshold", "", 0, 127, 0, 127);
        m_attack = new ControlValue(this, "ng_at", "Attack", "ms", 0, 127, 0, 127);
        m_decay = new ControlValue(this, "ng_dc", "Decay", "ms", 0, 127, 0, 1500);
        break;
    }
    case GateType::Classic:
    {
        m_processingTime = 5;
        m_threshold = new ControlValue(this, "gt", "Threshold");
        m_decay = new ControlValue(this, "gd", "Decay");
        break;
    }
    }
}

void NoiseGate::setValues(bool enabled, quint8 threshold, quint8 decay)
{
    m_moduleEnabled = enabled;

    m_threshold->setControlValue(threshold);
    m_decay->setControlValue(decay);

    emit dataChanged();
}

void NoiseGate::setValues(const gate_cpmodern_t& ngData)
{
    m_moduleEnabled = ngData.on;
    m_threshold->setControlValue(ngData.threshold);
    m_decay->setControlValue(ngData.decay);

    emit dataChanged();
}

void NoiseGate::setValues(uint8_t enabled, const gate_fx_t &ngData)
{
    m_moduleEnabled = enabled;

    m_threshold->setControlValue(ngData.threshold);
    m_attack->setControlValue(ngData.attack);
    m_decay->setControlValue(ngData.decay);

    emit dataChanged();
}

