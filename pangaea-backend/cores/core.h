#ifndef CORE_H
#define CORE_H

#include <QTimer>
#include <QFile>
#include <QFileInfo>

#include <QSettings>

#include "firmware.h"

#include "answerworker.h"
#include "preset.h"
#include "devicedescription.h"

#include "abstractdevice.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);


    void setFirmware (QString fullFilePath);
    void uploadFirmware(const QByteArray &firmware);

    QTimer *timeoutTimer;

signals:
    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgExchangeError();

    void sgFirmwareVersionInsufficient(Firmware *minimalFirmware, Firmware *actualFirmware);
    void sgRequestNewestFirmware(Firmware* actualFirmware);

    void sgCurrentDeviceChanged(AbstractDevice* device);

    void sgSetUIParameter(QString nameParam, qint32 value);
    void sgSetUIText(QString nameParam, QString value);

    void sgSetProgress(float val, QString extText);

    void sgReadyTodisconnect();
    void sgImmediatelyDisconnect(); // Принудительное после обновления

public slots:
    void slDeviceInstanciated();

    void disconnectFromDevice();
    void slInterfaceConnected(DeviceDescription interfaceDescription);

    void parseInputData(QByteArray data);
    void processCommands();

private slots:
    void recieveTimeout();

private:
    AnswerWorker commandWorker;

    const uint32_t fwUploadBlockSize = 100;
    QByteArray m_rawFirmwareData;

    AbstractDevice* currentDevice{nullptr};

    bool fwUpdate{false};
    bool isFormatting{false};

    QSettings* appSettings;
    
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
};
#endif // CORE_H
