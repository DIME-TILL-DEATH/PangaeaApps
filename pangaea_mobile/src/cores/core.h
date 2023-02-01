#ifndef CORE_H
#define CORE_H

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QTimer>
#include <QFile>
#include <QFileInfo>

#include <QSettings>

#include "bluetoothleuart.h"

#include "firmware.h"

#include "irworker.h"

#include "devicecontrols.h"
#include "answerworker.h"

#include "device.h"
#include "preset.h"
#include "presetlistmodel.h"

#include "presetmanager.h"

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(BluetoothleUART* bleConnection, QObject *parent = nullptr);
    ~Core();

    void registerQmlObjects(QQmlContext* qmlContext);

    void setParameter(QString name, quint8 val);
    // TODO: пока умеет только Preset и bank!!!! Дописать
    void restoreValue(QString name);
    void readAllParameters();

    void setPresetChange(quint8);
    void readPresetState();
    void saveChanges();

    void copyPreset();
    void pastePreset();
    void comparePreset();

    void importPreset(QString filePath, QString fileName);
    void exportPreset(QString filePath, QString fileName);

    void setImpulse (QString filePath, QString fileName);
    void escImpulse();

    void setFirmware (QString fullFilePath);
    void uploadFirmware(QByteArray firmware);

    void sw4Enable();

    void stopTimer();    

    void setPresetData(const Preset& preset);

private:
    IRWorker irWorker;
    DeviceControls deviceControls;
    AnswerWorker commandWorker;

    QList<Preset> m_presetsList;
    PresetListModel m_presetListModel;

    BluetoothleUART* m_bleConnection;
    // TODO проверять в интерфейсе
    bool isConnected{false};

    Device controlledDevice;

    Firmware* deviceFirmware{nullptr};

    QSettings* appSettings;

    bool fwUpdate{false};
    bool isFormatting{false};

    bool isPresetEdited{false};
    Preset currentPreset{&controlledDevice};
    Preset copiedPreset{&controlledDevice};
    PresetManager presetManager;

    QList<QString> m_qlFoundDevices;
    QList<QString> m_qlFoundAddress;

    QTimer *timer;

    bool enableRecieve{true};
    quint8 sendCount{0};
    QList<QByteArray> commandsPending;
    QList<QByteArray> commandsSended;

    quint32 symbolsToSend{0};
    quint32 symbolsSended{0};
    quint32 bytesToRecieve{0};
    quint32 bytesRecieved{0};

    void saveAndUpdatePreset();

    void pushCommandToQueue(QByteArray val);
    void calculateSendVolume();
    void sendCommand(QByteArray val);

    void doPresetChange(quint8 val);
    void updateProgressBar();

signals:
    void sgUpdateBLEDevicesList(QStringList strList);
    void sgConnectReady(void);

    void sgFirmwareVersionInsufficient(Firmware *minimalFirmware, Firmware *actualFirmware);
    void sgRequestNewestFirmware(Firmware* actualFirmware);

    void sgSetUIParameter(QString nameParam, qint32 inValue);
    void sgSetUIText(QString nameParam, QString value);

    void sgPresetChangeStage(quint8 inChangePreset);
    void sgSetProgress(float val, QString extText);

    void sgModuleNameUpdated(QString name);

    void sgRefreshPresetList(QList<Preset>* m_presetsList);

private slots:
    void slChangedState(BluetoothleUART::bluetoothleState state);
    void slReadyToDisconnect();

    void parseInputData(const QByteArray &ba);
    void processCommands();
    void recieveTimeout();

    void uploadImpulseData(const QByteArray& impulseData, bool isPreview, QString impulseName = "");
};
#endif // CORE_H
