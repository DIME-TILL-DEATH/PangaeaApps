#ifndef CPLEGACY_H
#define CPLEGACY_H

#include "abstractdevice.h"

#include "preset.h"
#include "presetmanager.h"

#include "poweramp.h"
#include "presetvolume.h"

class CPLegacy : public AbstractDevice
{
    Q_OBJECT

    Q_PROPERTY(quint8 bank READ bank NOTIFY bankPresetChanged FINAL)
    Q_PROPERTY(quint8 preset READ preset NOTIFY bankPresetChanged FINAL)

    Q_PROPERTY(quint8 outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged FINAL)

    Q_PROPERTY(PresetVolume* MV READ getMV NOTIFY MVChanged FINAL)
public:
    explicit CPLegacy(Core *parent);

public:
    void initDevice() override;

    Q_INVOKABLE void saveChanges();
    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false);

    quint8 bank() const {return m_bank;};
    quint8 preset() const {return m_preset;};

    quint8 outputMode() const {return m_outputMode;};
    void setOutputMode(quint8 newOutputMode);

    PresetVolume* getMV();
public slots:
    void parseAnswers(QByteArray &baAnswer) override;

signals:
    void bankPresetChanged();
    void outputModeChanged();

    void MVChanged();

private:
    quint8 m_bank;
    quint8 m_preset;
    quint8 m_outputMode;

    bool isPresetEdited{false};
    Preset currentPreset;
    Preset currentSavedPreset;
    Preset copiedPreset;
    PresetManager presetManager;

    // avaliable moduls
    PowerAmp* PA;
    PresetVolume* MV;

    void pushReadPresetCommands();
    void sendCommandToCP(const QByteArray &command);

    void bankPresetCommHandler(const QByteArray& arguments);
    void outputModeCommHandler(const QByteArray& arguments);
    void getStateCommHandler(const QByteArray& arguments);
};

#endif // CPLEGACY_H
