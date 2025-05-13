#ifndef CP1__FX_H
#define CP1__FX_H

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

public slots:
    QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

private:
    QList<PresetAbstract*> m_presetsList;

    PresetFx actualPreset{this};
    PresetFx savedPreset{this}; // TODO используется из листа
    PresetFx copiedPreset{this};

    void amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void getPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

};

#endif // CP1__FX_H
