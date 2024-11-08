#ifndef CORE_H
#define CORE_H

#include <QTimer>
#include <QFile>
#include <QFileInfo>

#include <QSettings>

#include "devicedescription.h"
#include "abstractdevice.h"

#include "maskedparser.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    QTimer *timeoutTimer;

signals:
    void sgWriteToInterface(QByteArray data, bool logCommand = true);
    void sgExchangeError();

    void sgRequestNewestFirmware(Firmware* actualFirmware);

    void sgCurrentDeviceChanged(AbstractDevice* device);

    void sgSetUIParameter(QString nameParam, qint32 value);
    void sgSetUIText(QString nameParam, QString value);

    void sgSetProgress(float val, QString extText);

    void sgReadyToDisconnect();
    void sgImmediatelyDisconnect(); // Принудительное после обновления

public slots:
    void slDeviceInstanciated();

    void disconnectFromDevice();
    void slInterfaceConnected(DeviceDescription interfaceDescription);

    void parseInputData(QByteArray data);
    void pushCommandToQueue(QByteArray command, bool finalize = true);
    void sendWithoutConfirmation(QByteArray data, qint64 dataSizeToSend = -1, qint64 dataSizeTorecieve = -1);
    void processCommands();

private slots:
    void recieveTimeout();

private:
    MaskedParser amtDevParser{"amtdev\rX\n", "1111111X1"};

    AbstractDevice* currentDevice{nullptr};

    DeviceConnectionType m_currentConnectionType;
    
    quint8 sendCount{0};
    QList<QByteArray> commandsPending;
    QList<QByteArray> commandsSended;
    quint16 commandCount;

    quint32 symbolsToSend{0};
    quint32 symbolsSended{0};
    quint32 symbolsToRecieve{0};
    quint32 symbolsRecieved{0};

    void calculateSendVolume();
    void updateProgressBar();
    void sendCommand(QByteArray);
};
#endif // CORE_H
