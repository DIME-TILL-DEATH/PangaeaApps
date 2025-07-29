#include "controlqlegacy.h"

#include "eqparametric.h"

ControlQLegacy::ControlQLegacy(AbstractModule *parent, QString commandStr)
    : ControlValue{parent, commandStr,"Q-Factor", "", 100, -100, 0.25, 5.2} //-100, 0, 0.1, 10.1);
{
    EqParametric* ownerEq = qobject_cast<EqParametric*>(parent);
    if(ownerEq)
    {
        switch (ownerEq->eqMode())
        {

        case EqParametric::Legacy: break;
        case EqParametric::Modern: break;
        case EqParametric::Fx:
        {
            m_minControlValue = -60;
            m_maxControlValue = 60;
            m_minDisplayValue = 0.1;
            m_maxDisplayValue = 10;
            break;
        }
        }
    }

}

void ControlQLegacy::setDisplayValue(double newDisplayValue)
{
    if (qFuzzyCompare(m_displayValue, newDisplayValue))
        return;

    m_displayValue = newDisplayValue;

    m_isModified = true;
    emit isModifiedChanged();
    emit displayValueChanged();

    quint8 controlValue;

    EqParametric* ownerEq = qobject_cast<EqParametric*>(m_owner);
    if(ownerEq)
    {
        switch (ownerEq->eqMode())
        {

        case EqParametric::Legacy:
        case EqParametric::Modern:
            controlValue = static_cast<qint8>(100 - powf((m_displayValue-0.225) * powf(200, 3)/5, 1.0/3.0));
            break;
        case EqParametric::Fx:
        {
            if(m_displayValue <= 1)
                controlValue = static_cast<qint8>((m_displayValue-0.701) / 0.1);
            else
                controlValue = static_cast<qint8>((m_displayValue-0.001) / 0.1 + 20);
            break;
        }
        }
    }

    // cp100fx
    // if(num <= 30)
    // {
    //     a = num * 0.01f + 0.701f;
    //     ksprintf(q_sym , "%2f" , a);
    //     if(num == 30)ksprintf(q_sym , "%1f" , a);
    // }
    // else {
    //     a = (num - 20) * 0.1f + 0.001f;
    //     ksprintf(q_sym , "%1f" , a);
    // }

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
    double resultValue;

    EqParametric* ownerEq = qobject_cast<EqParametric*>(m_owner);
    if(ownerEq)
    {
        switch (ownerEq->eqMode())
        {

        case EqParametric::Legacy:
        case EqParametric::Modern:
            resultValue = powf((200 - (value+100)), 3) * (5/powf(200, 3)) + 0.225;
            break;
        case EqParametric::Fx:
        {
            if(value <= 30)
                resultValue = value * 0.01 + 0.701;
            else
                resultValue = (value - 20) * 0.1 + 0.001;
            break;
        }
        }
    }


    // cp100fx
    // if(num <= 30)
    // {
    //     a = num * 0.01f + 0.701f;
    //     ksprintf(q_sym , "%2f" , a);
    //     if(num == 30)ksprintf(q_sym , "%1f" , a);
    // }
    // else {
    //     a = (num - 20) * 0.1f + 0.001f;
    //     ksprintf(q_sym , "%1f" , a);
    // }

    if(resultValue == m_displayValue) return;

    m_displayValue = resultValue;
    emit displayValueChanged();
}
