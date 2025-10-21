#include "controlfcutlegacy.h"


ControlFCutLegacy::ControlFCutLegacy(AbstractModule *parent, void *pointer, FilterType filterType)
    : ControlValue{parent, pointer, "", "Cut freq.", "Hz", 0, 127, 20, 2000},
    m_filterType(filterType)
{
    switch(m_filterType)
    {
        case FilterTypeEnum::LOW_CUT:
        {
            m_commandString = "hp_f";
            m_minControlValue = 0;
            m_maxControlValue = 127;
            m_minDisplayValue = 20;
            m_maxDisplayValue = 1000;
            break;
        }
        case FilterTypeEnum::HIGH_CUT:
        {
            m_commandString = "lp_f";
            m_minControlValue = 127;
            m_maxControlValue = 0;
            m_minDisplayValue = 1000;
            m_maxDisplayValue = 20000;
            break;
        }
        default:
        break;
    }



}

void ControlFCutLegacy::setDisplayValue(double newDisplayValue)
{
    if (qFuzzyCompare(m_displayValue, newDisplayValue))
        return;

    m_displayValue = newDisplayValue;

    m_isModified = true;
    emit isModifiedChanged();
    emit displayValueChanged();

    quint8 controlValue = 0;


    switch(m_filterType)
    {
    case FilterTypeEnum::LOW_CUT:
    {
        controlValue = static_cast<qint8>((m_displayValue - 20.0) * 127.0/980.0);
        break;
    }
    case FilterTypeEnum::HIGH_CUT:
    {
        controlValue = static_cast<qint8>(127.0 - sqrtf((m_displayValue - 1000.0) * powf(127.0, 2.0) / 19000.0));
        break;
    }
    default:
        break;
    }

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

    emit userModifiedValue();
}

void ControlFCutLegacy::setControlValue(qint32 value)
{
    double resultValue = 0;

    switch(m_filterType)
    {
    case FilterTypeEnum::LOW_CUT:
    {
        resultValue = value * (980.0/127.0) + 20.0;
        break;
    }
    case FilterTypeEnum::HIGH_CUT:
    {
        resultValue = powf(127.0 - value, 2.0) * (19000.0 / powf(127, 2)) + 1000.0;
        break;
    }
    default:
        break;
    }

    if(resultValue == m_displayValue) return;

    m_displayValue = resultValue;
    emit displayValueChanged();
}
