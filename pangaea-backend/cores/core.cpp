#include <QSettings>
#include <QThread>
#include <QSerialPortInfo>
#include <QDir>
#include <QStandardPaths>

#include <QFile>

#include <QtGui/QGuiApplication>

#include "core.h"

#include "cplegacy.h"


Core::Core(QObject *parent) : QObject(parent)
{

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif

    timeoutTimer = new QTimer(this);
    connect(timeoutTimer, &QTimer::timeout, this, &Core::recieveTimeout);
}


void Core::disconnectFromDevice()
{
    qDebug() << __FUNCTION__ << "disconnected";

    timeoutTimer->stop();

    commandsPending.clear();
    // recieveEnabled = true;

    if(currentDevice)
    {
        disconnect(currentDevice);
        delete(currentDevice);
        currentDevice = nullptr;
    }

    emit sgReadyTodisconnect();
}

void Core::slInterfaceConnected(DeviceDescription interfaceDescription)
{
    pushCommandToQueue("amtdev");

    processCommands();
}

void Core::parseInputData(QByteArray ba)
{
    qInfo() << "->" << __FUNCTION__ << ":" << ba;

    if(bytesToRecieve > 0)
    {
        bytesRecieved += ba.size()/2; // 1 byte = 2 symbols, 0xFF
    }
    else bytesRecieved = 0;

    updateProgressBar();

    lastRecievedData += ba;
    commandWorker.parseAnswers(ba);

    QList<QByteArray> recievedAnswer;
    if(currentDevice)
    {
        recievedAnswer = currentDevice->parseAnswers(ba);
    }

    while(commandWorker.haveAnswer())
    {
        lastRecievedData.clear();


        DeviceAnswer recievedCommand = commandWorker.popAnswer();
        QList<QByteArray> parseResult = recievedCommand.parseResult();
        recieveEnabled = recievedCommand.isEnableRecieve();

        switch(recievedCommand.answerType())
        {
            case AnswerType::getAMTDevType:
            {
                if(parseResult.size()==1)
                {
                    DeviceType deviceType = static_cast<DeviceType>(parseResult.at(0).toUInt());
                    controlledDevice.setDeviceType(deviceType);

                    if(controlledDevice.deviceType() < DeviceType::LEGACY_DEVICES)
                    {
                        currentDevice = new CPLegacy(this);

                        connect(currentDevice, &AbstractDevice::sgDeviceInstanciated, this, &Core::slDeviceInstanciated);

                        connect(currentDevice, &AbstractDevice::sgPushCommandToQueue, this, &Core::pushCommandToQueue);
                        connect(currentDevice, &AbstractDevice::sgProcessCommands, this, &Core::processCommands);

                        currentDevice->initDevice(deviceType);
                    }


                    if(controlledDevice.deviceType() == DeviceType::LA3)
                    {
                        // request LA3 maps
                        pushCommandToQueue("sm0");
                        pushCommandToQueue("sm1");
                        pushCommandToQueue("sw1");
                    }

                    commandsSended.removeFirst();
                }
                break;
            }

            case AnswerType::getFWVersion:
            {
                if(parseResult.size()==1)
                {
                    QString firmwareVersion = QString::fromStdString(parseResult.at(0).toStdString());
                    emit sgSetUIText ("devVersion", firmwareVersion);
                    qInfo() << recievedCommand.description() << firmwareVersion;

                    if(deviceFirmware != nullptr)
                        delete deviceFirmware;

                    Firmware *deviceFirmware = new Firmware(firmwareVersion, controlledDevice.deviceType(),
                                                            FirmwareType::DeviceInternal, "device:/internal");

                    controlledDevice.setActualFirmware(deviceFirmware);

                    qInfo() << "version control, minimal: " << controlledDevice.minimalFirmware()->firmwareVersion()
                               << " actual: " << controlledDevice.actualFirmware()->firmwareVersion();

                    if(controlledDevice.isFimwareSufficient())
                    {
                        bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable").toBool();

                        if(isCheckUpdatesEnabled)
                        {
                            emit sgRequestNewestFirmware(deviceFirmware);
                        }
                    }
                    else
                    {
                        qInfo() << "firmware insufficient!";
                        emit sgFirmwareVersionInsufficient(controlledDevice.minimalFirmware(), controlledDevice.actualFirmware());
                    }
                }
                break;
            }

            case AnswerType::la3CleanPreset:
            {
                if(parseResult.size()==1)
                {
                    quint8 bankPreset = parseResult.at(0).toUInt();

                    qInfo() << "LA3 clean preset mapping: " << bankPreset;
                    emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_CLEAN_PRESET, bankPreset);
                }
                break;
            }

            case AnswerType::la3DrivePreset:
            {
                if(parseResult.size()==1)
                {
                    quint8 bankPreset = parseResult.at(0).toUInt();
                    qInfo() << "LA3 drive preset mapping: " << bankPreset;
                    emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_DRIVE_PRESET, bankPreset);
                }
                break;
            }

            case AnswerType::la3ModeChange:
            {
                qInfo() << recievedCommand.description();

                // pushReadPresetCommands();
                // pushCommandToQueue("rns\n");
                pushCommandToQueue("sw1\n");
                processCommands();
                break;
            }

            case AnswerType::la3CurrentChannel:
            {
                if(parseResult.size()==1)
                {
                    QString strCurrentLa3Mode = QString::fromStdString(parseResult.at(0).toStdString());
                    quint8 currentLa3Mode = 0;
                    if(strCurrentLa3Mode == "high")
                    {
                        currentLa3Mode = 0;
                        qInfo() << "LA3 current mode: " << "clean channel";
                    }
                    else
                    {
                        currentLa3Mode = 1;
                        qInfo() << "LA3 current mode: " << "clean channel";
                    }
                    emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_CURRENT_CHANNEL, currentLa3Mode);
                }
                break;
            }

            // case AnswerType::getIrNameSize:
            // {
            //     if(recievedCommand.parseResult().length()==1)
            //     {
            //         QByteArray baAnswer = recievedCommand.parseResult().at(0);
            //         QString wavName;
            //         int wavSize=0;
            //         if(baAnswer.indexOf("FILE_NOT_FIND") == 0)
            //         {
            //             wavName= "";
            //             break;
            //         }
            //         else
            //         {
            //             quint16 positionEndName = baAnswer.lastIndexOf(".wav");
            //             wavName = QString::fromUtf8(baAnswer.left(positionEndName+4));
            //             wavName.replace(" ","_");

            //             QByteArray baWavSize = baAnswer.mid(positionEndName+5);
            //             wavSize = baWavSize.toInt();
            //         }

            //         switch(presetManager.currentState())
            //         {
            //             //Повтор необходим. Надо чтобы обрабатывало cc/r ТОЛЬКО в состоянии копирования и экспорта
            //             case PresetState::Copying:
            //             {
            //                 // copiedPreset.setImpulseName(wavName);

            //                 bytesToRecieve = wavSize;
            //                 qDebug() << "Recieve Ir. Name:" << wavName << " ,size:" << bytesToRecieve;
            //                 emit sgSetUIParameter("ir_downloading", true);
            //                 break;
            //             }

            //             case PresetState::Exporting:
            //             {
            //                 bytesToRecieve = wavSize;
            //                 // currentPreset.setImpulseName(wavName);
            //                 emit sgSetUIParameter("ir_downloading", true);
            //                 break;
            //             }

            //             default:
            //             {
            //                 // currentPreset.setImpulseName(wavName);
            //             }
            //         }
            //             recieveEnabled = false; // команда cc обрабатывается двумя парсерами. Этим и getIr при полном выполнении
            //     }
            //     break;
            // }

            // case AnswerType::getIr:
            // {
            //     QByteArray impulseData;
            //     if(recievedCommand.parseResult().length()==2)
            //     {
            //         impulseData = QByteArray::fromHex(parseResult.at(1));
            //     }

            //     switch(presetManager.currentState())
            //     {
            //         case PresetState::Copying:
            //         {
            //             // copiedPreset.setWaveData(impulseData);
            //             break;
            //         }

            //         case PresetState::Exporting:
            //         {
            //             // currentPreset.setWaveData(impulseData);
            //             // currentPreset.exportData();

            //             // emit sgSetUIText("preset_exported", currentPreset.pathToExport());

            //             presetManager.returnToPreviousState();
            //             break;
            //         }

            //         default:{}
            //     }
            //     bytesToRecieve=0;

            //     timeoutTimer->start();

            //     qInfo() << recievedCommand.description();
            //     break;
            // }

            case AnswerType::requestNextChunk:
            {
                qInfo() << recievedCommand.description();
                if(!fwUpdate)
                {
                   recieveEnabled = false;

                }
                else
                {
                    if(m_rawFirmwareData.length() > 0)
                    {
                        QByteArray baSend, baTmp;
                        baTmp = m_rawFirmwareData.left(fwUploadBlockSize);
                        m_rawFirmwareData.remove(0, baTmp.length());

                        baSend.append(QString("%1\n").arg(baTmp.length()).toUtf8());
                        baSend.append(baTmp);

                        commandsPending.append(baSend);
                    }
                    else
                    {
                        commandsPending.append("0\n"); // нулевой блок это знак что файл загружен
                    }
                    processCommands();
                }
                break;
            }

            case AnswerType::ackFWUFinished:
            {
                if(fwUpdate)
                {
                    emit sgSetUIParameter("fw_update_enabled", false);
                    fwUpdate = false;
                }
                else
                {
                    isFormatting = false;
                    emit sgSetUIParameter("format_complete", true);
                }
                qInfo() << recievedCommand.description();
                emit sgImmediatelyDisconnect();
                break;
            }

            default:
            {
            }
        }
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

void Core::slDeviceInstanciated(DeviceType deviceType)
{
    emit sgCurrentDeviceChanged(currentDevice);
    currentDevice->moveToThread(QGuiApplication::instance()->thread());
}

void Core::pushCommandToQueue(QByteArray command, bool finalize)
{
    if(finalize) command.append("\r\n");

    commandsPending.append(command);
    calculateSendVolume();
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

void Core::setFirmware(QString fullFilePath)
{
    qDebug()<< __FUNCTION__ << "fullFilePath" <<fullFilePath;

    emit sgSetUIText("firmware_path", fullFilePath);

    if(!Firmware::isFirmwareFile(fullFilePath))
    {
        emit sgSetUIText("not_fw_file_error", fullFilePath);
        qDebug() << __FUNCTION__ << "Not firmware file";
        return;
    }

    QFile file(fullFilePath.toUtf8());

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baFW = file.read(file.size());
        uploadFirmware(baFW);

        file.close();
    }
    else
    {
        qDebug() << __FUNCTION__ << __LINE__ << "Can not open file " << fullFilePath;
        emit sgSetUIText("open_fw_file_error", fullFilePath);
    }
}

void Core::uploadFirmware(const QByteArray& firmware)
{
    if(firmware.length()>0)
    {
        m_rawFirmwareData = firmware;

        symbolsToSend = m_rawFirmwareData.size() + 4 * m_rawFirmwareData.size() / fwUploadBlockSize + 2; // 128\n = 4 * (parts num and 0\n = 2
        bytesToRecieve = 0;// 18 * m_rawFirmwareData.size() / fwUploadBlockSize; // REQUEST_CHUNK_SIZE\n = 18

        emit sgSetUIParameter("fw_update_enabled", true);
        timeoutTimer->stop();
        // timeoutTimer->setInterval(5000);

        fwUpdate = true;
        recieveEnabled = false;

        QByteArray baTmp, baSend;
        baSend.append("fwu\r");

        baTmp = m_rawFirmwareData.left(fwUploadBlockSize);
        m_rawFirmwareData.remove(0, baTmp.length());

        baSend.append(QString("%1\n").arg(baTmp.length()).toUtf8());
        baSend.append(baTmp);

        commandsPending.append(baSend);
        processCommands();
    }
}


// void Core::importPreset(QString filePath, QString fileName)
// {
//     Q_UNUSED(fileName)

//     Preset importedPreset;

//     if(!importedPreset.importData(filePath))
//     {
//         emit sgSetUIText("preset_import_unsuccecfull", "");
//         return;
//     }

//     // TODO: paste и import одинаковые операции. Закинуть в один метод и вызывать его
//     // setPresetData(importedPreset);

//     if(importedPreset.waveData().isEmpty())
//     {
//         if(currentPreset.impulseName() != "")
//         {
//             importedPreset.setWaveData(IRWorker::flatIr());
//             importedPreset.setImpulseName("");
//         }
//     }
//     uploadImpulseData(importedPreset.waveData(), true, importedPreset.impulseName());
//     currentPreset = importedPreset;

//     isPresetEdited = true;
//     emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);
// }

// void Core::exportPreset(QString filePath, QString fileName)
// {
//     if(presetManager.currentState() != PresetState::Exporting)
//     {
//         QString folderPath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/presets/";

//         qDebug() << "Preset name: " <<fileName;

//         presetManager.setCurrentState(PresetState::Exporting);

//         if(!QDir(folderPath).exists())
//         {
//             QDir().mkpath(folderPath);
//         }

//         pushCommandToQueue("gs");
//         currentPreset.setPathToExport(filePath);
//         pushCommandToQueue("cc");
//         processCommands();
//     }
// }


void Core::slRecieveAppAction(AppAction appParameterType, QVariantList parameters)
{
    switch(appParameterType)
    {

    case FORMAT_FLASH:
    {
        emit sgSetUIParameter("wait", true);

        isFormatting = true;
        timeoutTimer->setInterval(10000);

        emit sgWriteToInterface(QString("fsf\r\n").toUtf8());
        recieveEnabled = false;
        break;
    }

    case SET_LA3_MAPPINGS:
    {
        quint8 cleanPreset = parameters.at(0).toInt();
        quint8 drivePreset = parameters.at(1).toInt();
        pushCommandToQueue(QString("sm0 %2").arg(cleanPreset).toUtf8());
        pushCommandToQueue(QString("sm1 %2").arg(drivePreset).toUtf8());
        processCommands();
        break;
    }
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

        if(controlledDevice.deviceType() == DeviceType::legacyCP100 || controlledDevice.deviceType() == DeviceType::legacyCP100PA) // TODO chunkSize зависит от интерфейса
        {
            chunckSize=512;
            sleepTime=50;
        }
        else
        {
            chunckSize=120;
            sleepTime=150;
        }

        if((commandToSend.length() > chunckSize) && (!fwUpdate))
        {
            for(int sendPosition=0; sendPosition < commandToSend.length(); sendPosition += chunckSize)
            {
                timeoutTimer->stop();
                sendCommand(commandToSend.mid(sendPosition, chunckSize));
                QThread::msleep(sleepTime);
            }
        }
        else
        {
            emit sgSetUIParameter("wait", true);
            sendCommand(commandToSend);
        }
        commandsSended.append(commandToSend);


        quint32 timeoutInterval=1000;
        if(commandToSend.indexOf("rns")==0)
        {
            timeoutInterval = 10000;
        }

        if(!fwUpdate)
        {
            timeoutTimer->start(timeoutInterval);
        }
    }
    else
    {
        emit sgSetUIParameter("wait", false);
    }
}

void Core::sendCommand(QByteArray val)
{
    symbolsSended += val.size();
    recieveEnabled = false;
    emit sgWriteToInterface(val);

    updateProgressBar();
}

void Core::updateProgressBar()
{
    if(currentDevice) bytesToRecieve = currentDevice->bytesToRecieve();

    float fVal = (double)(symbolsSended+bytesRecieved) / (double)(symbolsToSend+bytesToRecieve);
    emit sgSetProgress(fVal, "");
}

void Core::recieveTimeout()
{
    if(isFormatting) // Format timeout
    {
        emit sgSetUIParameter("format_error", 1);
        isFormatting = false;
    }

    if(commandsSended.size()>0)
    {
        QByteArray commandWithoutAnswer = commandsSended.first();

        if(recieveEnabled)
        {
            sendCount = 0;
        }
        else
        {
            sendCount++;
            qInfo() << "!!!!!!!!!!!!!!!!! recieve timeout !!!!!!!!!!!!!!!!!" << sendCount;

            sendCommand(commandWithoutAnswer);
            timeoutTimer->setInterval(1000);

            if(sendCount>3)
            {
                sendCount = 0;

                disconnectFromDevice();

                if(fwUpdate)
                {
                    fwUpdate = false;
                }

                commandsPending.clear();
                commandsSended.clear();
                emit sgExchangeError();
                return;
            }
        }
    }
}
