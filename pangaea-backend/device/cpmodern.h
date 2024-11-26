#ifndef CPMODERN_H
#define CPMODERN_H

#include <QObject>
#include "abstractdevice.h"

#include <QSettings>

#include "irworker.h"

#include "preset.h"

#include "presetvolume.h"
#include "compressor.h"
#include "noisegate.h"
#include "preamp.h"
#include "poweramp.h"
#include "cabsim.h"
#include "eqparametric.h"
#include "earlyreflections.h"


class CPModern : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(PresetVolume* MV READ getMV CONSTANT)
    Q_PROPERTY(QString currentPresetName READ currentPresetName WRITE setCurrentPresetName NOTIFY currentPresetNameChanged FINAL)
public:
    CPModern(Core *parent);

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


    PresetVolume* getMV() {return MV;};

    QString currentPresetName() const {return actualPreset.presetName();};
    void setCurrentPresetName(const QString &newCurrentPresetName);

    Q_INVOKABLE void setFirmware(QString fullFilePath) override;
    Q_INVOKABLE void formatMemory() override;

    // avaliable modules
    // вынести создание наружу? DependencyContainer
    // public для тестов
    PresetVolume* MV;
    Compressor* CM;
    NoiseGate* NG;
    Preamp* PR;
    PowerAmp* PA;
    CabSim* IR;
    EqParametric* EQ;
    EarlyReflections* ER;


public slots:
    QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

signals:

    void currentPresetNameChanged();

private:
    IRWorker irWorker;
    QSettings* appSettings;

    void setDeviceType(DeviceType newDeviceType);

    void pushReadPresetCommands();

    void setPresetData(const Preset &preset);
    void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");

    void uploadFirmware(const QByteArray& fwData);

    void amtVerCommHandler(const QString &command, const QByteArray &arguments);
    void getIrNameCommHandler(const QString &command, const QByteArray &arguments);
    void irCommHandler(const QString &command, const QByteArray &arguments);
    void getPresetListCommHandler(const QString &command, const QByteArray &arguments);

    void getBankPresetCommHandler(const QString &command, const QByteArray &arguments);
    void getOutputModeCommHandler(const QString &command, const QByteArray &arguments);
    void pnameCommHandler(const QString &command, const QByteArray &arguments);
    void stateCommHandler(const QString &command, const QByteArray &arguments);

    void ackEscCommHandler(const QString &command, const QByteArray &arguments);
    void ackSaveChanges(const QString &command, const QByteArray &arguments);
    void ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments);

    void getCCCommHandler(const QList<QByteArray> &arguments);
    void getCCNameSizeCommHandler(const QList<QByteArray> &arguments);
    void ackCCCommHandler(const QList<QByteArray> &arguments);
    void endCCCommHandler(const QList<QByteArray> &arguments);

    void errorCCCommHandler(const QString &command, const QByteArray &arguments);

    QByteArray m_rawFirmwareData;
    const uint32_t fwUploadBlockSize = 100;
    void requestNextChunkCommHandler(const QString &command, const QByteArray &arguments);
    void fwuFinishedCommHandler(const QString &command, const QByteArray &arguments);
    void formatFinishedCommHandler(const QString &command, const QByteArray &arguments);
};

#endif // CPMODERN_H
