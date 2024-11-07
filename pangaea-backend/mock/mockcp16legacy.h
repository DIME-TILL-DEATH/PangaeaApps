#ifndef MOCKCP16LEGACY_H
#define MOCKCP16LEGACY_H

#include "abstractmockdevice.h"

class MockCP16Legacy : public AbstractMockDevice
{
public:
    explicit MockCP16Legacy(QObject *parent = nullptr);

private:
    QString basePath;

    void initFolders();


    void amtDevCommHandler(const QString &command, const QByteArray& arguments);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments);
    void bankPresetCommHandler(const QString &command, const QByteArray& arguments);
    void outputModeCommHandler(const QString &command, const QByteArray& arguments);
    void getStateCommHandler(const QString &command, const QByteArray& arguments);
    void reqImpulseName(const QString &command, const QByteArray& arguments);
};

#endif // MOCKCP16LEGACY_H
