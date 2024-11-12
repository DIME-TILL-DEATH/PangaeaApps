#ifndef ABSTRACTMOCKDEVICE_H
#define ABSTRACTMOCKDEVICE_H

#include <QObject>

#include "parser.h"

class AbstractMockDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMockDevice(QObject *parent = nullptr);

signals:
    void answerReady(const QByteArray& answerData);

public slots:
    virtual void writeToDevice(const QByteArray& data);

protected:
    Parser m_parser{"Mock device recieve"};

    quint8 m_outputMode{0};
    quint8 m_bank{0};
    quint8 m_preset{0};
};

#endif // ABSTRACTMOCKDEVICE_H
