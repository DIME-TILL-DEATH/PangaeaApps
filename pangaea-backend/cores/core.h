#ifndef CORE_H
#define CORE_H

#include <QTimer>
#include <QFile>
#include <QFileInfo>

#include <QSettings>

#include "firmware.h"
#include "deviceparameter.h"
#include "answerworker.h"
#include "preset.h"
#include "presetmanager.h"

#include "irworker.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    void setParameter(QString name, quint8 value);
    void setDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value);

    // TODO: пока умеет только Preset и bank!!!! Дописать
    void restoreValue(QString name);
    void readAllParameters();

    void setImpulse(QString fullFilePath);
    void escImpulse();

    void setPresetChange(quint8);

    void saveChanges();

    void comparePreset();
    void slEscImpuls();

    void pushReadPresetCommands();

    void copyPreset();
    void pastePreset();

    void importPreset(QString filePath, QString fileName);
    void exportPreset(QString filePath, QString fileName);

    void setFirmware (QString fullFilePath);
    void uploadFirmware(QByteArray firmware);

    void sw4Enable();

    void stopCore();

private:
    IRWorker irWorker;
    AnswerWorker commandWorker;

    QList<Preset> m_presetsList;

    Device controlledDevice;

    Firmware* deviceFirmware{nullptr};

    bool fwUpdate{false};
    bool isFormatting{false};

    bool isPresetEdited{false};
    Preset currentPreset{&controlledDevice};
    Preset currentSavedPreset{&controlledDevice};
    Preset copiedPreset{&controlledDevice};
    PresetManager presetManager;

    QSettings* appSettings;
    
    QTimer *timeoutTimer;
    QTimer *indicationTimer;
    
    bool recieveEnabled{true};
    quint8 sendCount{0};
    QList<QByteArray> commandsPending;
    QList<QByteArray> commandsSended;
    quint16 commandCount;

    quint32 symbolsToSend{0};
    quint32 symbolsSended{0};
    quint32 bytesToRecieve{0};
    quint32 bytesRecieved{0};

    void pushCommandToQueue(QByteArray);
    void calculateSendVolume();
    void sendCommand(QByteArray);

    void doPresetChange(quint8 val);
    void updateProgressBar();

    bool bEditable;

    void setPresetData(const Preset& preset);
signals:
    void sgWriteToInterface(QByteArray data);
    void sgSilentWriteToInterface(QByteArray data);
    void sgExchangeError();

    void sgFirmwareVersionInsufficient(Firmware *minimalFirmware, Firmware *actualFirmware);
    void sgRequestNewestFirmware(Firmware* actualFirmware);

    void sgSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetUIParameter(QString nameParam, qint32 value);
    void sgSetUIText(QString nameParam, QString value);

    void sgPresetChangeStage(quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);

    void sgRefreshPresetList(QList<Preset>* m_presetsList);
    void sgUpdatePreset(Preset newPreset);

    void sgReadyTodisconnect();
    void sgImmediatelyDisconnect(); // Принудительное после обновления по USB

public slots:
    void slReadyToDisconnect();

    void parseInputData(QByteArray data);
    void processCommands();

    void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");

private slots:
    void indicationRequest();
    void recieveTimeout();
};
#endif // CORE_H
