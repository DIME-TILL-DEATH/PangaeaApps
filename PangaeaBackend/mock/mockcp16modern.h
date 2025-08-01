#ifndef MOCKCP16MODERN_H
#define MOCKCP16MODERN_H

#include <QObject>
#include "abstractmockdevice.h"

#include "hardwareclassicpreset.h"
#include "presetlegacy.h"

#include "irfile.h"

class MockCP16Modern : public AbstractMockDevice
{
    Q_OBJECT
public:
    explicit MockCP16Modern(QMutex* mutex, QByteArray* uartBuffer, QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;
    void newDataRecieved() override;

    static QString mockName() {return "offline CP16(MODERN)";};

    // public for testing purposes
    IrFile linkedIr;
    QString currentPresetName;
    preset_data_cpmodern_t currentPresetData;
protected:
    QString m_mockName{"virtual_CP16Modern"};

    QFile uploadingIrFile;

    system_parameters_cp_t m_systemParameters{0};

    quint8 m_outputMode{0};

    void initFolders();

    bool saveSysParameters();
    bool loadPresetData(quint8 prBank, quint8 prPreset, save_data_cpmodern_t *presetSavedData);
    bool savePresetData(quint8 prBank, quint8 prPreset, const save_data_cpmodern_t *presetSaveData);

    void getIrInfo();
    void irDownload(const QString& pathToIr);

    void changePreset(quint8 bank, quint8 preset);

    void amtDevCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void bankPresetCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void outputModeCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void pnameCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void irCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void lccCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void listCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void mconfigCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void rvconfigCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void getPresetListCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void savePresetCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void presetChangeCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void copyCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void formatMemoryCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void escAckCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    // Parameters comm handlers
    QMap<QString, quint8*> paramsByteMap;
    QMap<QString, quint16*> paramsWordMap;
    void setParamsHandler(QString commStr, quint8* commPtr);
    void setParamsHandler(QString commStr, quint16* commPtr);
    void parametersByteCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void parametersWordCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void eqParametersCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);


    // fw update simulation
    bool fwUpdateMode{false};
    QByteArray fwPart;
    QFile fwFile;
    int fwChunkSize;
    void startFwUpdateCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
};

#endif // MOCKCP16MODERN_H
