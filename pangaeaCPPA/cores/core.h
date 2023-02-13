#ifndef CORE_H
#define CORE_H

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QTimer>
#include <QFile>
#include <QFileInfo>

#include <QSettings>

#include "firmware.h"

#include "answerworker.h"
#include "devicecontrols.h"
#include "preset.h"
#include "presetlistmodel.h"
#include "presetmanager.h"

#include "irworker.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    ~Core();

    void setParameter(QString name, quint8 value);
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

    void setPresetData(const Preset& preset);

private:
    IRWorker irWorker;
    DeviceControls deviceControls;
    AnswerWorker commandWorker;

    QList<Preset> m_presetsList;
    PresetListModel m_presetListModel;

    Device controlledDevice;

    Firmware* deviceFirmware{nullptr};

    bool fwUpdate{false};
    bool isFormatting{false};

    bool isPresetEdited{false};
    Preset currentPreset{&controlledDevice};
    Preset copiedPreset{&controlledDevice};
    PresetManager presetManager;

    QSettings* appSettings;

    QTimer *timer;

    bool enableRecieve{true};
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

signals:
    void sgWriteToInterface(QByteArray data);

    void sgFirmwareVersionInsufficient(Firmware *minimalFirmware, Firmware *actualFirmware);
    void sgRequestNewestFirmware(Firmware* actualFirmware);

    void sgSetUIParameter(QString nameParam, qint32 value);
    void sgSetUIText(QString nameParam, QString value);
//    void sgSetUIDataList(QString nameParam, QVariantList list);

    void sgPresetChangeStage(quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);

//    void sgModuleNameUpdated(QString name);

    void sgRefreshPresetList(QList<Preset>* m_presetsList);

public slots:
    void slReadyToDisconnect();

    void parseInputData(const QByteArray &data);
    void processCommands();
    void recieveTimeout();

    void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");
};

#endif // CORE_H
