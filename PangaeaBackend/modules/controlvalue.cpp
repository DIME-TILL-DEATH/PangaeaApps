#include "controlvalue.h"

#include "parser.h"
#include "abstractdevice.h"

ControlValue::ControlValue(AbstractModule *owner, void *valuePtr, QString commandName,
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
    if(owner)
    {
        using namespace std::placeholders;
        connect(this, &ControlValue::userModifiedValue, owner, &AbstractModule::userModifiedModuleParameters);
        AbstractDevice* ownerDevice = m_owner->owner();
        Parser* parser = ownerDevice->dataInParser();
        parser->addSetterHandler(commandName, std::bind(&ControlValue::setControlValue, this, _1));
    }

    value_ptr = valuePtr;

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

    if(m_customDisplaySetter)
    {
        m_customDisplaySetter(newDisplayValue);
    }
    else
    {
        m_displayValue = newDisplayValue;

        m_isModified = true;

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

            if(value_ptr) *static_cast<quint16*>(value_ptr) = controlValue;
        }
        else
        {
            quint8 controlValue = static_cast<qint8>((m_displayValue - k1)/k2);

            strValue.setNum(controlValue, 16);
            if(strValue.size() > 2) strValue = strValue.right(2);

            if(value_ptr) *static_cast<quint8*>(value_ptr) = controlValue;
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
    }

    emit isModifiedChanged();
    emit displayValueChanged();
    emit userModifiedValue();
}

void ControlValue::setControlValue(qint32 value)
{
    if(m_customControlSetter)
    {
        m_customControlSetter(value);
    }
    else
    {
        if(value > fmax(m_minControlValue, m_maxControlValue)) value = m_maxControlValue;
        if(value < fmin(m_minControlValue, m_maxControlValue)) value = m_minControlValue;

        double k2 = (m_minDisplayValue-m_maxDisplayValue)/(m_minControlValue-m_maxControlValue);
        double k1 = m_minDisplayValue-(m_minControlValue*k2);
        double resultValue = k1 + value*k2;

        if(resultValue == m_displayValue) return;

        m_displayValue = resultValue;

        if(value_ptr)
        {
            if(m_maxControlValue>0xFF)
            {
                if(value_ptr) *static_cast<quint16*>(value_ptr) = value;
            }
            else
            {
                if(value_ptr) *static_cast<quint8*>(value_ptr) = value;
            }
        }
    }

    emit displayValueChanged();
}

void ControlValue::modifyDisplayValue(double value)
{
    m_displayValue = value;
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

void ControlValue::setControlSetter(std::function<void (qint32)> setter)
{
    m_customControlSetter = setter;
}

void ControlValue::setDisplaySetter(std::function<void (qint32)> setter)
{
    m_customDisplaySetter = setter;
}

QString ControlValue::commandString() const
{
    return m_commandString;
}

void ControlValue::setIsModified(bool newIsModified)
{
    if (m_isModified == newIsModified)
        return;
    m_isModified = newIsModified;
    emit isModifiedChanged();
}
