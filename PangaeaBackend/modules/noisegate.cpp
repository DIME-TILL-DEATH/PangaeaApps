#include "noisegate.h"

NoiseGate::NoiseGate(AbstractDevice *owner, preset_data_cplegacy_t *ngData)
    : AbstractModule{owner, ModuleType::NG, "NG", "go"}
{
    m_fullModuleName = AbstractModule::tr("Noise gate");
    
    m_threshold = new ControlValue(this, &ngData->gate_threshold, "gt", "Threshold");
    m_decay = new ControlValue(this, &ngData->gate_decay, "gd", "Decay");

    m_moduleEnabled = (bool*)&ngData->gate_on;
}

NoiseGate::NoiseGate(AbstractDevice *owner, preset_data_cpmodern_t *ngData)
    : AbstractModule{owner, ModuleType::NG, "NG", "go"}
{
    m_fullModuleName = AbstractModule::tr("Noise gate");
    
    m_processingTime = 5;
    m_threshold = new ControlValue(this, &ngData->gate.threshold, "gt", "Threshold");
    m_decay = new ControlValue(this, &ngData->gate.decay, "gd", "Decay");

    m_moduleEnabled = (bool*)&ngData->gate.on;
}

NoiseGate::NoiseGate(AbstractDevice *owner, modules_data_fx_t *ngData)
    : AbstractModule{owner, ModuleType::NG, "NG", "ng_on"}
{
    m_fullModuleName = AbstractModule::tr("Noise gate");
    
    m_processingTime = 10;

    m_threshold = new ControlValue(this, &ngData->gate.threshold, "ng_th", "Threshold", "", 0, 127, 0, 127);
    m_attack = new ControlValue(this, &ngData->gate.attack, "ng_at", "Attack", "", 0, 127, 0, 127); //"ms", 0, 127, 0, 127);
    m_decay = new ControlValue(this, &ngData->gate.decay, "ng_dc", "Decay", "", 0, 127, 0, 127); //"ms", 0, 127, 0, 1500);

    m_moduleEnabled = (bool*)&ngData->switches.gate;
}

void NoiseGate::setValues(const preset_data_cplegacy_t &ngData)
{
    *m_moduleEnabled = ngData.gate_on;

    m_threshold->setControlValue(ngData.gate_threshold);
    m_decay->setControlValue(ngData.gate_decay);

    emit dataChanged();
}

void NoiseGate::setValues(const preset_data_cpmodern_t &ngData)
{
    *m_moduleEnabled = ngData.gate.on;
    m_threshold->setControlValue(ngData.gate.threshold);
    m_decay->setControlValue(ngData.gate.decay);

    emit dataChanged();
}

void NoiseGate::setValues(const modules_data_fx_t& ngData)
{
    *m_moduleEnabled = ngData.switches.gate;

    m_threshold->setControlValue(ngData.gate.threshold);
    m_attack->setControlValue(ngData.gate.attack);
    m_decay->setControlValue(ngData.gate.decay);

    emit dataChanged();
}

