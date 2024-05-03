#include "deviceparameter.h"

DeviceParameter::DeviceParameter(Type parameterType, QString paramName)
    : m_parameterType{parameterType},
    m_paramName{paramName}
{

}

QString DeviceParameter::sendString(quint8 value) const
{
    return sendString(m_parameterType, value);
}

QString DeviceParameter::sendString(Type parameterType, quint8 value)
{
    QString fullString;

    switch(parameterType)
    {

    case Type::EQ_VOLUME1: fullString += "eqv 0"; break;
    case Type::EQ_VOLUME2: fullString += "eqv 1"; break;
    case Type::EQ_VOLUME3: fullString += "eqv 2"; break;
    case Type::EQ_VOLUME4: fullString += "eqv 3"; break;
    case Type::EQ_VOLUME5: fullString += "eqv 4"; break;

    case Type::EARLY_VOLUME: fullString += "ev"; break;
    case Type::EARLY_TYPE: fullString += "et"; break;

    case Type::MASTER_VOLUME: fullString += "mv"; break;

    case Type::CABINET_ENABLE: fullString += "ce"; break;

    case Type::EQ_ON: fullString += "eqo"; break;

    case Type::EARLY_ON: fullString += "eo"; break;

    case Type::AMP_ON: fullString += "ao"; break;
    case Type::AMP_VOLUME: fullString += "av"; break;
    case Type::AMP_SLAVE: fullString += "as"; break;
    case Type::AMP_TYPE: fullString += "at"; break;

    case Type::PREAMP_ON: fullString += "pro"; break;
    case Type::PREAMP_VOLUME: fullString += "prv"; break;
    case Type::PREAMP_LOW: fullString += "prl"; break;
    case Type::PREAMP_MID: fullString += "prm"; break;
    case Type::PREAMP_HIGH: fullString += "prh"; break;

    case Type::GATE_ON: fullString += "go"; break;
    case Type::GATE_THRESHOLD: fullString += "gt"; break;
    case Type::GATE_DECAY: fullString += "gd"; break;

    case Type::COMPRESSOR_ON: fullString += "co"; break;
    case Type::COMPRESSOR_SUSTAIN: fullString += "cs"; break;
    case Type::COMPRESSOR_VOLUME: fullString += "cv"; break;

    case Type::EQ_FREQ1: fullString += "eqf 0"; break;
    case Type::EQ_FREQ2: fullString += "eqf 1"; break;
    case Type::EQ_FREQ3: fullString += "eqf 2"; break;
    case Type::EQ_FREQ4: fullString += "eqf 3"; break;
    case Type::EQ_FREQ5: fullString += "eqf 4"; break;

    case Type::EQ_Q1: fullString += "eqq 0"; break;
    case Type::EQ_Q2: fullString += "eqq 1"; break;
    case Type::EQ_Q3: fullString += "eqq 2"; break;
    case Type::EQ_Q4: fullString += "eqq 3"; break;
    case Type::EQ_Q5: fullString += "eqq 4"; break;

    case Type::LPF_VOLUME: fullString += "lv"; break;
    case Type::HPF_VOLUME: fullString += "hv"; break;
    case Type::HPF_ON: fullString += "ho"; break;
    case Type::LPF_ON: fullString += "lo"; break;

    case Type::PRESENCE_ON: fullString += "po"; break;
    case Type::PRESENCE_VOLUME: fullString += "pv"; break;

    case Type::EQ_PRE: fullString += "eqp"; break;
    }

    fullString += QString(" %1\r\n").arg(value, 0, 16);
    return fullString;
}

const QString &DeviceParameter::paramName() const
{
    return m_paramName;
}

quint8 DeviceParameter::value() const
{
    return m_value;
}

void DeviceParameter::setValue(quint8 newValue)
{
    m_value = newValue;
}

quint16 DeviceParameter::positionInArray() const
{
    return (quint8)m_parameterType;
}

bool DeviceParameter::isSigned() const
{
    switch(m_parameterType)
    {
    case Type::PREAMP_LOW: return true;
    case Type::PREAMP_MID: return true;
    case Type::PREAMP_HIGH: return true;

    case Type::EQ_FREQ1: return true;
    case Type::EQ_FREQ2: return true;
    case Type::EQ_FREQ3: return true;
    case Type::EQ_FREQ4: return true;
    case Type::EQ_FREQ5: return true;

    case Type::EQ_Q1: return true;
    case Type::EQ_Q2: return true;
    case Type::EQ_Q3: return true;
    case Type::EQ_Q4: return true;
    case Type::EQ_Q5: return true;

    default: return false;
    }
}

DeviceParameter::Type DeviceParameter::parameterType() const
{
    return m_parameterType;
}
