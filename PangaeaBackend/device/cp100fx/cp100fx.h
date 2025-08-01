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
#include "mastereq.h"

#include "fswfx.h"
#include "systemsettingsfx.h"

class Cp100fx : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(QString currentPresetName READ currentPresetName WRITE setCurrentPresetName NOTIFY currentPresetNameChanged FINAL)
    Q_PROPERTY(QString currentPresetComment READ currentPresetComment WRITE setCurrentPresetComment NOTIFY currentPresetCommentChanged FINAL)

    Q_PROPERTY(Volume* masterVolume READ masterVolume CONSTANT)
    Q_PROPERTY(Volume* phonesVolume READ phonesVolume CONSTANT)
    Q_PROPERTY(Volume* presetVolume READ presetVolume CONSTANT)
    Q_PROPERTY(Volume* attenuatorVolume READ attenuatorVolume CONSTANT)

    Q_PROPERTY(MasterEq* masterEq READ masterEq CONSTANT)

    Q_PROPERTY(QObjectList fsw READ fswList CONSTANT)
    Q_PROPERTY(SystemSettingsFx* systemSettings READ systemSettings CONSTANT)
    Q_PROPERTY(QObjectList controller READ controller NOTIFY controllersChanged)

    Q_PROPERTY(quint8 cntrlPcOut READ cntrlPcOut WRITE setCntrlPcOut NOTIFY cntrlPcOutChanged FINAL)
    Q_PROPERTY(quint8 cntrlSet READ cntrlSet WRITE setCntrlSet NOTIFY cntrlSetChanged FINAL)
    Q_PROPERTY(quint8 presetVolumeControl READ presetVolumeControl WRITE setPresetVolumeControl NOTIFY presetVolumeControlChanged FINAL)
public:
    Cp100fx(Core *parent);
    ~Cp100fx();

    QStringList strPresetNumbers() override;

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
    MasterEq m_masterEq{this};

    Volume m_masterVolume{this, Volume::VolumeType::MasterFx};
    Volume m_phonesVolume{this, Volume::VolumeType::PhonesFx};
    Volume m_presetVolume{this, Volume::VolumeType::PresetFx};
    Volume m_attenuatorVolume{this, Volume::VolumeType::AttenuatorFx};

    Volume* masterVolume() {return &m_masterVolume;};
    Volume* phonesVolume() {return &m_phonesVolume;};
    Volume* presetVolume() {return &m_presetVolume;};
    Volume* attenuatorVolume() {return &m_attenuatorVolume;};

    MasterEq* masterEq() {return &m_masterEq;};

    QString currentPresetName() const;
    void setCurrentPresetName(const QString &newCurrentPresetName);

    QString currentPresetComment() const;
    void setCurrentPresetComment(const QString &newCurrentPresetComment);

    QObjectList fswList() {return m_fswList;};
    SystemSettingsFx* systemSettings() {return &m_systemSettings;};

    QObjectList controller() {return m_actualControllersList;};

    quint8 cntrlPcOut() const {return actualPreset.cntrlPcOut();};
    void setCntrlPcOut(quint8 newCntrlPcOut);

    quint8 cntrlSet() const {return actualPreset.cntrlSet();};
    void setCntrlSet(quint8 newCntrlSet);

    quint8 presetVolumeControl() const {return actualPreset.presetData.volume_control;};
    void setPresetVolumeControl(quint8 newPresetVolumeControl);

public slots:
    QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

signals:
    void currentPresetNameChanged();
    void currentPresetCommentChanged();
    void systemSettingsChanged();
    void controllersChanged();

    void cntrlPcOutChanged();
    void cntrlSetChanged();
    void presetVolumeControlChanged();

private:
    QList<PresetAbstract*> m_presetsList;

    SystemSettingsFx m_systemSettings{this};
    QObjectList m_fswList;
    QObjectList m_actualControllersList;

    FswFx m_fswDown{0, this};
    FswFx m_fswConfirm{1, this};
    FswFx m_fswUp{2, this};

    PresetFx actualPreset{this};
    PresetFx savedPreset{this}; // TODO используется из листа
    PresetFx copiedPreset{this};

    void pushReadPresetCommands();

    void amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void sysSettingsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void getPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void ackPresetSavedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void plistCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void pnumCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pcommentCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void cntrlsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void cntrlPcOutCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void cntrlSetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
};

#endif // CP100FX_H
