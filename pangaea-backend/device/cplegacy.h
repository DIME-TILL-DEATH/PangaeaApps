#ifndef CPLEGACY_H
#define CPLEGACY_H

#include "abstractdevice.h"

#include <QSettings>

#include "irworker.h"

#include "preset.h"

#include "presetvolume.h"
#include "compressor.h"
#include "noisegate.h"
#include "poweramp.h"
#include "cabsim.h"
#include "hipassfilter.h"
#include "eqparametric.h"
#include "lowpassfilter.h"

#include "maskedparser.h"

class CPLegacy : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(quint8 outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged FINAL)

    Q_PROPERTY(PresetVolume* MV READ getMV NOTIFY MVChanged FINAL)
public:
    explicit CPLegacy(Core *parent);

public:
    void initDevice(DeviceType deviceType) override;
    void readFullState() override;

    Q_INVOKABLE void saveChanges() override;
    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) override;

    Q_INVOKABLE void comparePreset() override;
    Q_INVOKABLE void copyPreset() override;
    Q_INVOKABLE void pastePreset() override;
    Q_INVOKABLE void importPreset(QString filePath, QString fileName) override;
    Q_INVOKABLE void exportPreset(QString filePath, QString fileName) override;

    Q_INVOKABLE void restoreValue(QString name) override;

    void setImpulse(QString filePath) override;
    Q_INVOKABLE void escImpulse(); // TODO где используется? В мобильном не нашёл вызовов из QML

    quint8 outputMode() const {return m_outputMode;};
    void setOutputMode(quint8 newOutputMode);

    PresetVolume* getMV() {return MV;};

    Q_INVOKABLE void setFirmware(QString fullFilePath) override;
    Q_INVOKABLE void formatMemory() override;

public slots:
     QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

signals:
    void outputModeChanged();

    void MVChanged();
private:
    IRWorker irWorker;


    quint8 m_outputMode;

    // avaliable modules
    PresetVolume* MV;
    Compressor* CM;
    NoiseGate* NG;
    PowerAmp* PA;
    CabSim* IR;
    HiPassFilter* HPF;
    EqParametric* EQ;
    LowPassFilter* LPF;

    QSettings* appSettings;

    void setDeviceType(DeviceType newDeviceType);

    void sendCommandToCP(const QByteArray &command);

    void pushReadPresetCommands();

    void setPresetData(const Preset &preset);
    void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");

    void uploadFirmware(const QByteArray& fwData);

    void amtVerCommHandler(const QString &command, const QByteArray &arguments);
    void getIrNameCommHandler(const QString &command, const QByteArray &arguments);
    void getIrListCommHandler(const QString &command, const QByteArray &arguments);

    void getBankPresetCommHandler(const QString &command, const QByteArray &arguments);
    void getOutputModeCommHandler(const QString &command, const QByteArray &arguments);
    void getStateCommHandler(const QString &command, const QByteArray &arguments);

    void ackEscCommHandler(const QString &command, const QByteArray &arguments);
    void ackSaveChanges(const QString &command, const QByteArray &arguments);
    void ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments);

    // команды cc обрабатывается маскированным парсером
    MaskedParser getIr{"cc\r0\n0\n", "111X1X1"};
    MaskedParser getIrNameSize{"cc\r0\n", "111X1"};
    MaskedParser ackCC{"cc * *\r", "111X1X1"}; //upload save
    MaskedParser endCC{"ccEND\n", "111111"};
    void getIrCommHandler(const QList<QByteArray> &arguments);
    void getIrNameSizeCommHandler(const QList<QByteArray> &arguments);
    void ackCCCommHandler(const QList<QByteArray> &arguments);
    void endCCCommHandler(const QList<QByteArray> &arguments);

    void errorCCCommHandler(const QString &command, const QByteArray &arguments);

    QByteArray m_rawFirmwareData;
    const uint32_t fwUploadBlockSize = 100;
    void requestNextChunkCommHandler(const QString &command, const QByteArray &arguments);
    void fwuFinishedCommHandler(const QString &command, const QByteArray &arguments);
    void formatFinishedCommHandler(const QString &command, const QByteArray &arguments);
};

#endif // CPLEGACY_H
