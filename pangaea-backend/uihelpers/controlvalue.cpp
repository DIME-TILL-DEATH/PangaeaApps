#include "controlvalue.h"

ControlValue::ControlValue(AbstractModule *parent, DeviceParameter::Type deviceParameterType,
                           qint16 minControlValue, qint16 maxControlValue,
                           double minDisplayValue, double maxDisplayValue)
    : QObject{parent},
    m_deviceParameterType{deviceParameterType},
    m_minControlValue{minControlValue},
    m_maxControlValue{maxControlValue},
    m_minDisplayValue{minDisplayValue},
    m_maxDisplayValue{maxDisplayValue}
{
    if(parent != nullptr)
    {
        connect(this, &ControlValue::sgSetDeviceParameter, parent, &AbstractModule::sgSetDeviceParameter);
        connect(parent, &AbstractModule::sgSetUiDeviceParameter, this, &ControlValue::slSetControlValue);
        connect(parent, &AbstractModule::sgSetAppParameter, this, &ControlValue::slSetAppParameter);
    }
}

void ControlValue::setDisplayValue(double newDisplayValue)
{
    if (qFuzzyCompare(m_displayValue, newDisplayValue))
        return;

    m_displayValue = newDisplayValue;
    emit displayValueChanged();

    m_isModified = true;
    emit isModifiedChanged();

    double k2 = (m_minDisplayValue-m_maxDisplayValue)/(m_minControlValue-m_maxControlValue);
    double k1 = m_minDisplayValue-(m_minControlValue*k2);
    qint8 controlValue = (m_displayValue - k1)/k2;
    emit sgSetDeviceParameter(m_deviceParameterType, controlValue);

}

void ControlValue::slSetControlValue(DeviceParameter::Type deviceParameterType, qint32 value)
{
    if(deviceParameterType != m_deviceParameterType) return;

    double k2 = (m_minDisplayValue-m_maxDisplayValue)/(m_minControlValue-m_maxControlValue);
    double k1 = m_minDisplayValue-(m_minControlValue*k2);
    double resultValue = k1 + value*k2;

    if(resultValue == m_displayValue) return;

    m_displayValue = resultValue;
    emit displayValueChanged();
}

void ControlValue::slSetAppParameter(Core::AppParameter appParameterType, QVariant content)
{
    switch(appParameterType)
    {
    case Core::AppParameter::PRESET_MODIFIED:
        if(content.toBool() == false)
        {
            m_isModified = false;
            emit isModifiedChanged();
        }
        break;

    default: {}
    }
}

