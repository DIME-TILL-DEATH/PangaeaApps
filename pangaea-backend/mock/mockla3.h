#ifndef MOCKLA3_H
#define MOCKLA3_H

#include "mockcp16modern.h"

class MockLa3 : public MockCP16Modern
{
    Q_OBJECT
public:
    explicit MockLa3(QMutex* mutex, QByteArray* uartBuffer, QObject *parent = nullptr);

    static QString mockName() {return "offline LA3 preamp";};
private:
    void amtDevLa3CommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void la3MapCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
};

#endif // MOCKLA3_H
