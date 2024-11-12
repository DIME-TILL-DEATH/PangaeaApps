#ifndef MOCKCP16LEGACY_H
#define MOCKCP16LEGACY_H

#include "abstractmockdevice.h"

#include "preset.h"

#define FIRMWARE_STRING_SIZE 20
typedef struct system_parameters_t
{
    uint8_t empty[2];
    uint8_t output_mode;
    uint8_t reserved[8];
    uint8_t eol_symb = '\n';
    char firmware_version[FIRMWARE_STRING_SIZE];
#ifdef __LA3_MOD__
    uint8_t la3_cln_preset;
    uint8_t la3_drv_preset;
#endif
}system_parameters_t;

class MockCP16Legacy : public AbstractMockDevice
{
public:
    explicit MockCP16Legacy(QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;

private:
    QString basePath;

    void initFolders();

    preset_data_legacy_t currentPresetData;

    bool saveSysParameters();
    bool loadPresetData(quint8 prBank, quint8 prPreset, preset_data_legacy_t* presetData);
    bool savePresetData(quint8 prBank, quint8 prPreset, const preset_data_legacy_t* presetData);

    void amtDevCommHandler(const QString &command, const QByteArray& arguments);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments);
    void bankPresetCommHandler(const QString &command, const QByteArray& arguments);
    void outputModeCommHandler(const QString &command, const QByteArray& arguments);
    void getStateCommHandler(const QString &command, const QByteArray& arguments);
    void getImpulseNameCommHandler(const QString &command, const QByteArray& arguments);
    void getRnsCommHandler(const QString &command, const QByteArray& arguments);

    void savePresetCommHandler(const QString &command, const QByteArray& arguments);
    void presetChangeCommHandler(const QString &command, const QByteArray& arguments);

    void formatMemoryCommHandler(const QString &command, const QByteArray& arguments);

    void ccCommHandler(const QString &command, const QByteArray& arguments);

    void escAckCommHandler(const QString &command, const QByteArray& arguments);

    // Parameters comm handlers
    QMap<QString, quint8*> paramsMap;
    void setParamsHandler(QString commStr, quint8* commPtr);
    void setEqHandler(QString commStr, quint8* commPtr);
    void parametersCommHandler(const QString &command, const QByteArray& arguments);

    void eqParametersCommHandler(const QString &command, const QByteArray& arguments);

    // fw update simulation
    bool fwUpdateMode{false};
    QByteArray fwPart;
    QFile fwFile;
    int fwChunkSize;
    void startFwUpdateCommHandler(const QString &command, const QByteArray& arguments);
};

#endif // MOCKCP16LEGACY_H
