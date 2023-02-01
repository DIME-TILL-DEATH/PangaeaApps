#include "deviceparameter.h"

DeviceParameter::DeviceParameter(QString paramName, QString commHandler, quint8 position, bool isSigned)
    : m_paramName{paramName},
      m_commHandler{commHandler},
      m_positionInArray{position},
      m_isSigned{isSigned}
{

}

//QString DevParam::sendString()
//{

//}

QString DeviceParameter::sendString(quint8 value)
{
    return m_commHandler + QString(" %1\r\n").arg(value, 0, 16);
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
    return m_positionInArray;
}

bool DeviceParameter::isSigned() const
{
    return m_isSigned;
}
