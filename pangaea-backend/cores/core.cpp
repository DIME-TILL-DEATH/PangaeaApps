#include <QSettings>
#include <QThread>
#include <QSerialPortInfo>
#include <QDir>
#include <QStandardPaths>

#include <QFile>

#include "core.h"

// TODO change to MTU, or lower than size. Wait answer


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

    indicationTimer = new QTimer(this);
    indicationTimer->setInterval(1000);
    connect(indicationTimer, &QTimer::timeout, this, &Core::indicationRequest);
}


void Core::slReadyToDisconnect()
{
    timeoutTimer->stop();
    indicationTimer->stop();

    commandsPending.clear();
    recieveEnabled = true;

    emit sgReadyTodisconnect();
}

void Core::slInterfaceConnected(DeviceDescription interfaceDescription)
{
    switch (interfaceDescription.connectionType())
    {
    case DeviceConnectionType::BLE:
        indicationTimer->setInterval(500);
        break;
    default:
        indicationTimer->setInterval(20);
        break;
    }
    readAllParameters();
}

void Core::parseInputData(QByteArray ba)
{

    if(bytesToRecieve > 0)
    {
        bytesRecieved += ba.size()/2; // 1 byte = 2 symbols, 0xFF
    }
    else bytesRecieved = 0;

    updateProgressBar();

    lastRecievedData += ba;
    commandWorker.parseAnswers(ba);

    qInfo() << "->" << __FUNCTION__ << ":" << ba;

    while(commandWorker.haveAnswer())
    {
        lastRecievedData.clear();

        // if(commandWorker.displayNextAnswer())
        //     qInfo() << "->" << __FUNCTION__ << ":" << ba;

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

                    if(controlledDevice.deviceType() == DeviceType::CP16 || controlledDevice.deviceType() == DeviceType::CP16PA)
                    {
                        pushCommandToQueue("sw4 disable"); // disable hardware switches
                    }

                    if(controlledDevice.deviceType() == DeviceType::LA3PA || controlledDevice.deviceType() == DeviceType::LA3RV)
                    {
                        // request LA3 maps
                        pushCommandToQueue("sm0");
                        pushCommandToQueue("sm1");
                        pushCommandToQueue("sw1");
                    }

                    emit sgRecieveDeviceParameter(DeviceParameter::Type::DEVICE_TYPE, deviceType);

                    qInfo() << recievedCommand.description();
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

                    if(controlledDevice.deviceType() == DeviceType::UnknownDev)
                    {
                        pushCommandToQueue("amtdev\r\n");
                        processCommands();
                        return;
                    }

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
                    //emit sgSetUIText("devVersion", deviceFirmware->firmwareVersion());

                    qInfo() << "Firmware can indicate:" << controlledDevice.isFirmwareCanIndicate();
                    emit sgRecieveDeviceParameter(DeviceParameter::Type::FIRMWARE_CAN_INDICATE, controlledDevice.isFirmwareCanIndicate());
                }
                break;
            }

            case AnswerType::getStatus:
            {
                qInfo() << recievedCommand.description();
                if(recievedCommand.parseResult().size()==1)
                {
                    QByteArray baPresetData = parseResult.at(0);

                    if(baPresetData.size()==(43*2))
                    {
                        switch(presetManager.currentState())
                        {
                            case PresetState::Copying:
                            {
                               // copy preview wave data
                               if(currentPreset.wavSize() != 0)
                                   copiedPreset = currentPreset;

                                copiedPreset.setRawData(baPresetData);
                                presetManager.returnToPreviousState();
                                break;
                            }

                            case PresetState::Changing:
                            {
                                currentPreset.setRawData(baPresetData);
                                currentSavedPreset = currentPreset;
                                emit sgUpdatePreset(currentSavedPreset);
                                presetManager.returnToPreviousState();
                                break;
                            }

                            case PresetState::Compare:
                            {
                                // Необходимая заглушка! не удалять
                                break;
                            }

                            default:
                            {
                                currentPreset.setRawData(baPresetData);
                                emit sgUpdatePreset(currentPreset);
                                break;
                            }
                        }

                        quint8 count=0;
                        quint8 nomByte=0;
                        QString sss;

                        preset_data_legacy_t str;
                        memcpy(&str, baPresetData.data(), sizeof(preset_data_legacy_t));


                        foreach(QChar val, baPresetData) //quint8
                        {
                            if((nomByte&1)==0)
                            {
                                sss.clear();
                                sss.append(val);
                            }
                            else
                            {
                                sss.append(val);

                                DeviceParameter::Type paramType = static_cast<DeviceParameter::Type>(count);
                                if(DeviceParameter::isSigned(paramType))
                                {
                                    emit sgRecieveDeviceParameter(paramType, (qint8)sss.toInt(nullptr, 16));
                                }
                                else
                                {
                                    emit sgRecieveDeviceParameter(paramType, (qint16)sss.toInt(nullptr, 16));
                                }
                                count++;
                            }
                            nomByte++;
                        }

                        emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);
                    }
                }
                break;
            }

            case AnswerType::getBankPreset:
            {
                if(parseResult.size()==1)
                {
                    if(parseResult.at(0).size()==4)
                    {
                        quint8 bank = parseResult.at(0).left(2).toUInt();
                        quint8 preset   = parseResult.at(0).right(2).toUInt();

                        currentPreset.setBankPreset(static_cast<quint8>(bank), static_cast<quint8>(preset));

                        emit sgRecieveDeviceParameter(DeviceParameter::Type::BANK,  currentPreset.bankNumber());
                        emit sgRecieveDeviceParameter(DeviceParameter::Type::PRESET, currentPreset.presetNumber());

                        qInfo() << recievedCommand.description() << "bank:" << bank << "preset:" << preset;
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

                pushReadPresetCommands();
                pushCommandToQueue("rns\n");
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

            case AnswerType::getOutputMode:
            {
                if(parseResult.size()==1)
                {
                    quint8 mode = parseResult.at(0).toUInt();
                    emit sgRecieveDeviceParameter(DeviceParameter::Type::OUTPUT_MODE, mode);
                    qInfo() << recievedCommand.description();
                }
                break;
            }

            case AnswerType::getIrList:
            {
                qInfo() << recievedCommand.description();

                m_presetsList.clear();

                QString impulsNames;
                QString impulsEn;

                quint8 bankNumber=0;
                quint8 presetNumber=0;

                QList<QByteArray>::const_iterator it = parseResult.constBegin();
                while (it != parseResult.constEnd())
                {
                    Preset currentLitedPreset(&controlledDevice);

                    currentLitedPreset.setBankPreset(bankNumber, presetNumber);

                    if(presetNumber == controlledDevice.maxBankCount()-1)
                    {
                        presetNumber = 0;
                        bankNumber++;
                    }
                    else
                    {
                        presetNumber++;
                    }

                    QByteArray ba = *it;
                    if(ba=="*")
                        ba="";
                    quint16 positionEndName = ba.lastIndexOf(".wav ");
                    currentLitedPreset.setImpulseName(ba.left(positionEndName+4));
                    impulsNames.append(ba.left(positionEndName+4));
                    impulsNames.append(",");
                    it++;

                    ba = *it;
                    currentLitedPreset.setIsIrEnabled(ba.toInt());
                    impulsEn.append(ba);
                    impulsEn.append(",");
                    it++;

                    m_presetsList.append(currentLitedPreset);
                }
                emit sgRefreshPresetList(&m_presetsList);

                break;
            }

            case AnswerType::getIrName:
            {
                QString impulseName;

                if(parseResult.size()>0)
                    impulseName = recievedCommand.parseResult().at(0);
                else
                    impulseName = "";

                switch(presetManager.currentState())
                {
                    case PresetState::Compare:
                    {
                        // Необходимая заглушка. Не удалять!
                        break;
                    }

                    default:
                    {
                        currentPreset.setImpulseName(impulseName);
                    }
                }

                emit sgSetUIText("impulse_name", impulseName);
                qInfo() << recievedCommand.description() << "impulse name:" << impulseName;
                emit sgUpdatePreset(currentPreset);
                break;
            }

            case AnswerType::getIrNameSize:
            {
                if(recievedCommand.parseResult().length()==1)
                {
                    QByteArray baAnswer = recievedCommand.parseResult().at(0);
                    QString wavName;
                    int wavSize=0;
                    if(baAnswer.indexOf("FILE_NOT_FIND") == 0)
                    {
                        wavName= "";
                        break;
                    }
                    else
                    {
                        quint16 positionEndName = baAnswer.lastIndexOf(".wav");
                        wavName = QString::fromUtf8(baAnswer.left(positionEndName+4));
                        wavName.replace(" ","_");

                        QByteArray baWavSize = baAnswer.mid(positionEndName+5);
                        wavSize = baWavSize.toInt();
                    }

                    switch(presetManager.currentState())
                    {
                        //Повтор необходим. Надо чтобы обрабатывало cc/r ТОЛЬКО в состоянии копирования и экспорта
                        case PresetState::Copying:
                        {
                            copiedPreset.setImpulseName(wavName);

                            bytesToRecieve = wavSize;
                            qDebug() << "Recieve Ir. Name:" << wavName << " ,size:" << bytesToRecieve;
                            emit sgSetUIParameter("ir_downloading", true);
                            break;
                        }

                        case PresetState::Exporting:
                        {
                            bytesToRecieve = wavSize;
                            currentPreset.setImpulseName(wavName);
                            emit sgSetUIParameter("ir_downloading", true);
                            break;
                        }

                        default:
                        {
                            currentPreset.setImpulseName(wavName);
                        }
                    }
                        recieveEnabled = false; // команда cc обрабатывается двумя парсерами. Этим и getIr при полном выполнении
                }
                break;
            }

            case AnswerType::getIr:
            {
                QByteArray impulseData;
                if(recievedCommand.parseResult().length()==2)
                {
                    impulseData = QByteArray::fromHex(parseResult.at(1));
                }

                switch(presetManager.currentState())
                {
                    case PresetState::Copying:
                    {
                        copiedPreset.setWaveData(impulseData);
                        break;
                    }

                    case PresetState::Exporting:
                    {
                        currentPreset.setWaveData(impulseData);
                        currentPreset.exportData();

                        emit sgSetUIText("preset_exported", currentPreset.pathToExport());

                        presetManager.returnToPreviousState();
                        break;
                    }

                    default:{}
                }
                bytesToRecieve=0;

                timeoutTimer->start();
                indicationTimer->start();

                qInfo() << recievedCommand.description();
                break;
            }

            case AnswerType::indicationRequest:
            {
                recieveEnabled = false; // TODO waitAnswer?

                //sqrt(ind_in_p[1])*(31.0/sqrt(8388607.0));

                quint32 signalIn = parseResult.at(0).toInt();
                quint32 signalOut = parseResult.at(1).toInt();
                // quint32 signalOutR = parseResult.at(2).toInt();

                // qDebug() << signalIn << signalOutL << signalOutR;

                emit sgRecieveDeviceParameter(DeviceParameter::Type::SIGNAL_IN, signalIn);
                emit sgRecieveDeviceParameter(DeviceParameter::Type::SIGNAL_OUT, signalOut);
                // emit sgSetUiDeviceParameter(DeviceParameter::Type::SIGNAL_OUT_R, signalOutR);
                break;
            }

            case AnswerType::ccAck:
            {
                emit sgSetUIParameter("ir_upload_finished", true);
                recieveEnabled=false;

                timeoutTimer->stop(); // wait for impulse apllying (TODO возможно по размеру импульса посчитать время сохранения в устройстве)
                indicationTimer->stop();

                qInfo() << recievedCommand.description();
                break;
            }

            case AnswerType::ccEND:
            {
                timeoutTimer->start();
                indicationTimer->start();

                qInfo() << recievedCommand.description();
                presetManager.returnToPreviousState();
                break;
            }

            case AnswerType::ccError:
            {
                timeoutTimer->start();
                indicationTimer->start();

                emit sgSetUIText("impulse_save_error", currentPreset.impulseName());

                qInfo() << recievedCommand.description();
                presetManager.returnToPreviousState();
                break;
            }

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

            case AnswerType::ackEsc:
            {
                presetManager.setCurrentState(PresetState::Compare);
                pushCommandToQueue("gs\r\n");
                pushCommandToQueue("rn\r\n");
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

            case AnswerType::ackPresetChange:
            {
                qInfo() << "Preset change, updating state";                
                currentPreset.clearWavData();
                presetManager.setCurrentState(PresetState::Changing);
                pushReadPresetCommands();
                break;
            }

            case AnswerType::ackSavePreset:
            {
                qInfo() << "Preset data saved to device";
                currentPreset.clearWavData();                                
                break;
            }

            case AnswerType::ackEnableSW4:
            {
                qInfo() << "Enabling buttons";
                slReadyToDisconnect();
                break;
            }

            default:
            {
                qDebug() << recievedCommand.description();
            }
        }

        if(recieveEnabled)
        {
            if(commandsSended.length()>0)
                commandsSended.removeFirst();

            qDebug() << "Commands waiting answer: " << commandsSended.count();
            processCommands();
        }
    }
}

void Core::pushCommandToQueue(QByteArray val)
{
    val.append("\r\n");
    commandsPending.append(val);
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

void Core::setImpulse(QString filePath)
{
    QString fileName;
    QFileInfo fileInfo(filePath);

    if(fileInfo.isFile())
    {
        fileInfo.absoluteDir();
        fileName = fileInfo.fileName();
    }
    else return;

    stWavHeader wavHead = IRWorker::getFormatWav(filePath);

    if((wavHead.sampleRate != 48000) || (wavHead.bitsPerSample != 24) || (wavHead.numChannels != 1))
    {
        qDebug() << __FUNCTION__ << "Not supported wav format";
        emit sgSetUIText("not_supported_ir",
                         QString().setNum(wavHead.sampleRate)+" Hz/"+
                         QString().setNum(wavHead.bitsPerSample)+" bits/"+
                         QString().setNum(wavHead.numChannels)+" channel");
    }
    else
    {
        qDebug("$$$$$ %s %d", __FUNCTION__, __LINE__);

        isPresetEdited = true;
        emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);

        irWorker.decodeWav(filePath);

        QByteArray fileData = irWorker.formFileData();
        currentPreset.setWaveData(fileData);

        uploadImpulseData(fileData, true, fileName);
    }
}

void Core::escImpulse()
{
    pushCommandToQueue("lcc");
    pushCommandToQueue("rn");
    processCommands();
}

void Core::uploadImpulseData(const QByteArray &impulseData, bool isPreview, QString impulseName)
{
    if(impulseData.isEmpty())
    {
        qDebug() << __FUNCTION__ << "no wave data";
        return;
    }

    presetManager.setCurrentState(PresetState::UploadingIr);

    impulseName.replace(QString(" "), QString("_"), Qt::CaseInsensitive);

    emit sgSetUIText("impulse_name", impulseName);   
    currentPreset.setImpulseName(impulseName);

    QByteArray baSend, irData;
    quint16 bytesToUpload;

    if(isPreview)
    {
        baSend.append(QString("cc s 1\r").toUtf8());
        bytesToUpload = 984*3;
        irData = impulseData.mid(44); //cut wav header
    }
    else
    {
        baSend.append(QString("cc %1 0\r").arg(impulseName).toUtf8());
        irData = impulseData;
        bytesToUpload = impulseData.size();
        currentPreset.setImpulseName(impulseName);
    }
    emit sgUpdatePreset(currentPreset);

    for(quint16 i=0; i<bytesToUpload; i++)
    {
        QString sTmp;
        quint8  chr;
        if(i>=irData.length())
            sTmp = QString("00");
        else
        {
            chr = irData.at(i);
            if(chr<=15)
                sTmp = QString("0%1").arg(chr, 1, 16);
            else
                sTmp = QString("%1").arg (chr, 2, 16);
        }
        baSend.append(sTmp.toUtf8());
    }
    pushCommandToQueue(baSend);

    // загрузка импульса автоматом включает модуль в устройстве, отобразить это визуально
    emit sgRecieveDeviceParameter(DeviceParameter::Type::CABINET_ENABLE, true);

    processCommands();
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

void Core::changePreset(quint8 bank, quint8 preset)
{
    quint8 val = Preset::calcPresetFlatIndex(controlledDevice.deviceType(), bank, preset);

    isPresetEdited = false;
    emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);

    if(presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    emit sgUpdatePreset(currentSavedPreset); // Обновить актуальный пресет перед переключением

    pushCommandToQueue(QString("pc %2").arg(val, 2, 16, QChar('0')).toUtf8());

    processCommands();
}

void Core::saveChanges()
{
    qDebug()<<__FUNCTION__;

    if(presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    pushCommandToQueue("sp");
    processCommands();
    // impulse data uploading after answer
    if(currentPreset.waveData() == IRWorker::flatIr())
    {
        if(controlledDevice.deviceType()==DeviceType::CP16 || controlledDevice.deviceType()==DeviceType::CP16PA)
            pushCommandToQueue("pwsd");
        else
            pushCommandToQueue("dcc");

        currentPreset.clearWavData();
    }
    else
    {
        uploadImpulseData(currentPreset.waveData(), false, currentPreset.impulseName());
    }

    qDebug() << "Current preset bp: " << currentPreset.bankNumber() << ":" << currentPreset.presetNumber() << "impulse name:" << currentPreset.impulseName();

    currentSavedPreset = currentPreset;
    emit sgUpdatePreset(currentSavedPreset);
    isPresetEdited = false;
    emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);
}

void Core::comparePreset()
{
    if(presetManager.currentState() != PresetState::Compare)
    {
        emit sgSetAppParameter(AppParameter::COMPARE_STATE, true);
        pushCommandToQueue("gs\r\n");
        pushCommandToQueue("esc\r\n");
    }
    else
    {
        presetManager.returnToPreviousState();
        setPresetData(currentPreset);
        uploadImpulseData(currentPreset.waveData(), true, currentPreset.impulseName());
        emit sgSetAppParameter(AppParameter::COMPARE_STATE, false);
    }
    processCommands();
}

void Core::importPreset(QString filePath, QString fileName)
{
    Q_UNUSED(fileName)

    Preset importedPreset(&controlledDevice);

    if(!importedPreset.importData(filePath))
    {
        emit sgSetUIText("preset_import_unsuccecfull", "");
        return;
    }

    // TODO: paste и import одинаковые операции. Закинуть в один метод и вызывать его
    setPresetData(importedPreset);

    if(importedPreset.waveData().isEmpty())
    {
        if(currentPreset.impulseName() != "")
        {
            importedPreset.setWaveData(IRWorker::flatIr());
            importedPreset.setImpulseName("");
        }
    }
    uploadImpulseData(importedPreset.waveData(), true, importedPreset.impulseName());
    currentPreset = importedPreset;

    isPresetEdited = true;
    emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);
}

void Core::setPresetData(const Preset &preset)
{
    QByteArray ba;

    ba.append("gs 1\r");
    ba.append(preset.rawData());
    pushCommandToQueue(ba);

    pushCommandToQueue("gs"); // read settled state

    processCommands();
}

void Core::exportPreset(QString filePath, QString fileName)
{
    if(presetManager.currentState() != PresetState::Exporting)
    {
        QString folderPath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/presets/";

        qDebug() << "Preset name: " <<fileName;

        presetManager.setCurrentState(PresetState::Exporting);

        if(!QDir(folderPath).exists())
        {
            QDir().mkpath(folderPath);
        }

        pushCommandToQueue("gs");
        currentPreset.setPathToExport(filePath);
        pushCommandToQueue("cc");
        processCommands();
    }
}

void Core::copyPreset()
{
    presetManager.setCurrentState(PresetState::Copying);

    if(currentPreset.wavSize() == 0)
        pushCommandToQueue("cc");

    pushCommandToQueue("gs");
    processCommands();
}

void Core::pastePreset()
{
    setPresetData(copiedPreset);

    quint8 currentBankNumber = currentPreset.bankNumber();
    quint8 currentPresetNumber = currentPreset.presetNumber();

    // TODO для исправления бага с обновлением пресета после вставки
    // эта часть должна быть в getStatus по ключу PresetState::Pasting
    // но это немного костыльно и MAP для актуального пресета должен обновляться
    // и хранится как-то по-другому. При этом при переключении применятся старый
    if(copiedPreset.waveData().isEmpty())
    {
        if(currentPreset.impulseName() != "")
        {
            copiedPreset.setWaveData(IRWorker::flatIr());
            copiedPreset.setImpulseName("");
        }
    }
    uploadImpulseData(copiedPreset.waveData(), true, copiedPreset.impulseName());
    currentPreset = copiedPreset;
    currentPreset.setBankPreset(currentBankNumber, currentPresetNumber);

    isPresetEdited = true;
    emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);
}

void Core::setParameter(QString name, quint8 value)
{
    QString sendStr;

    if(name==("esc")) sendStr = QString("esc\r\n");

    if(sendStr.length()>0)
    {
        emit sgWriteToInterface(sendStr.toUtf8());
        recieveEnabled = false;
    }
    else
    {
        qWarning() << "Send string empty! Parameter doesn't exist";
    }
}

void Core::slRecieveAppAction(AppAction appParameterType, QVariantList parameters)
{
    switch(appParameterType)
    {
    case SAVE_CHANGES: saveChanges(); break;
    case CHANGE_PRESET: changePreset(parameters.at(0).toInt(),parameters.at(1).toInt()); break;
    case COPY_PRESET: copyPreset(); break;
    case PASTE_PRESET: pastePreset(); break;
    case COMPARE_PRESET: comparePreset(); break;

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

void Core::slSetDeviceParameter(DeviceParameter::Type deviceParameterType, quint8 value)
{
    QString sendStr = DeviceParameter::sendString(deviceParameterType, value);
    isPresetEdited = true;
    emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);

    recieveEnabled = false;

    if(sendStr.size() > 0)
        emit sgWriteToInterface(sendStr.toUtf8(), false);

    if(deviceParameterType == DeviceParameter::Type::CABINET_ENABLE)
    {
        //TODO в rawData обновлять все поля пресета, а не только это
        currentPreset.setIsIrEnabled(value);
    }
}

void Core::restoreValue(QString name)
{
    Q_UNUSED(name)
    // TODO: добавить в Preset  объект ?DeviceControls?. По имени находить позицию параметра в rawData и возвращать, устанавливая этот параметр
    // и отдельно обрабатывать случай банка и пресета
    // пока костыль для отмены сохранения пресета!!!!!


    if(name == "bank") emit sgRecieveDeviceParameter(DeviceParameter::Type::BANK,  currentPreset.bankNumber());

    if(name == "preset") emit sgRecieveDeviceParameter(DeviceParameter::Type::PRESET, currentPreset.presetNumber());
}

void Core::readAllParameters()
{
    isPresetEdited = false;

    pushCommandToQueue("amtdev");
    pushCommandToQueue("amtver");

    pushReadPresetCommands();

    pushCommandToQueue("rns");

    pushCommandToQueue("gm");

    processCommands();
}

void Core::pushReadPresetCommands()
{
    pushCommandToQueue("gb");
    pushCommandToQueue("rn");
    pushCommandToQueue("gs");
}

void Core::processCommands()
{
    if(commandsSended.count()>0) return;

    if(commandsPending.size()>0)
    {
        QByteArray commandToSend = commandsPending.takeFirst();

        int chunckSize;
        int sleepTime;

        if(controlledDevice.deviceType() == DeviceType::CP100 || controlledDevice.deviceType() == DeviceType::CP100PA)
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
            emit sgSetUIParameter("data_uploading", true);
            for(int sendPosition=0; sendPosition < commandToSend.length(); sendPosition += chunckSize)
            {
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

        if(commandToSend.indexOf("cc\r\n")==0)
        {
            timeoutTimer->stop();
            indicationTimer->stop();
        }
        else
        {
            quint32 timeoutInterval=1000;
            if(commandToSend.indexOf("rns\r\n")==0)
            {
                timeoutInterval = 10000;
            }

            if(!fwUpdate)
            {
                timeoutTimer->start(timeoutInterval);
                indicationTimer->start();
            }
        }
    }
    else
    {
        emit sgSetUIParameter("wait", false);
    }
}

void Core::indicationRequest()
{

    // if(controlledDevice.isFirmwareCanIndicate())
    // {
    //     qDebug() << "Indication request";
    //     emit sgWriteToInterface("iio\r\n", false);
    // }
    // else
    // {
        indicationTimer->stop();
    // }
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
            qInfo() << "Data recieved, but not parsed: " << lastRecievedData;

            sendCommand(commandWithoutAnswer);
            timeoutTimer->setInterval(1000);

            if(sendCount>3)
            {
                sendCount = 0;

                emit sgExchangeError();

                if(fwUpdate)
                {
                    fwUpdate = false;
                }

                commandsPending.clear();
                commandsSended.clear();
                return;
            }
        }
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
    float fVal = (double)(symbolsSended+bytesRecieved) / (double)(symbolsToSend+bytesToRecieve);
    emit sgSetProgress(fVal, "");
}

void Core::sw4Enable()
{
    if(controlledDevice.deviceType() == DeviceType::CP16 || controlledDevice.deviceType() == DeviceType::CP16PA)
    {
        pushCommandToQueue("sw4 enable");
        processCommands();
    }
}

void Core::stopCore()
{
    timeoutTimer->stop();
    indicationTimer->stop();
}
