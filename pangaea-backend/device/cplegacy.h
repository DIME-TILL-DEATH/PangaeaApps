#ifndef CPLEGACY_H
#define CPLEGACY_H

#include "abstractdevice.h"

#include "preset.h"
#include "presetmanager.h"

#include "presetvolume.h"
#include "noisegate.h"
#include "poweramp.h"
#include "cabsim.h"
#include "eqparametric.h"

class CPLegacy : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(quint8 outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged FINAL)

    Q_PROPERTY(PresetVolume* MV READ getMV NOTIFY MVChanged FINAL)
public:
    explicit CPLegacy(Core *parent);

public:
    void initDevice(DeviceType deviceType) override;

    Q_INVOKABLE void saveChanges() override;
    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) override;
    Q_INVOKABLE void comparePreset();
    Q_INVOKABLE void escImpulse(); // TODO где используется?

    quint8 outputMode() const {return m_outputMode;};
    void setOutputMode(quint8 newOutputMode);

    PresetVolume* getMV();

signals:
    void outputModeChanged();

    void MVChanged();
private:
    quint8 m_outputMode;

    // avaliable modules
    PresetVolume* MV;
    NoiseGate* NG;
    PowerAmp* PA;
    CabSim* IR;
    EqParametric* EQ;

    void sendCommandToCP(const QByteArray &command);

    void pushReadPresetCommands();
    void setPresetData(const Preset &preset);

    void getIrNameCommHandler(const QString &command, const QByteArray &arguments);
    void getIrListCommHandler(const QString &command, const QByteArray &arguments);

    void bankPresetCommHandler(const QString &command, const QByteArray &arguments);
    void outputModeCommHandler(const QString &command, const QByteArray &arguments);
    void getStateCommHandler(const QString &command, const QByteArray &arguments);

    void ackEscCommHandler(const QString &command, const QByteArray &arguments);
    void ackSaveChanges(const QString &command, const QByteArray &arguments);
    void ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments);
};

#endif // CPLEGACY_H
