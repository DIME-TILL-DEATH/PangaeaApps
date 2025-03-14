#ifndef DEVICEANSWER_H
#define DEVICEANSWER_H

#include <QString>
#include <QList>
#include <QByteArray>
#include <QDebug>

#include "maskedparser.h"

enum AnswerType{
    unknown = 0,
    //gsEND,
    ccAck,
    ccError,
    ccEND,
    getAMTDevType,
    getStatus,
    getBankPreset,
    getIr,
    getIrNameSize,
    getIrEmpty,
    getOutputMode,
    getIrName,
    getIrNameEmpty,
    getIrList,
    getFWVersion,
    ackStatusSettling,
    ackCe,
    ackDcc,
    ackLcc,
    ackEsc,
    ackSavePreset,
    ackPresetChange,
    ackFWUFinished,
    ackDisableSW4,
    ackEnableSW4,
    ackOutputMode,
    ackPwc,
    ackPws,
    ackPwl,
    ackSetParameter,
    requestNextChunk,
    indicationRequest,
    la3CleanPreset,
    la3DrivePreset,
    la3ModeChange,
    la3CurrentChannel,
    endOperation
};

class DeviceAnswer
{
public:

    explicit DeviceAnswer(MaskedParser* parser, AnswerType type = AnswerType::unknown, QString description="", quint32 timeout=1000, bool displayble=true);

    bool processRawData(QByteArray rawData);

    AnswerType answerType() const;
    const QString &description() const;
    const QList<QByteArray> &parseResult() const;

    bool isEnableRecieve() const;

    bool displayble() const;

private:
    MaskedParser* m_parser;

    AnswerType m_answerType;
    QString m_description;

    bool m_isEnableRecieve=false;
    bool m_displayble{true};

    QList<QByteArray> m_parseResult;

    quint32 m_timeout;
};

#endif // DEVICEANSWER_H
