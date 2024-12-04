#ifndef CPLEGACY_H
#define CPLEGACY_H

#include "abstractdevice.h"

#include <QSettings>

#include "irworker.h"

#include "presetlegacy.h"

#include "presetvolume.h"
#include "compressor.h"
#include "noisegate.h"
#include "preamp.h"
#include "poweramp.h"
#include "cabsim.h"
#include "hipassfilter.h"
#include "eqparametric.h"
#include "lowpassfilter.h"
#include "earlyreflections.h"

#include "maskedparser.h"



class CPLegacy : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(PresetVolume* MV READ getMV NOTIFY MVChanged FINAL)
    Q_PROPERTY(bool isPaFw READ isPaFw CONSTANT)
    Q_PROPERTY(bool isPreEq READ isPreEq WRITE setIsPreEq NOTIFY isPreEqChanged FINAL)
public:
    explicit CPLegacy(Core *parent);
    ~CPLegacy();

    void updateOutputModeNames() override;

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

    void startIrUpload(QString srcFilePath, QString dstFilePath = "") override;
    Q_INVOKABLE void escImpulse(); // TODO где используется? В мобильном не нашёл вызовов из QML

    quint64 maxIrSize() override {return 984 * 3 + 44;};

    PresetVolume* getMV() {return MV;};

    Q_INVOKABLE void setFirmware(QString fullFilePath) override;
    Q_INVOKABLE void formatMemory() override;

    bool isPaFw() const {return m_isPaFw;};
    bool isPreEq() const {return m_isPreEq;};
    void setIsPreEq(bool newIsPreEq);

    // avaliable modules
    // вынести создание наружу?
    // public для тестов
    PresetVolume* MV;
    Compressor* CM;
    NoiseGate* NG;
    Preamp* PR;
    PowerAmp* PA;
    CabSim* IR;
    HiPassFilter* HPF;
    EqParametric* EQ;
    LowPassFilter* LPF;
    EarlyReflections* ER;
public slots:
     QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

    void slIrEnabledChanged();

signals:

    void MVChanged();
    void isPreEqChanged();

private:
    IRWorker irWorker;

    QList<PresetAbstract*> m_presetsList;
    PresetLegacy actualPreset{this};
    PresetLegacy savedPreset{this}; // TODO используется из листа
    PresetLegacy copiedPreset{this};

    bool m_isPreEq{false};
    bool m_isPaFw{false};

    QSettings* appSettings;

    void setDeviceType(DeviceType newDeviceType);

    void pushReadPresetCommands();


    void setPresetData(const PresetLegacy &preset);
    void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");

    void uploadFirmware(const QByteArray& fwData);

    void arrangePrePost();

    void amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getIrNameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getIrListCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void getBankPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getOutputModeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getStateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void ackEscCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void ackSaveChanges(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    // команды cc обрабатывается маскированным парсером
    MaskedParser getIr{"cc\r0\n0\n", "111X1X1"};
    MaskedParser getIrNameSize{"cc\r0\n", "111X1"};
    MaskedParser ackCC{"cc * *\r", "111X1X1"}; //upload save
    MaskedParser endCC{"ccEND\n", "111111"};
    void getIrCommHandler(const QList<QByteArray> &arguments);
    void getIrNameSizeCommHandler(const QList<QByteArray> &arguments);
    void ackCCCommHandler(const QList<QByteArray> &arguments);
    void endCCCommHandler(const QList<QByteArray> &arguments);

    void errorCCCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    QByteArray m_rawFirmwareData;
    const uint32_t fwUploadBlockSize = 100;
    void requestNextChunkCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void fwuFinishedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void formatFinishedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
};

#endif // CPLEGACY_H
