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
        m_attack = new ControlValue(this, "ng_at", "Attack", "", 0, 127, 0, 127); //"ms", 0, 127, 0, 127);
        m_decay = new ControlValue(this, "ng_dc", "Decay", "", 0, 127, 0, 127); //"ms", 0, 127, 0, 1500);
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

void NoiseGate::setValues(const preset_data_cplegacy_t &ngData)
{
    m_moduleEnabled = ngData.gate_on;

    m_threshold->setControlValue(ngData.gate_threshold);
    m_decay->setControlValue(ngData.gate_decay);

    emit dataChanged();
}

void NoiseGate::setValues(const preset_data_cpmodern_t &ngData)
{
    m_moduleEnabled = ngData.gate.on;
    m_threshold->setControlValue(ngData.gate.threshold);
    m_decay->setControlValue(ngData.gate.decay);

    emit dataChanged();
}

void NoiseGate::setValues(const preset_data_fx_t& ngData)
{
    m_moduleEnabled = ngData.switches.gate;

    m_threshold->setControlValue(ngData.gate.threshold);
    m_attack->setControlValue(ngData.gate.attack);
    m_decay->setControlValue(ngData.gate.decay);

    emit dataChanged();
}

