#include <QThread>
#include <QSerialPortInfo>
#include <QDir>
#include <QStandardPaths>

#include <QFile>

#include <QtGui/QGuiApplication>

#include "core.h"

#include "cplegacy.h"
#include "cpmodern.h"

Core::Core(QObject *parent) : QObject(parent)
{
    timeoutTimer = new QTimer(this);
    connect(timeoutTimer, &QTimer::timeout, this, &Core::recieveTimeout);
}

void Core::disconnectFromDevice()
{
    qDebug() << Q_FUNC_INFO << "disconnected";

    timeoutTimer->stop();

    commandsPending.clear();
    commandsSended.clear();

    if(currentDevice)
    {
        currentDevice->deleteLater();
        currentDevice = nullptr;
    }
    emit sgReadyToDisconnect();
}

void Core::slInterfaceConnected(DeviceDescription interfaceDescription)
{
    m_currentConnectionType = interfaceDescription.connectionType();

    timeoutTimer->start(1000);

    pushCommandToQueue("amtdev");
    processCommands();
}

void Core::parseInputData(QByteArray ba)
{
    qInfo() << "->" << __FUNCTION__ << ":" << ba;

    if(symbolsToRecieve > 0)
    {
        symbolsRecieved += ba.size()/2; // 1 byte = 2 symbols, 0xFF
    }
    else symbolsRecieved = 0;

    updateProgressBar();

    QList<QByteArray> recievedAnswer;
    if(currentDevice)
    {
        recievedAnswer = currentDevice->parseAnswers(ba);
    }

    QList<QByteArray> parseResult;
    if(amtDevParser.getParse(ba, &parseResult))
    {
        if(currentDevice)
        {
            delete(currentDevice);
            currentDevice = nullptr;
        }

        DeviceType deviceType = static_cast<DeviceType>(parseResult.at(0).toUInt());
        if(deviceType < DeviceType::LEGACY_DEVICES)
        {
            currentDevice = new CPLegacy(this);

        }
        else
        {
            currentDevice = new CPModern(this);
        }
        connect(currentDevice, &AbstractDevice::sgDeviceInstanciated, this, &Core::slDeviceInstanciated);
        currentDevice->initDevice(deviceType);
        timeoutTimer->setInterval(10000);

        if(commandsSended.size()>0) commandsSended.removeFirst();
    }

    foreach(QByteArray answer, recievedAnswer)
    {
        for(int i=0; i < commandsSended.size(); i++)
        {
            if(commandsSended.at(i).indexOf(answer) == 0)
            {
                // qDebug() << "Answer recieved for command: " << commandsSended.at(i) << " commands waiting answer: " << commandsSended.count();
                commandsSended.remove(i);
            }
        }
    }

    processCommands();
}

void Core::slDeviceInstanciated()
{
    currentDevice->moveToThread(QGuiApplication::instance()->thread());
    emit sgCurrentDeviceChanged(currentDevice);

    connect(currentDevice, &AbstractDevice::sgWriteToInterface, this, &Core::sgWriteToInterface, Qt::QueuedConnection);
    connect(currentDevice, &AbstractDevice::sgPushCommandToQueue, this, &Core::pushCommandToQueue, Qt::QueuedConnection);
    connect(currentDevice, &AbstractDevice::sgSendWithoutConfirmation, this, &Core::sendWithoutConfirmation, Qt::QueuedConnection);
    connect(currentDevice, &AbstractDevice::sgProcessCommands, this, &Core::processCommands, Qt::QueuedConnection);
    connect(currentDevice, &AbstractDevice::sgDisconnect, this, &Core::disconnectFromDevice, Qt::QueuedConnection);

    if(m_currentConnectionType == DeviceConnectionType::Offline)
    {
        // Подождать пока прогрузится интерфейс, тк MOCK устройства отвечают слишком быстро в том же потоке
        // TODO возможно нужны сигналы подтверждения от loader
        QThread::msleep(500);
    }
    currentDevice->readFullState();
}

void Core::pushCommandToQueue(QByteArray command, bool finalize)
{
    if(finalize) command.append("\r\n"); // \r

    commandsPending.append(command);
    calculateSendVolume();
}

void Core::sendWithoutConfirmation(QByteArray data, qint64 dataSizeToSend, qint64 dataSizeToRecieve)
{
    if(dataSizeToSend > -1) symbolsToSend = dataSizeToSend;

    if(dataSizeToRecieve > -1) symbolsToRecieve = dataSizeToRecieve;

    commandsSended.clear();
    commandsPending.append(data);
}

void Core::calculateSendVolume()
{
    symbolsSended = 0;
    symbolsToSend = 0;
    QByteArray baTmp;
    foreach(baTmp, commandsPending)
    {
        symbolsToSend += baTmp.length();
    }
}

void Core::processCommands()
{
    if(commandsSended.count()>0) return;

    if(commandsPending.size()>0)
    {
        QByteArray commandToSend = commandsPending.takeFirst();

        int chunckSize;
        int sleepTime;


        switch(m_currentConnectionType)
        {
        // case DeviceDescription::DeviceConnectionType::USBManual:
        case DeviceConnectionType::USB:
        {
            // возможно для CP16 нужен больше sleep time
            chunckSize=512;
            sleepTime=50;
            break;
        }
        case DeviceConnectionType::BLE:
        {
            chunckSize=120;
            sleepTime=150;
            break;
        }

        default:
        {
            chunckSize=512;
            sleepTime=100;
        }
        }

        if((commandToSend.length() > chunckSize))
        {
            if(currentDevice)
            {
                if(!currentDevice->isUpdatingFirmware())
                {
                    for(int sendPosition=0; sendPosition < commandToSend.length(); sendPosition += chunckSize)
                    {
                        timeoutTimer->stop();
                        sendCommand(commandToSend.mid(sendPosition, chunckSize));
                        QThread::msleep(sleepTime);
                        timeoutTimer->start();
                    }
                }
            }
        }
        else
        {
            emit sgSetUIParameter("wait", true);
            sendCommand(commandToSend);
        }
        commandsSended.append(commandToSend);
    }
    else
    {
        if(symbolsSended >= symbolsToSend)
            emit sgSetUIParameter("wait", false);
    }
}

void Core::sendCommand(QByteArray val)
{
    symbolsSended += val.size();
    emit sgWriteToInterface(val);

    updateProgressBar();
}

void Core::updateProgressBar()
{
    if(currentDevice) symbolsToRecieve = currentDevice->symbolsToRecieve();


    float fVal = (double)(symbolsSended+symbolsRecieved) / (double)(symbolsToSend+symbolsToRecieve);

    // qDebug() << Q_FUNC_INFO << "send:" << symbolsSended << symbolsToSend << "recieved:" << symbolsRecieved << symbolsToRecieve << "fval:" << fVal;
    emit sgSetProgress(fVal, "");
}

void Core::recieveTimeout()
{
    if(commandsSended.size()>0)
    {
        QByteArray commandWithoutAnswer = commandsSended.first();

        sendCount++;
        qInfo("!!!!!!!!!!!!!!!!! recieve timeout, attempt=%d !!!!!!!!!!!!!!!!!", sendCount);

        sendCommand(commandWithoutAnswer);

        if(sendCount>3)
        {
            sendCount = 0;

            disconnectFromDevice();

            commandsPending.clear();
            commandsSended.clear();
            emit sgExchangeError();
            return;
        }
    }
    else
    {
        sendCount = 0;
    }
}
