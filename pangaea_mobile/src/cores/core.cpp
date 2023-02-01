#include <QDebug>
#include <QThread>

#include <QStandardPaths>

#include "core.h"
#include "qdir.h"

Core::Core(BluetoothleUART *bleConnection, QObject *parent) :
    QObject(parent),
    m_bleConnection{bleConnection}
{
#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif

    QObject::connect(&deviceControls, &DeviceControls::sgSetInterfaceValue, this, &Core::sgSetUIParameter);

    timer = new QTimer(this);
    timer->setInterval(1000);
    QObject::connect(timer, &QTimer::timeout, this, &Core::recieveTimeout);
    timer->start();

    m_presetListModel.refreshModel(&m_presetsList);
    connect(this, &Core::sgRefreshPresetList, &m_presetListModel, &PresetListModel::refreshModel, Qt::QueuedConnection);

    // TODO работа с абстрактным интерфейсом
    connect(m_bleConnection, &BluetoothleUART::changedState, this, &Core::slChangedState);
    connect(m_bleConnection, &BluetoothleUART::sgErrorDisconnected, this, &Core::slReadyToDisconnect);
}

void Core::registerQmlObjects(QQmlContext *qmlContext)
{
    qmlContext->setContextProperty("_presetListModel", &m_presetListModel);
}

// Отличие от стационарного
void Core::slChangedState(BluetoothleUART::bluetoothleState state)
{
    switch(state){
        case BluetoothleUART::UpdateDeviceList:
        {
            m_bleConnection->getDeviceList(m_qlFoundDevices);
            emit sgUpdateBLEDevicesList(m_qlFoundDevices);

            m_bleConnection->prevState();
            break;
        }

        case BluetoothleUART::UpdateModuleName:
        {
            emit sgModuleNameUpdated(m_bleConnection->moduleName());
        }
        break;

        case BluetoothleUART::AcquireData:
        {
            connect(m_bleConnection, &BluetoothleUART::newData, this, &Core::parseInputData);
            isConnected = true;
            fwUpdate = false;

            emit sgConnectReady();
            break;
        }

        default: break;
    }
}

void Core::slReadyToDisconnect()
{
    commandsPending.clear();
    enableRecieve = true;
    isConnected = false;    
    emit sgSetUIParameter("ready_to_disconnect", true);
}

void Core::parseInputData(const QByteArray &ba)
{
    qDebug() << "->" << __FUNCTION__ << ":" << ba << "\nCommands sended:" << commandsSended.count();

    if(bytesToRecieve > 0)
    {
        bytesRecieved += ba.size()/2; // 1 byte = 2 symbols, 0xFF
    }
    else bytesRecieved = 0;

    updateProgressBar();

    commandWorker.parseAnswers(ba);

    while (commandWorker.haveAnswer())
    {
        DeviceAnswer recievedCommand = commandWorker.popAnswer();
        QList<QByteArray> parseResult = recievedCommand.parseResult();
        enableRecieve = recievedCommand.isEnableRecieve();

        switch(recievedCommand.answerType())
        {
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

        case AnswerType::getAMTDevType:
        {
            if(parseResult.size()==1)
            {
                DeviceType deviceType = static_cast<DeviceType>(parseResult.at(0).toUInt());
                controlledDevice.setDeviceType(deviceType);

                emit sgSetUIParameter("type_dev", deviceType);
                emit sgSetUIParameter("set_max_map", controlledDevice.maxBankPresetCount());
                qInfo() << recievedCommand.description();
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
                            break;
                        }
                    }

                    deviceControls.setParametersFromRaw(baPresetData);
                    deviceControls.setAllUIValues();

                    emit sgSetUIParameter ("preset_edited",   isPresetEdited);
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

        case AnswerType::getIrName:
        {
            QString impulseName;

            if(parseResult.size()>0)
                impulseName = recievedCommand.parseResult().at(0);
            else
                impulseName = QObject::tr("empty");

            switch(presetManager.currentState())
            {
                case PresetState::Compare:
                {
                    // Необходимая заглушкаю Не удалять!
                    break;
                }

                default:
                {
                    currentPreset.setImpulseName(impulseName);
                }
            }
            emit sgSetUIText("impulse_name", impulseName);
            qInfo() << recievedCommand.description() << "impulse name:" << impulseName;
            break;
        }

        case AnswerType::getIrList:
        {
            qInfo() << recievedCommand.description();

            m_presetsList.clear();

            QList<QByteArray>::const_iterator it = parseResult.constBegin();
            while (it != parseResult.constEnd())
            {
                Preset currentPreset(&controlledDevice);

                // TODO использовать стандартный запрос листа, без *
                QByteArray ba = *it;
                currentPreset.setBankPreset(ba.left(1).toInt(nullptr, 16), ba.right(1).toInt(nullptr, 16)); //TODO убрать * в rns. Добавить парсер для CP100
                it++;

                ba = *it;
                if(ba=="*")
                    ba="empty";
                quint16 positionEndName = ba.lastIndexOf(".wav ");
                currentPreset.setImpulseName(ba.left(positionEndName+4));
                it++;

                ba = *it;
                currentPreset.setIsIrEnabled(ba.toInt());
                it++;

                m_presetsList.append(currentPreset);
            }

            emit sgRefreshPresetList(&m_presetsList);
            emit sgSetUIParameter("open_preset_list", 1);

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
            qInfo() << recievedCommand.description();
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
            pushCommandToQueue("gs");
            pushCommandToQueue("rn");
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
            readPresetState();
            break;
        }

        case AnswerType::ackSavePreset:
        {
            qInfo() << "Preset saved to device";
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

void Core::setImpulse(QString filePath, QString fileName)
{
    stWavHeader wavHead = IRWorker::getFormatWav(filePath);

    if((wavHead.sampleRate != 48000) || (wavHead.bitsPerSample != 24) || (wavHead.numChannels != 1))
    {
        qDebug() << __FUNCTION__ << "Not supported wav format";
        emit sgSetUIText("not_supported_ir",
                         QString::number(wavHead.sampleRate)+","+
                         QString::number(wavHead.bitsPerSample)+","+
                         QString::number(wavHead.numChannels));
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

void Core::uploadImpulseData(const QByteArray &impulseData, bool isPreview, QString impulseName)
{
    if(impulseData.isEmpty())
    {
        qDebug() << __FUNCTION__ << "no wave data";
        return;
    }

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

void Core::escImpulse()
{
    pushCommandToQueue("lcc");
    pushCommandToQueue("rn");
    processCommands();
}

void Core::setFirmware(QString fullFilePath)
{
    qDebug()<< __FUNCTION__ << "fullFilePath" <<fullFilePath;

    emit sgSetUIText("firmware_path", fullFilePath);

    QFile *file = new QFile(fullFilePath.toUtf8());

    if(file != nullptr)
    {
        if(file->open(QIODevice::ReadOnly))
        {
            QByteArray baFW = file->read(file->size());
            uploadFirmware(baFW);

            file->close();
        }
        else
        {
            qDebug() << __FUNCTION__ << __LINE__ << "Can not open file " << fullFilePath;
            emit sgSetUIText("open_fw_file_error", fullFilePath);
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << __LINE__ << "Can not open file " << fullFilePath;
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

    // TODO реализовать логику как в стационарном. Загрузка IR после подтверждения сохранения.

    if(currentPreset.waveData() == IRWorker::flatIr())
    {
        pushCommandToQueue("preset_delete_wavs");
        pushCommandToQueue("dcc");  // перестраховку для старых версий прошивок
        processCommands();
    }
    else
    {
        uploadImpulseData(currentPreset.waveData(), false, currentPreset.impulseName());
    }
    saveAndUpdatePreset();
}

void Core::saveAndUpdatePreset()
{
    isPresetEdited = false;
    emit sgSetUIParameter ("preset_edited",  isPresetEdited);

    currentPreset.clearWavData();

    pushCommandToQueue("sp");
    pushCommandToQueue("rn");
    pushCommandToQueue("gs");
    processCommands();
}

void Core::comparePreset()
{
    if(presetManager.currentState() != PresetState::Compare)
    {
        emit sgSetUIParameter("compare_state", true);
        pushCommandToQueue("gs");
        pushCommandToQueue("esc");
    }
    else
    {
        setPresetData(currentPreset);
        uploadImpulseData(currentPreset.waveData(), true, currentPreset.impulseName());
        emit sgSetUIParameter("compare_state", false);
        presetManager.returnToPreviousState();
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
        if(currentPreset.impulseName() != QObject::tr("empty"))
        {
            importedPreset.setWaveData(IRWorker::flatIr());
            importedPreset.setImpulseName(QObject::tr("empty"));
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

        //currentPreset.setPathToExport(folderPath + fileName + ".pst");
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

    if(copiedPreset.waveData().isEmpty())
    {
        if(currentPreset.impulseName() != QObject::tr("empty"))
        {
            copiedPreset.setWaveData(IRWorker::flatIr());
            copiedPreset.setImpulseName(QObject::tr("empty"));
        }
    }
    uploadImpulseData(copiedPreset.waveData(), true, copiedPreset.impulseName());
    currentPreset = copiedPreset;

    isPresetEdited = true;
    emit sgSetUIParameter ("preset_edited", isPresetEdited);
}

void Core::setParameter(QString name, quint8 value)
{
    qDebug()<<"setValue"<<name<<value;

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
        pushCommandToQueue("rns *"); //TODO использовать обычную команду, добавить парсер для CP100
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

    if(sendStr.length()>0)
    {
        if(isConnected)
        {
            enableRecieve = false;
            m_bleConnection->writeData(sendStr.toUtf8());
        }
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

    pushCommandToQueue("sw4 disable");
    pushCommandToQueue("amtdev");
    pushCommandToQueue("amtver");

    readPresetState();

    pushCommandToQueue("gm");

    processCommands();
}

void Core::readPresetState()
{
    pushCommandToQueue("gb");
    pushCommandToQueue("rn");
    pushCommandToQueue("gs");
}

void Core::processCommands()
{
    if(commandsPending.size()>0)
    {
        QByteArray commandToSend = commandsPending.takeFirst();

        emit sgSetUIParameter("wait", true);

#ifdef Q_OS_ANDROID
        //TODO iOS?
        int chunckSize=100;
        int sleepTime=200;
#else
        int chunckSize=500;
        int sleepTime=75;
#endif

        if(commandToSend.length() > chunckSize)
        {
            for(int sendPosition=0; sendPosition < commandToSend.length(); sendPosition += chunckSize)
            {
                sendCommand(commandToSend.mid(sendPosition, chunckSize));
                QThread::msleep(sleepTime);
            }
        }
        else
        {
            sendCommand(commandToSend);
        }
        commandsSended.append(commandToSend);
        //------------------------------------------------------------------
//        quint32 timeoutInterval;
//        if(commandToSend.indexOf("rns *\r\n")==0)
//        {
//            timeoutInterval = 5000;
//        }
//        else if(commandToSend.indexOf("cc\r\n")==0)
//        {
//            // TODO: таймаут должен рассчитываться исходя из размера импульса
//            timeoutInterval = 150000;
//        }
//        else
//        {
//            timeoutInterval = 1000;
//        }
//        timer->setInterval(timeoutInterval);
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
            qDebug() << "!!!!!!!!!!!!!!!!! sendCount !!!!!!!!!!!!!!!!!" << sendCount;
            sendCommand(commandWithoutAnswer);

            if(sendCount>3)
            {
                sendCount = 0;

                if(fwUpdate)
                {
                    fwUpdate = false;
                    emit sgSetUIParameter("slider_enabled", 1); // TODO interface active???? Or device???
                }

                emit sgSetUIText("exchange_error", commandWithoutAnswer);
                commandsPending.clear();
                commandsSended.clear();
                return;
            }
        }
    }
}

void Core::sendCommand(QByteArray val)
{
    if(isConnected)
    {
        qDebug() << "<- writeData:" << val << "hex:" << val.toHex() << "lenght:" << val.length();

        symbolsSended += val.size();
        enableRecieve = false;
        m_bleConnection->writeData(val);

        updateProgressBar();
    }
}

void Core::updateProgressBar()
{
    float fVal = (double)(symbolsSended+bytesRecieved) / (double)(symbolsToSend+bytesToRecieve);
    emit sgSetProgress(fVal, "");
}

void Core::sw4Enable()
{
    pushCommandToQueue("sw4 enable");
    processCommands();
}

void Core::stopTimer()
{
    timer->stop();
}

Core::~Core()
{
    if(deviceFirmware != nullptr) delete deviceFirmware;
}
