#include "attenuator.h"

Attenuator::Attenuator(AbstractDevice *owner)
    : AbstractModule{owner, ModuleType::ATTENUATOR, "ATTENUATOR", ""}
{
    m_source = new ControlValue(this, nullptr, "vl_at_mode", "Att. src");
    m_source->setControlSetter(std::bind(&Attenuator::srcControlSetter, this, std::placeholders::_1));
    m_source->setDisplaySetter(std::bind(&Attenuator::srcDisplaySetter, this, std::placeholders::_1));

    m_value = new ControlValue(this, nullptr, "vl_at", "Attenuator", "dB", 0, 127, 0, 127);
    m_value->setControlSetter(std::bind(&Attenuator::valControlSetter, this, std::placeholders::_1));
}

void Attenuator::setPresetValue(quint8 val)
{
    m_presetAttValue = val;

    if(m_source->displayValue())
    {
        m_value->modifyDisplayValue(val);
        emit m_value->displayValueChanged();
    }
    emit dataChanged();
}

void Attenuator::setGlobalValue(quint8 val)
{
    m_globalAttValue = val;

    if(!m_source->displayValue())
    {
        m_value->modifyDisplayValue(val);
        emit m_value->displayValueChanged();
    }

    emit dataChanged();
}

void Attenuator::setSource(quint8 src)
{
    m_source->setControlValue(src);

    emit dataChanged();
}

void Attenuator::srcDisplaySetter(double value)
{
    if(value) m_value->modifyDisplayValue(m_presetAttValue);
    else m_value->modifyDisplayValue(m_globalAttValue);

    emit m_value->displayValueChanged();

    QString strValue;
    quint8 intValue = value;

    m_source->modifyDisplayValue(value);
    strValue.setNum(intValue, 16);
    sendDataToDevice(m_source->commandString().toUtf8() + " " + strValue.toUtf8() + "\r\n");

    m_source->setIsModified(true);
    emit m_source->isModifiedChanged();
    emit m_source->displayValueChanged();
    emit m_source->userModifiedValue();
}

void Attenuator::srcControlSetter(qint32 value)
{
    m_source->modifyDisplayValue(value);
    emit m_source->displayValueChanged();

    if(value) m_value->modifyDisplayValue(m_presetAttValue);
    else m_value->modifyDisplayValue(m_globalAttValue);

    emit m_value->displayValueChanged();
}

void Attenuator::valControlSetter(qint32 value)
{
    if(m_source->displayValue() == 1) m_presetAttValue = value;
    else m_globalAttValue = value;

    m_value->modifyDisplayValue(value);

    emit m_value->displayValueChanged();
}
