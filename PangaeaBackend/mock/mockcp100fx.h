#ifndef MOCKCP1_FX_H
#define MOCKCP1_FX_H

#include <QObject>
#include "abstractmockdevice.h"

class MockCP100fx : public AbstractMockDevice
{
    Q_OBJECT
public:
    explicit MockCP100fx(QMutex *mutex, QByteArray *uartBuffer, QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;
    void newDataRecieved() override;

    static QString mockName() {return "offline CP100FX";};

private:
    void initFolders();

    void amtDevCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

};

#endif // MOCKCP1_FX_H
