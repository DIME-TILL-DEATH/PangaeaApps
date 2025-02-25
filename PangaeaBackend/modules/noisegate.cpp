#include "noisegate.h"

NoiseGate::NoiseGate(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::NG, "NG", "go"}
{
    m_processingTime = 5;
    m_fullModuleName = AbstractModule::tr("Noise gate");

    m_threshold = new ControlValue(this, "gt", "Threshold");
    m_decay = new ControlValue(this, "gd", "Decay");
}

void NoiseGate::setValues(bool enabled, quint8 threshold, quint8 decay)
{
    m_moduleEnabled = enabled;
    m_threshold->setControlValue(threshold);
    m_decay->setControlValue(decay);

    emit dataChanged();
}

void NoiseGate::setValues(const gate_data_t& ngData)
{
    m_moduleEnabled = ngData.on;
    m_threshold->setControlValue(ngData.threshold);
    m_decay->setControlValue(ngData.decay);

    emit dataChanged();
}

