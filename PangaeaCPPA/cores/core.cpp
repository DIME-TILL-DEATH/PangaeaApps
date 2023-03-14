#include <QSettings>
#include <QThread>
#include <QSerialPortInfo>
#include <QDir>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QStandardPaths>

#include <QFile>

#include "core.h"

Core::Core(QObject *parent) : QObject(parent)
{

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif

    QObject::connect(&deviceControls, &DeviceControls::sgSetInterfaceValue, this, &Core::sgSetUIParameter);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Core::recieveTimeout);

    m_presetListModel.refreshModel(&m_presetsList);
    connect(this, &Core::sgRefreshPresetList, &m_presetListModel, &PresetListModel::refreshModel, Qt::QueuedConnection);

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "PresetListModel", &m_presetListModel);
}


void Core::slReadyToDisconnect()
{
    timer->stop();
    commandsPending.clear();
    enableRecieve = true;

//    isConnected = false;  //TODO interface status. Differs from mobile
    emit sgSetUIParameter("ready_to_disconnect", true);
}

void Core::parseInputData(const QByteArray& ba)
{
    qDebug() << "->" << __FUNCTION__ << ":" << ba;

    if(bytesToRecieve > 0)
    {
        bytesRecieved += ba.size()/2; // 1 byte = 2 symbols, 0xFF
    }
    else bytesRecieved = 0;

    updateProgressBar();

    commandWorker.parseAnswers(ba);

    while(commandWorker.haveAnswer())
    {
        DeviceAnswer recievedCommand = commandWorker.popAnswer();
        QList<QByteArray> parseResult = recievedCommand.parseResult();
        enableRecieve = recievedCommand.isEnableRecieve();

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
                        pushCommandToQueue("sw4 disable");
                    }

                    emit sgSetUIParameter("type_dev", deviceType);
                    emit sgSetUIParameter("set_max_map", controlledDevice.maxBankPresetCount()); //TODO for mobile?
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
                    qInfo() << recievedCommand.description();

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

                    qDebug() << "version control, minimal: " << controlledDevice.minimalFirmware()->firmwareVersion()
                               << " actual: " << controlledDevice.actualFirmware()->firmwareVersion();

                    if(*controlledDevice.minimalFirmware() > *controlledDevice.actualFirmware())
                    {
                        qWarning() << "firmware insufficient!";
                        emit sgFirmwareVersionInsufficient(controlledDevice.minimalFirmware(), controlledDevice.actualFirmware());
                    }
                    else
                    {
                        bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable").toBool();

                        if(isCheckUpdatesEnabled)
                        {
                           emit sgRequestNewestFirmware(deviceFirmware);
                        }
                    }
                    emit sgSetUIText("devVersion", deviceFirmware->firmwareVersion());
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
                                copiedPreset.setRawData(baPresetData);
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
                                m_presetListModel.updatePreset(currentPreset);
                                break;
                            }
                        }

                        deviceControls.setParametersFromRaw(baPresetData);
                        deviceControls.setAllUIValues();

                        emit sgSetUIParameter("preset_edited",   isPresetEdited);
                        emit sgSetUIParameter("editable",      bEditable); // TODO отличается от мобильного
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

                        emit sgSetUIParameter("bank",  currentPreset.bankNumber());
                        emit sgSetUIParameter("preset", currentPreset.presetNumber());

                        qInfo() << recievedCommand.description() << "bank:" << bank << "preset:" << preset;
                    }
                }
                break;
            }

            case AnswerType::getOutputMode:
            {
                if(parseResult.size()==1)
                {
                    quint8 mode = parseResult.at(0).toUInt();
                    emit sgSetUIParameter("output_mode",  mode);
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
                    Preset currentPreset(&controlledDevice);

                    currentPreset.setBankPreset(bankNumber, presetNumber);

                    if(presetNumber == controlledDevice.maxBankPresetCount()-1)
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
                    currentPreset.setImpulseName(ba.left(positionEndName+4));
                    impulsNames.append(ba.left(positionEndName+4));
                    impulsNames.append(",");
                    it++;

                    ba = *it;
                    currentPreset.setIsIrEnabled(ba.toInt());
                    impulsEn.append(ba);
                    impulsEn.append(",");
                    it++;

                    m_presetsList.append(currentPreset);
                }
                emit sgRefreshPresetList(&m_presetsList);
                emit sgSetUIParameter("open_preset_list", true);

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
                m_presetListModel.updatePreset(currentPreset);
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
                            emit sgSetUIParameter("ir_downloading", true);
                            break;
                        }

                        default:
                        {
                            currentPreset.setImpulseName(wavName);
                        }
                    }
                    enableRecieve = false; // команда cc обрабатывается двумя парсерами. Этим и getIr при полном выполнении
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
                timer->start();
                qInfo() << recievedCommand.description();
                break;
            }

            case AnswerType::ccAck:
            {
                emit sgSetUIParameter("ir_upload_finished", true);
                enableRecieve=false;
                timer->stop(); // wait for impulse apllying (TODO возможно по размеру импульса посчитать время сохранения в устройстве)
                qInfo() << recievedCommand.description();
                break;
            }

            case AnswerType::ccEND:
            {
                timer->start();

                qInfo() << recievedCommand.description();
                presetManager.returnToPreviousState();
                break;
            }

            case AnswerType::requestNextChunk:
            {
                if(!fwUpdate)
                {
                    enableRecieve = false;
                }
                qInfo() << recievedCommand.description();
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
                    emit sgSetUIParameter("slider_enabled", 1);
                }
                else
                {
                    isFormatting = false;
                    emit sgSetUIParameter("format_complete", true);
                }
                qInfo() << recievedCommand.description();
                emit sgSetUIParameter("slider_enabled", 1);
                break;
            }

            case AnswerType::ackPresetChange:
            {
                qInfo() << "Preset change, updating state";
                currentPreset.clearWavData();
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

        if(enableRecieve)
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
        emit sgSetUIParameter ("preset_edited", isPresetEdited);

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
        m_presetListModel.updatePreset(currentPreset);
    }

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

    processCommands();
}

void Core::slEscImpuls()
{
    pushCommandToQueue("lcc");
    pushCommandToQueue("rn");
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

void Core::uploadFirmware(QByteArray firmware)
{
    if(firmware.length()>0)
    {
        const uint32_t sizeBlock = 1024;

        emit sgSetUIParameter("fw_update_enabled", true);
        emit sgSetUIParameter("slider_enabled", 0);
        timer->setInterval(1000);

        fwUpdate = true;
        enableRecieve = false;

        QByteArray baSend;
        baSend.append("fwu\r");

        do
        {
            QByteArray baTmp;
            baTmp = firmware.left(sizeBlock);
            firmware.remove(0, baTmp.length()); // TODO: цикл for и передача firmware По константной ссылке

            baSend.append(QString("%1\n").arg(baTmp.length()).toUtf8());
            baSend.append(baTmp);

            commandsPending.append(baSend);
            baSend.clear();

        } while (firmware.length()>0);
        pushCommandToQueue("0\n"); // нахрена? но так было в изначальном коде
        processCommands();
    }
}

void Core::setPresetChange(quint8 inChangePreset)
{    
    if(isPresetEdited)
        emit sgPresetChangeStage(inChangePreset);
    else
    {
        doPresetChange(inChangePreset);
    }
}

void Core::doPresetChange(quint8 val)
{
    isPresetEdited = false;
    emit sgSetUIParameter ("preset_edited", isPresetEdited);
    emit sgSetUIParameter ("compare_state", false);

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
//            pushCommandToQueue("preset_delete_wavs");
        else
            pushCommandToQueue("dcc");

        currentPreset.clearWavData();
    }
    else
    {
        uploadImpulseData(currentPreset.waveData(), false, currentPreset.impulseName());
    }

    qDebug() << "Current preset bp: " << currentPreset.bankNumber() << ":" << currentPreset.presetNumber() << "impulse name:" << currentPreset.impulseName();

    m_presetListModel.updatePreset(currentPreset);
    isPresetEdited = false;
    emit sgSetUIParameter ("preset_edited",  isPresetEdited);
    bEditable = true;
    emit sgSetUIParameter ("editable",      bEditable);
}

void Core::comparePreset()
{
    if(presetManager.currentState() != PresetState::Compare)
    {
        emit sgSetUIParameter("compare_state", true);
        pushCommandToQueue("gs\r\n");
        pushCommandToQueue("esc\r\n");
    }
    else
    {
        presetManager.returnToPreviousState();
        setPresetData(currentPreset);
        uploadImpulseData(currentPreset.waveData(), true, currentPreset.impulseName());
        emit sgSetUIParameter("compare_state", false);
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
    emit sgSetUIParameter ("preset_edited", isPresetEdited);
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

    if(copiedPreset.waveData().isEmpty())
    {
        if(currentPreset.impulseName() != "")
        {
            copiedPreset.setWaveData(IRWorker::flatIr());
            copiedPreset.setImpulseName(QObject::tr(""));
        }
    }
    uploadImpulseData(copiedPreset.waveData(), true, copiedPreset.impulseName());
    currentPreset = copiedPreset;
    currentPreset.setBankPreset(currentBankNumber, currentPresetNumber);

    isPresetEdited = true;
    emit sgSetUIParameter ("preset_edited", isPresetEdited);
}

void Core::setParameter(QString name, quint8 value)
{
    qDebug()<<"setValue"<<name<<value;

    if(name==("preset_change"))
    {
        setPresetChange(value);
        qDebug()<<__FUNCTION__<<__LINE__;
    }

    if(name=="fw_update_complete")
    {
        fwUpdate = false;
        return;
    }

    if(name==("copy"))
    {
        copyPreset();
        return;
    }

    if(name==("paste"))
    {
        pastePreset();
        return;
    }

    if(name==("save_change"))
    {
        saveChanges();
        return;
    }

    if(name==("compare"))
    {
        comparePreset();
        return;
    }

    if(name==("set_preset_change"))
    {
        setPresetChange(value);
        return;
    }

    if(name==("do_preset_change"))
    {
        doPresetChange(value);
        return;
    }

    if(name == "call_preset_list")
    {
        pushCommandToQueue("rns");
        processCommands();
        return;
    }

    QString sendStr;

    if(name==("output_mode"))
        sendStr = QString("gm %1\r\n").arg(value, 0, 16);

    if(name==("format"))
    {
        emit sgSetUIParameter("wait", true);
        emit sgSetUIParameter("slider_enabled", 0);

        isFormatting = true;
        timer->setInterval(10000);

        sendStr = QString("fsf\r\n");
    }

    if(name==("esc"))
        sendStr = QString("esc\r\n");

    if(deviceControls.containsParameter(name))
    {
        sendStr = deviceControls.getParameterSendString(name, value);
        isPresetEdited = true;
        emit sgSetUIParameter ("preset_edited", isPresetEdited);
    }

    if(name=="cabinet_enable")
    {
        //TODO в device controls обновлять все поля пресета, а не только это
        currentPreset.setIsIrEnabled(value);
    }

    if(sendStr.length()>0)
    {
        enableRecieve = false;
        emit sgWriteToInterface(sendStr.toUtf8());
    }
}

void Core::restoreValue(QString name)
{
    Q_UNUSED(name)
    // TODO: добавить в Preset  объект DeviceControls. По имени находить позицию параметра в rawData и возвращать, устанавливая этот параметр
    // и отдельно обрабатывать случай банка и пресета
    // пока костыль для отмены сохранения пресета!!!!!

    if(name == "bank") emit sgSetUIParameter("bank", currentPreset.bankNumber());

    if(name == "preset") emit sgSetUIParameter("preset", currentPreset.presetNumber());
}

void Core::readAllParameters()
{
    isPresetEdited = false;

    pushCommandToQueue("amtdev");
    pushCommandToQueue("amtver");

    pushCommandToQueue("rns");

    pushReadPresetCommands();

    pushCommandToQueue("gm");

    processCommands();

    bEditable = true;
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
            chunckSize=500;
            sleepTime=75;
        }
        else
        {
            chunckSize=100;
            sleepTime=150;
        }

        if(commandToSend.length() > chunckSize)
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
            timer->stop();
        }
        else
        {
            quint32 timeoutInterval=1000;
            if(commandToSend.indexOf("rns\r\n")==0)
            {
                timeoutInterval = 10000;
            }
            timer->start(timeoutInterval);
        }
    }
    else
    {
        emit sgSetUIParameter("wait", false);
    }
}

void Core::recieveTimeout()
{
    if(isFormatting) // Format timeout
    {
        emit sgSetUIParameter("format_error", 1);
        emit sgSetUIParameter("slider_enabled", 1);
        isFormatting = false;
    }

    if(commandsSended.size()>0)
    {
        QByteArray commandWithoutAnswer = commandsSended.first();

        if(enableRecieve)
        {
            sendCount = 0;
        }
        else
        {
            sendCount++;
            qDebug() << "!!!!!!!!!!!!!!!!! recieve timeout !!!!!!!!!!!!!!!!!" << sendCount;
            sendCommand(commandWithoutAnswer);
            timer->setInterval(1000);

            if(sendCount>3)
            {
                sendCount = 0;

                if(fwUpdate)
                {
                    fwUpdate = false;
                    //TODO что за slider_enabled в мобильном? wait заменяет?
                    emit sgSetUIParameter("slider_enabled", 1);
                }

//                emit sgSetUIText("exchange_error", commandWithoutAnswer);
                emit sgExchangeError();

                commandsPending.clear();
                commandsSended.clear();
                return;
            }
        }
    }
}

void Core::sendCommand(QByteArray val)
{
    qDebug() << "<- writeData:" << val << "hex:" << val.toHex() << "lenght:" << val.length();

    symbolsSended += val.size();
    enableRecieve = false;
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
    timer->stop();
}
