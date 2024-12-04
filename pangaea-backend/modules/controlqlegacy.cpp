#include "controlqlegacy.h"

ControlQLegacy::ControlQLegacy(AbstractModule *parent, QString commandStr)
    : ControlValue{parent, commandStr,"Q-Factor", "", 100, -100, 0.25, 5.2} //-100, 0, 0.1, 10.1);
{

}

void ControlQLegacy::setDisplayValue(double newDisplayValue)
{
    if (qFuzzyCompare(m_displayValue, newDisplayValue))
        return;

    m_displayValue = newDisplayValue;

    m_isModified = true;
    emit isModifiedChanged();
    emit displayValueChanged();


    quint8 controlValue = 100 - powf((m_displayValue-0.225) * powf(200, 3)/5, 1.0/3.0);

    QString strValue;
    strValue.setNum(controlValue, 16);
    if(strValue.size() > 2) strValue = strValue.right(2);

    QString fullCommand = m_commandString + " " + strValue + "\r\n"; //\r

    if(buffer.isEmpty())
    {
        buffer.append(fullCommand.toUtf8());
    }
    else
    {
        // drop same values
        if(buffer.last() != fullCommand.toUtf8()) buffer.append(fullCommand.toUtf8());
    }
    // if(m_owner) m_owner->sendDataToDevice(fullCommand.toUtf8());

    emit userModifiedValue();
}

void ControlQLegacy::setControlValue(qint32 value)
{

    double resultValue = powf((200 - (value+100)), 3) * (5/powf(200, 3)) + 0.225;

    if(resultValue == m_displayValue) return;

    m_displayValue = resultValue;
    emit displayValueChanged();
}
