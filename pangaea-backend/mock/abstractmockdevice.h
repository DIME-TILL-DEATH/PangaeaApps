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
    void writeToDevice(const QByteArray& data);

protected:
    Parser m_parser;
};

#endif // ABSTRACTMOCKDEVICE_H
