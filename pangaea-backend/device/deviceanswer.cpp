#include "deviceanswer.h"

DeviceAnswer::DeviceAnswer(MaksedParser* parser, AnswerType type, QString description, quint32 timeout, bool displayble)
    : m_parser{parser},
      m_answerType{type},
      m_description{description},
    m_displayble{displayble},
    m_timeout{timeout}
{

}

bool DeviceAnswer::processRawData(QByteArray rawData)
{
    m_isEnableRecieve = false;
    if(m_parser->getParse(rawData, &m_parseResult))
    {
        m_isEnableRecieve = true;
    }   
    return m_isEnableRecieve;
}

AnswerType DeviceAnswer::answerType() const
{
    return m_answerType;
}

const QString &DeviceAnswer::description() const
{
    return m_description;
}

const QList<QByteArray> &DeviceAnswer::parseResult() const
{
    return m_parseResult;
}

bool DeviceAnswer::isEnableRecieve() const
{
    return m_isEnableRecieve;
}

bool DeviceAnswer::displayble() const
{
    return m_displayble;
}
