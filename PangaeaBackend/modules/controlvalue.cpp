#include "controlvalue.h"

#include <QDebug>


ControlValue::ControlValue(AbstractModule *owner, QString commandName,
                           QString name, QString units,
                           qint16 minControlValue, qint16 maxControlValue,
                           double minDisplayValue, double maxDisplayValue)
    :QObject{owner},
    m_owner{owner},
    m_commandString{commandName},
    m_minControlValue{minControlValue},
    m_maxControlValue{maxControlValue},
    m_minDisplayValue{minDisplayValue},
    m_maxDisplayValue{maxDisplayValue},
    m_name{name},
    m_units{units}
{
    if(owner) connect(this, &ControlValue::userModifiedValue, owner, &AbstractModule::userModifiedModuleParameters);

    connect(&frameTimer, &QTimer::timeout, this, &ControlValue::sendFrame);
    frameTimer.start(100);
}

ControlValue::~ControlValue()
{
    frameTimer.stop();
}

void ControlValue::setDisplayValue(double newDisplayValue)
{
    if (qFuzzyCompare(m_displayValue, newDisplayValue))
        return;

    m_displayValue = newDisplayValue;

    m_isModified = true;
    emit isModifiedChanged();
    emit displayValueChanged();

    double k2 = (m_minDisplayValue-m_maxDisplayValue)/(m_minControlValue-m_maxControlValue);
    double k1 = m_minDisplayValue-(m_minControlValue*k2);

    QString fullCommand;
    QString strValue;
    if(m_maxControlValue>0xFF)
    {
        quint16 controlValue = static_cast<qint16>((m_displayValue - k1)/k2);
        strValue.setNum(controlValue, 16);
        if(strValue.size() > 4) strValue = strValue.right(4);
        fullCommand = m_commandString + " " + strValue + "\r\n";
    }
    else
    {
        quint8 controlValue = static_cast<qint8>((m_displayValue - k1)/k2);
        
        strValue.setNum(controlValue, 16);
        if(strValue.size() > 2) strValue = strValue.right(2);
    }
    fullCommand = m_commandString + " " + strValue + "\r\n";

    if(buffer.isEmpty())
    {
        buffer.append(fullCommand.toUtf8());
    }
    else
    {
        // drop same values
        if(buffer.last().indexOf(fullCommand.toUtf8()) == -1)
        {
            buffer.append(fullCommand.toUtf8());
        }
    }
    // if(m_owner) m_owner->sendDataToDevice(fullCommand.toUtf8());

    emit userModifiedValue();
}

void ControlValue::sendFrame()
{
    if(buffer.size() > 0)
    {
        QByteArray resultBa;
        foreach(QByteArray comm, buffer)
        {
            resultBa.append(comm);
        }

        if(m_owner)
        {
            m_owner->sendDataToDevice(resultBa);
        }
        buffer.clear();
    }
}

bool ControlValue::enabled() const
{
    if(m_owner != nullptr)
        return m_owner->moduleEnabled();
    else
        return true;
}


void ControlValue::setControlValue(qint32 value)
{
    if(value > fmax(m_minControlValue, m_maxControlValue)) value = m_maxControlValue;
    if(value < fmin(m_minControlValue, m_maxControlValue)) value = m_minControlValue;

    double k2 = (m_minDisplayValue-m_maxDisplayValue)/(m_minControlValue-m_maxControlValue);
    double k1 = m_minDisplayValue-(m_minControlValue*k2);
    double resultValue = k1 + value*k2;

    if(resultValue == m_displayValue) return;

    m_displayValue = resultValue;
    emit displayValueChanged();
}

void ControlValue::setIsModified(bool newIsModified)
{
    if (m_isModified == newIsModified)
        return;
    m_isModified = newIsModified;
    emit isModifiedChanged();
}
