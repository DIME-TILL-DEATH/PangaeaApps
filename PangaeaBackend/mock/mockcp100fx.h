#ifndef MOCKCP100_FX_H
#define MOCKCP100_FX_H

#include <QObject>
#include "abstractmockdevice.h"

#include "hardwarefxpreset.h"

class MockCP100fx : public AbstractMockDevice
{
    Q_OBJECT
public:
    explicit MockCP100fx(QMutex *mutex, QByteArray *uartBuffer, QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;
    void newDataRecieved() override;

    static QString mockName() {return "offline CP100FX";};

    preset_data_fx_t currentPresetData;
    preset_data_fx_t defaultPresetData;

    system_parameters_fx_t currentSystemData;
    system_parameters_fx_t defaultSystemData;

    // uint8_t __CCM_BSS__ sys_para[512] = {/*mode*/0,/*midi_ch*/0,/*cab num*/0,/*exp_type*/1,/*foot1*/0,/*foot2*/0,
    //                                      /*foot3*/0,/*calibrate*/0,0,0xff,0xf};
private:
    QString presetName;
    QString presetComment;

    quint32 delayTime;

    QMap<QString, quint8*> paramsByteMap;
    QMap<QString, quint16*> paramsWordMap;

    void initFolders();
    bool saveSysParameters();
    void loadPresetData(quint8 presetNumber);
    void savePresetData(quint8 prPreset);

    void setParamsHandler(QString commStr, quint8 *commPtr);
    void setParamsHandler(QString commStr, quint16 *commPtr);
    void parametersByteCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void parametersWordCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void amtDevCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    static constexpr quint8 stringDataSize = 15;
};

#endif // MOCKCP100_FX_H
