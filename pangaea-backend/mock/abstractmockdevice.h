#ifndef ABSTRACTMOCKDEVICE_H
#define ABSTRACTMOCKDEVICE_H

#include <QObject>

#include "parser.h"

enum MockDeviceType
{
    Mock_Abstract = 0,
    Mock_CPLegacy,
    Mock_CPModern
};

class AbstractMockDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMockDevice(QObject *parent = nullptr);

    MockDeviceType mockDeviceType() {return m_mockDeviceType;};

signals:
    void answerReady(const QByteArray& answerData);

public slots:
    virtual void writeToDevice(const QByteArray& data);

protected:
    Parser m_parser{"Mock device recieve"};

    quint8 m_outputMode{0};
    quint8 m_bank{0};
    quint8 m_preset{0};

    MockDeviceType m_mockDeviceType{MockDeviceType::Mock_Abstract};
};

#endif // ABSTRACTMOCKDEVICE_H
