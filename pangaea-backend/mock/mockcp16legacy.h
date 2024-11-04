#ifndef MOCKCP16LEGACY_H
#define MOCKCP16LEGACY_H

#include "abstractmockdevice.h"

class MockCP16Legacy : public AbstractMockDevice
{
public:
    explicit MockCP16Legacy(QObject *parent = nullptr);

private:
    void amtDevCommHandler(const QByteArray& arguments);
    void amtVerCommHandler(const QByteArray& arguments);
    void bankPresetCommHandler(const QByteArray& arguments);
    void outputModeCommHandler(const QByteArray& arguments);
    void getStateCommHandler(const QByteArray& arguments);
    void reqImpulseName(const QByteArray& arguments);
};

#endif // MOCKCP16LEGACY_H
