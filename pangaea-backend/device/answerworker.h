#ifndef ANSWERWORKER_H
#define ANSWERWORKER_H

#include <QList>

#include "deviceanswer.h"

class AnswerWorker
{
public:
    AnswerWorker();

    DeviceAnswer parseRawData(const QByteArray& rawData);

    void parseAnswers(const QByteArray& rawData);
    DeviceAnswer popAnswer();
    bool haveAnswer();

    bool displayNextAnswer();
private:
    QList<DeviceAnswer> m_devCommandList;
    QList<DeviceAnswer> m_recievedAnswers;
};

#endif // ANSWERWORKER_H
