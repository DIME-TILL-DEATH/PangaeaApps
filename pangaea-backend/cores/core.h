#ifndef CORE_H
#define CORE_H

#include <QTimer>
#include <QFile>
#include <QFileInfo>

#include <QSettings>

#include "firmware.h"

#include "answerworker.h"
#include "preset.h"
#include "presetmanager.h"
#include "devicedescription.h"
#include "deviceparameter.h"

#include "irworker.h"

#include "abstractdevice.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    enum AppParameter
    {
        PRESET_MODIFIED,
        COMPARE_STATE,
        FW_UPDATE_COMPLITED
    };
    Q_ENUM(AppParameter)

    enum AppAction
    {
        CHANGE_PRESET,
        COPY_PRESET,
        PASTE_PRESET,
        COMPARE_PRESET,
        SAVE_CHANGES,
        FORMAT_FLASH,
        SET_LA3_MAPPINGS
        // CALL_PRESET_LIST
    };
    Q_ENUM(AppAction)

    // TODO: пока умеет только Preset и bank!!!! Дописать
    // void restoreValue(QString name);

    void setImpulse(QString fullFilePath);
    // void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");
    // void escImpulse();

    // void copyPreset();
    // void pastePreset();

    // void importPreset(QString filePath, QString fileName);
    // void exportPreset(QString filePath, QString fileName);

    void setFirmware (QString fullFilePath);
    void uploadFirmware(const QByteArray &firmware);

private:
    IRWorker irWorker;
    AnswerWorker commandWorker;

    QByteArray lastRecievedData;

    const uint32_t fwUploadBlockSize = 100;
    QByteArray m_rawFirmwareData;


    Device controlledDevice;
    AbstractDevice* currentDevice;

    Firmware* deviceFirmware{nullptr};

    bool fwUpdate{false};
    bool isFormatting{false};

    // bool isPresetEdited{false};
    // Preset currentPreset;
    // Preset currentSavedPreset;
    // Preset copiedPreset;
    PresetManager presetManager;

    QSettings* appSettings;
    
    QTimer *timeoutTimer;
    
    bool recieveEnabled{true};
    quint8 sendCount{0};
    QList<QByteArray> commandsPending;
    QList<QByteArray> commandsSended;
    quint16 commandCount;

    quint32 symbolsToSend{0};
    quint32 symbolsSended{0};
    quint32 bytesToRecieve{0};
    quint32 bytesRecieved{0};

    void pushCommandToQueue(QByteArray command, bool finalize = true);
    void calculateSendVolume();
    void updateProgressBar();
    void sendCommand(QByteArray);

signals:
    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgExchangeError();

    void sgFirmwareVersionInsufficient(Firmware *minimalFirmware, Firmware *actualFirmware);
    void sgRequestNewestFirmware(Firmware* actualFirmware);

    void sgCurrentDeviceChanged(AbstractDevice* device);

    void sgRecieveDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value);
    void sgSetAppParameter(AppParameter appParameterType, QVariant content);
    void sgSetUIParameter(QString nameParam, qint32 value);
    void sgSetUIText(QString nameParam, QString value);

    void sgSetProgress(float val, QString extText);

    void sgReadyTodisconnect();
    void sgImmediatelyDisconnect(); // Принудительное после обновления по USB

public slots:
    void slDeviceInstanciated(DeviceType deviceType);

    void disconnectFromDevice();
    void slInterfaceConnected(DeviceDescription interfaceDescription);

    void parseInputData(QByteArray data);
    void processCommands();


    void slRecieveAppAction(AppAction appParameterType, QVariantList parameters);

private slots:
    // void indicationRequest();
    void recieveTimeout();
};
#endif // CORE_H
