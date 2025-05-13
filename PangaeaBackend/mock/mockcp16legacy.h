#ifndef MOCKCP16LEGACY_H
#define MOCKCP16LEGACY_H

#include "abstractmockdevice.h"

#include "presetlegacy.h"
#include "hardwareclassicpreset.h"


class MockCP16Legacy : public AbstractMockDevice
{
public:
    explicit MockCP16Legacy(QMutex* mutex, QByteArray* uartBuffer, QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;
    void newDataRecieved() override;

    static QString mockName() {return "offline CP16(LEGACY)";};

private:

    void initFolders();

    preset_cplegacy_t currentPresetData;

    bool saveSysParameters();
    bool loadPresetData(quint8 prBank, quint8 prPreset, preset_cplegacy_t* presetData);
    bool savePresetData(quint8 prBank, quint8 prPreset, const preset_cplegacy_t* presetData);

    void amtDevCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void bankPresetCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void outputModeCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void getStateCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void getImpulseNameCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void getRnsCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void savePresetCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void presetChangeCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void formatMemoryCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void ccCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void lccCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void escAckCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    // Parameters comm handlers
    QMap<QString, quint8*> paramsMap;
    void setParamsHandler(QString commStr, quint8* commPtr);
    void setEqHandler(QString commStr, quint8* commPtr);
    void parametersCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void eqParametersCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    // fw update simulation
    bool fwUpdateMode{false};
    QByteArray fwPart;
    QFile fwFile;
    int fwChunkSize;
    void startFwUpdateCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
};

#endif // MOCKCP16LEGACY_H
