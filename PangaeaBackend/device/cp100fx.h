#ifndef CP100FX_H
#define CP100FX_H

#include <QObject>

#include "abstractdevice.h"
#include "presetfx.h"

#include "resonancefilter.h"
#include "compressor.h"
#include "noisegate.h"
#include "preamp.h"
#include "poweramp.h"
#include "dualcabsim.h"
#include "eqparametric.h"
#include "tremolo.h"
#include "chorus.h"
#include "phaser.h"
#include "earlyreflections.h"
#include "delay.h"
#include "reverb.h"
#include "flanger.h"

class Cp100fx : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(QString currentPresetName READ currentPresetName WRITE setCurrentPresetName NOTIFY currentPresetNameChanged FINAL)
    Q_PROPERTY(QString currentPresetComment READ currentPresetComment WRITE setCurrentPresetComment NOTIFY currentPresetCommentChanged FINAL)

    Q_PROPERTY(Volume* masterVolume READ masterVolume CONSTANT)
    Q_PROPERTY(Volume* phonesVolume READ phonesVolume CONSTANT)
    Q_PROPERTY(Volume* presetVolume READ presetVolume CONSTANT)
    Q_PROPERTY(Volume* attenuatorVolume READ attenuatorVolume CONSTANT)
public:
    Cp100fx(Core *parent);
    ~Cp100fx();

    void updateOutputModeNames() override;

    quint16 processingUsed() override;

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

    Q_INVOKABLE void previewIr(QString srcFilePath) override;
    Q_INVOKABLE void startIrUpload(QString srcFilePath, QString dstFilePath = "", bool trimFile = false) override;

    Q_INVOKABLE void setFirmware(QString fullFilePath) override;
    Q_INVOKABLE void formatMemory() override;

    ResonanceFilter* RF;
    Compressor* CM;
    NoiseGate* NG;
    Preamp* PR;
    PowerAmp* PA;
    DualCabSim* IR;
    EqParametric* EQ;
    Delay* DL;
    Phaser* PH;
    Chorus* CH;
    Tremolo* TR;
    EarlyReflections* ER;
    Reverb* RV;
    Flanger* FL;

    Volume m_masterVolume{this, Volume::VolumeType::MasterFx};
    Volume m_phonesVolume{this, Volume::VolumeType::PhonesFx};
    Volume m_presetVolume{this, Volume::VolumeType::PresetFx};
    Volume m_attenuatorVolume{this, Volume::VolumeType::AttenuatorFx};

    Volume* masterVolume() {return &m_masterVolume;};
    Volume* phonesVolume() {return &m_phonesVolume;};
    Volume* presetVolume() {return &m_presetVolume;};
    Volume* attenuatorVolume() {return &m_attenuatorVolume;};

    QString currentPresetName() const {return m_currentPresetName;};
    void setCurrentPresetName(const QString &newCurrentPresetName);

    QString currentPresetComment() const {return m_currentPresetComment;};
    void setCurrentPresetComment(const QString &newCurrentPresetComment);

public slots:
    QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

signals:
    void currentPresetNameChanged();
    void currentPresetCommentChanged();

private:
    QList<PresetAbstract*> m_presetsList;

    PresetFx actualPreset{this};
    PresetFx savedPreset{this}; // TODO используется из листа
    PresetFx copiedPreset{this};

    void pushReadPresetCommands();

    void amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void getPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    QString m_currentPresetName;
    QString m_currentPresetComment;
};

#endif // CP100FX_H
