#ifndef MOCKCP16MODERN_H
#define MOCKCP16MODERN_H

#include <QObject>
#include "abstractmockdevice.h"

#include "hardwarepreset.h"
#include "preset.h"

class MockCP16Modern : public AbstractMockDevice
{
    Q_OBJECT
public:
    explicit MockCP16Modern(QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;

    static QString mockName() {return "offline CP16(MODERN)";};

private:
    QString basePath;

    void initFolders();

    QString currentPresetName;
    preset_data_t currentPresetData;

    bool saveSysParameters();
    bool loadPresetData(quint8 prBank, quint8 prPreset, save_data_t *presetSavedData);
    bool savePresetData(quint8 prBank, quint8 prPreset, const save_data_t *presetSaveData);

    void amtDevCommHandler(const QString &command, const QByteArray& arguments);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments);
    void bankPresetCommHandler(const QString &command, const QByteArray& arguments);
    void outputModeCommHandler(const QString &command, const QByteArray& arguments);
    void pnameCommHandler(const QString &command, const QByteArray& arguments);
    void stateCommHandler(const QString &command, const QByteArray& arguments);
    void getImpulseNameCommHandler(const QString &command, const QByteArray& arguments);
    void getPresetListCommHandler(const QString &command, const QByteArray& arguments);

    void savePresetCommHandler(const QString &command, const QByteArray& arguments);
    void presetChangeCommHandler(const QString &command, const QByteArray& arguments);

    void formatMemoryCommHandler(const QString &command, const QByteArray& arguments);

    void ccCommHandler(const QString &command, const QByteArray& arguments);

    void escAckCommHandler(const QString &command, const QByteArray& arguments);

    // Parameters comm handlers
    QMap<QString, quint8*> paramsMap;
    void setParamsHandler(QString commStr, quint8* commPtr);
    void parametersCommHandler(const QString &command, const QByteArray& arguments);
    void eqParametersCommHandler(const QString &command, const QByteArray& arguments);


    // fw update simulation
    bool fwUpdateMode{false};
    QByteArray fwPart;
    QFile fwFile;
    int fwChunkSize;
    void startFwUpdateCommHandler(const QString &command, const QByteArray& arguments);
};

#endif // MOCKCP16MODERN_H
