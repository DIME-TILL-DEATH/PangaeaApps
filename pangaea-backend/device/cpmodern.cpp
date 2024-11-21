#include "cpmodern.h"

#include <QDir>
#include <QStandardPaths>

#include "core.h"

#include "eqband.h"

CPModern::CPModern(Core *parent)
    :AbstractDevice{parent}
{
    m_deviceClass = DeviceClass::CP_MODERN;

    m_avaliableOutputModes.append(QObject::tr("Phones"));
    m_avaliableOutputModes.append(QObject::tr("Line"));
    m_avaliableOutputModes.append(QObject::tr("Balanced"));
    m_avaliableOutputModes.append(QObject::tr("L: Processed/R: Monitor"));

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtver", std::bind(&CPModern::amtVerCommHandler, this, _1, _2));

    m_parser.addCommandHandler("rn", std::bind(&CPModern::getIrNameCommHandler, this, _1, _2));
    m_parser.addCommandHandler("rns", std::bind(&CPModern::getIrListCommHandler, this, _1, _2));

    m_parser.addCommandHandler("gb", std::bind(&CPModern::getBankPresetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gm", std::bind(&CPModern::getOutputModeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("state", std::bind(&CPModern::stateCommHandler, this, _1, _2));

    m_parser.addCommandHandler("sp", std::bind(&CPModern::ackSaveChanges, this, _1, _2));
    m_parser.addCommandHandler("pc", std::bind(&CPModern::ackPresetChangeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("esc", std::bind(&CPModern::ackEscCommHandler, this, _1, _2));

    m_parser.addCommandHandler("SYNC ERROR", std::bind(&CPModern::errorCCCommHandler, this, _1, _2));

    m_parser.addCommandHandler("REQUEST_CHUNK_SIZE", std::bind(&CPModern::requestNextChunkCommHandler, this, _1, _2));
    m_parser.addCommandHandler("FR_OK", std::bind(&CPModern::fwuFinishedCommHandler, this, _1, _2));
    m_parser.addCommandHandler("fsf", std::bind(&CPModern::formatFinishedCommHandler, this, _1, _2));

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif
}

void CPModern::initDevice(DeviceType deviceType)
{
    m_deviceType = deviceType;
    setDeviceType(m_deviceType);

    MV = new PresetVolume(this);

    NG = new NoiseGate(this);
    CM = new Compressor(this);
    PR = new Preamp(this);
    PA = new PowerAmp(this);
    IR = new CabSim(this);
    EQ = new EqParametric(this, EqParametric::EqMode::Modern, 0);
    ER = new EarlyReflections(this);

    m_moduleList.append(NG);
    m_moduleList.append(CM);
    m_moduleList.append(PR);
    m_moduleList.append(PA);
    m_moduleList.append(IR);
    m_moduleList.append(EQ);
    m_moduleList.append(ER);

    m_modulesListModel.refreshModel(&m_moduleList);

    emit modulesListModelChanged();
    emit presetListModelChanged();
    emit sgDeviceInstanciated();
}

void CPModern::setDeviceType(DeviceType newDeviceType)
{
    m_deviceType = newDeviceType;

    if(m_minimalFirmware != nullptr)
    {
        delete(m_minimalFirmware);
        m_minimalFirmware = nullptr;
    }

    switch(m_deviceType)
    {
    case DeviceType::modernCP:
        m_minimalFirmware = new Firmware("2.00.02", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16Modern.ble");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "CP16 Modern";
        // m_isPaFw = true;
        break;
    default:
        qWarning() << __FUNCTION__ << "Unknown device type";

        m_minimalFirmware = new Firmware("2.00.01", newDeviceType, FirmwareType::DeviceInternal, "");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "Unknown CP device";
        break;
        break;
    }
}

void CPModern::readFullState()
{
    m_presetManager.setCurrentState(PresetState::Changing);
    m_parser.enableFullEndMode();
    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("rns");

    emit sgPushCommandToQueue("gm");

    pushReadPresetCommands();

    emit sgProcessCommands();
}

void CPModern::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("gb");
    emit sgPushCommandToQueue("rn");
    emit sgPushCommandToQueue("state");
}

QList<QByteArray> CPModern::parseAnswers(QByteArray &baAnswer)
{
    QList<QByteArray> recievedCommAnswers;

    recievedCommAnswers += m_parser.parseNewData(baAnswer);

    // if(getIrNameSize.getParse(baAnswer, &parseResults))
    // {
    //     getIrNameSizeCommHandler(parseResults); // это лишь начало команды. Не считаем что она выполнена
    //     recievedCommAnswers.removeAll(QString("cc").toUtf8());

    // }
    // if(getIr.getParse(baAnswer, &parseResults))
    // {
    //     getIrCommHandler(parseResults);
    //     recievedCommAnswers.append("cc");
    // }
    // if(ackCC.getParse(baAnswer, &parseResults))
    // {
    //     ackCCCommHandler(parseResults);
    //     recievedCommAnswers.append("cc");
    // }
    // if(endCC.getParse(baAnswer, &parseResults))
    // {
    //     endCCCommHandler(parseResults);
    //     recievedCommAnswers.append("cc");
    // }

    QList<QByteArray> args;
    if(undefCommParser.getParse(baAnswer, &args))
    {
        QByteArray data;
        if(!args.isEmpty()) data = args.first();
        undefinedCommandCommHandler("", data);
    }

    return recievedCommAnswers;
}

void CPModern::saveChanges()
{
    qInfo()<<__FUNCTION__;

    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    emit sgPushCommandToQueue("sp");
    emit sgProcessCommands();

    // impulse data uploading after answer
    if(actualPreset.waveData() == IRWorker::flatIr())
    {
        // if(m_deviceType==DeviceType::legacyCP16 || m_deviceType==DeviceType::legacyCP16PA)
            emit sgPushCommandToQueue("pwsd");
        // else
        //     emit sgPushCommandToQueue("dcc");

        actualPreset.clearWavData();
    }
    else
    {
        uploadImpulseData(actualPreset.waveData(), false, actualPreset.impulseName());
    }

    savedPreset = actualPreset;
    m_presetListModel.updatePreset(savedPreset);

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
    emit deviceUpdatingValues(); // clear modified marks on control values
}

void CPModern::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if((newPreset == m_preset) && (newBank == m_bank)) return;

    quint8 val = Preset::calcPresetFlatIndex(m_deviceType, newBank, newPreset);


    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(savedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pc %2").arg(val, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void CPModern::comparePreset()
{
    if(m_presetManager.currentState() != PresetState::Compare)
    {
        emit sgPushCommandToQueue("state");
        emit sgPushCommandToQueue("esc");
    }
    else
    {
        m_presetManager.returnToPreviousState();
        setPresetData(actualPreset);
        uploadImpulseData(actualPreset.waveData(), true, actualPreset.impulseName());
    }
    emit sgProcessCommands();
}

void CPModern::copyPreset()
{
    m_presetManager.setCurrentState(PresetState::Copying);

    if(actualPreset.wavSize() == 0)
        emit sgPushCommandToQueue("cc");

    emit sgPushCommandToQueue("state");
    emit sgProcessCommands();
}

void CPModern::pastePreset()
{
    setPresetData(copiedPreset);

    quint8 currentBankNumber = actualPreset.bankNumber();
    quint8 currentPresetNumber = actualPreset.presetNumber();

    // TODO для исправления бага с обновлением пресета после вставки
    // эта часть должна быть в getStatus по ключу PresetState::Pasting
    // но это немного костыльно и MAP для актуального пресета должен обновляться
    // и хранится как-то по-другому. При этом при переключении применятся старый
    if(copiedPreset.waveData().isEmpty())
    {
        if(actualPreset.impulseName() != "")
        {
            copiedPreset.setWaveData(IRWorker::flatIr());
            copiedPreset.setImpulseName("");
        }
    }
    uploadImpulseData(copiedPreset.waveData(), true, copiedPreset.impulseName());
    actualPreset = copiedPreset;
    actualPreset.setBankPreset(currentBankNumber, currentPresetNumber);

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPModern::importPreset(QString filePath, QString fileName)
{
    Q_UNUSED(fileName)

    Preset importedPreset{this};

    if(!importedPreset.importData(filePath))
    {
        emit sgDeviceError(DeviceErrorType::PresetImportUnsuccesfull);
        return;
    }

    setPresetData(importedPreset);

    if(importedPreset.waveData().isEmpty())
    {
        if(actualPreset.impulseName() != "")
        {
            importedPreset.setWaveData(IRWorker::flatIr());
            importedPreset.setImpulseName("");
        }
    }
    uploadImpulseData(importedPreset.waveData(), true, importedPreset.impulseName());
    actualPreset = importedPreset;

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPModern::exportPreset(QString filePath, QString fileName)
{
    if(m_presetManager.currentState() != PresetState::Exporting)
    {
        QString folderPath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/presets/";

        qInfo() << __FUNCTION__ << "Preset name: " << fileName;

        m_presetManager.setCurrentState(PresetState::Exporting);

        if(!QDir(folderPath).exists())
        {
            QDir().mkpath(folderPath);
        }

        emit sgPushCommandToQueue("state");
        actualPreset.setPathToExport(filePath);
        emit sgPushCommandToQueue("cc");
        emit sgProcessCommands();
    }
}

void CPModern::restoreValue(QString name)
{
    //  TODO пока костыль для отмены сохранения пресета!!!!!
    if(name == "bank-preset")
    {
        m_bank = actualPreset.bankNumber();
        m_preset = actualPreset.presetNumber();
        emit bankPresetChanged();
    }
}

void CPModern::setImpulse(QString filePath)
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
        qWarning() << __FUNCTION__ << "Not supported wav format";
        emit sgDeviceError(DeviceErrorType::IrFormatNotSupported, QString().setNum(wavHead.sampleRate)+" Hz/"+
                                                                      QString().setNum(wavHead.bitsPerSample)+" bits/"+
                                                                      QString().setNum(wavHead.numChannels)+" channel");
    }
    else
    {
        irWorker.decodeWav(filePath);

        QByteArray fileData = irWorker.formFileData();
        actualPreset.setWaveData(fileData);
        uploadImpulseData(fileData, true, fileName);
    }
}

void CPModern::uploadImpulseData(const QByteArray &impulseData, bool isPreview, QString impulseName)
{
    if(impulseData.isEmpty())
    {
        qInfo() << __FUNCTION__ << "no wave data";
        return;
    }

    m_presetManager.setCurrentState(PresetState::UploadingIr);

    impulseName.replace(QString(" "), QString("_"), Qt::CaseInsensitive);

    actualPreset.setImpulseName(impulseName);

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
        actualPreset.setImpulseName(impulseName);
    }
    m_presetListModel.updatePreset(actualPreset);

    for(quint16 i=0; i<bytesToUpload; i++)
    {
        QString sTmp;
        quint8  chr;
        if(i>=irData.length())
            sTmp = QString("00");
        else
        {
            chr = irData.at(i);
            sTmp = QString("%1").arg (chr, 2, 16, QChar('0'));
        }
        baSend.append(sTmp.toUtf8());
    }
    emit sgPushCommandToQueue(baSend);
    emit sgProcessCommands();

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    IR->setImpulseName(impulseName);
    IR->setEnabled(true); // загрузка импульса автоматом включает модуль в устройстве, отобразить это визуально
}

void CPModern::escImpulse()
{
    emit sgPushCommandToQueue("lcc");
    emit sgPushCommandToQueue("rn");
    emit sgProcessCommands();
}

void CPModern::setPresetData(const Preset &preset)
{
    QByteArray ba;

    ba.append("state wr\r");
    ba.append(preset.rawData());

    emit sgPushCommandToQueue(ba);
    emit sgPushCommandToQueue("state"); // read settled state
    emit sgProcessCommands();
}

void CPModern::setFirmware(QString fullFilePath)
{
    qDebug()<< __FUNCTION__ << "fullFilePath" << fullFilePath;

    emit sgDeviceMessage(DeviceMessageType::FirmwareFilePath, fullFilePath);

    if(!Firmware::isFirmwareFile(fullFilePath))
    {
        qWarning() << __FUNCTION__ << "Not firmware file";
        emit sgDeviceError(DeviceErrorType::FirmwareFileError, "Not firmware file.");
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
        qWarning() << __FUNCTION__ << __LINE__ << "Can not open file " << fullFilePath;
        emit sgDeviceError(DeviceErrorType::FirmwareFileError, "Can't open file.");
    }
}

void CPModern::uploadFirmware(const QByteArray &firmware)
{
    if(firmware.length()>0)
    {
        m_rawFirmwareData = firmware;

        qint64 symbolsToSend = m_rawFirmwareData.size() + 4 * m_rawFirmwareData.size() / fwUploadBlockSize + 2; // 128\n = 4 * (parts num and 0\n = 2
        qint64 symbolsToRecieve = 0;// 18 * m_rawFirmwareData.size() / fwUploadBlockSize; // REQUEST_CHUNK_SIZE\n = 18

        emit sgDeviceMessage(DeviceMessageType::FirmwareUpdateStarted);
        emit sgDisableTimeoutTimer();

        fwUpdate = true;

        QByteArray baTmp, baSend;
        baSend.append("fwu\r");

        baTmp = m_rawFirmwareData.left(fwUploadBlockSize);
        m_rawFirmwareData.remove(0, baTmp.length());

        baSend.append(QString("%1\n").arg(baTmp.length()).toUtf8());
        baSend.append(baTmp);

        emit sgSendWithoutConfirmation(baSend, symbolsToSend, symbolsToRecieve);
        emit sgProcessCommands();
    }
}

void CPModern::formatMemory()
{
    emit sgDeviceMessage(DeviceMessageType::FormatMemoryStarted);

    isFormatting = true;
    emit sgDisableTimeoutTimer();
    emit sgSendWithoutConfirmation(QString("fsf\r\n").toUtf8());
    emit sgProcessCommands();
}

//=======================================================================================
//                  ***************Comm handlers************
//=======================================================================================

void CPModern::amtVerCommHandler(const QString &command, const QByteArray &arguments)
{
    QString firmwareVersion = arguments;

    m_firmwareName += " v." + firmwareVersion;
    emit firmwareNameChanged();

    qInfo() << __FUNCTION__ << firmwareVersion;

    m_actualFirmware = new Firmware(firmwareVersion, m_deviceType, FirmwareType::DeviceInternal, "device:/internal");

    qInfo() << "Firmware name: " << m_firmwareName
            << ", version control, minimal: " << m_minimalFirmware->firmwareVersion()
            << " actual: " << m_actualFirmware->firmwareVersion();

    if(*m_actualFirmware > *m_minimalFirmware)
    {
        bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable").toBool();

        if(isCheckUpdatesEnabled)
        {
            // emit sgRequestNewestFirmware(deviceFirmware);
        }
    }
    else
    {
        qWarning() << "firmware insufficient!";
        emit sgDeviceError(DeviceErrorType::FimrmwareVersionInsufficient, "", {m_actualFirmware->firmwareVersion(), m_minimalFirmware->firmwareVersion()});
    }
}

void CPModern::getBankPresetCommHandler(const QString &command, const QByteArray &arguments)
{
    m_bank = arguments.left(2).toUInt();
    m_preset  = arguments.right(2).toUInt();

    actualPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
    emit bankPresetChanged();
}

void CPModern::getOutputModeCommHandler(const QString &command, const QByteArray &arguments)
{
    quint8 mode = arguments.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

//-------------------------------------acknowledegs------------------------------------------
void CPModern::ackEscCommHandler(const QString &command, const QByteArray &arguments)
{
    m_presetManager.setCurrentState(PresetState::Compare);
    setPresetData(savedPreset);
    emit sgPushCommandToQueue("rn");
    emit sgProcessCommands();
}

void CPModern::ackSaveChanges(const QString &command, const QByteArray &arguments)
{
    actualPreset.clearWavData();
}

void CPModern::ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments)
{
    actualPreset.clearWavData();
    m_presetManager.setCurrentState(PresetState::Changing);
    pushReadPresetCommands();
    emit sgProcessCommands();
}

void CPModern::requestNextChunkCommHandler(const QString &command, const QByteArray &arguments)
{
    qDebug() << "remaining part " << m_rawFirmwareData.size();

    if(m_rawFirmwareData.length() > 0)
    {
        QByteArray baSend, baTmp;
        baTmp = m_rawFirmwareData.left(fwUploadBlockSize);
        m_rawFirmwareData.remove(0, baTmp.length());

        baSend.append(QString("%1\n").arg(baTmp.length()).toUtf8());
        baSend.append(baTmp);

        emit sgSendWithoutConfirmation(baSend);
    }
    else
    {
        // нулевой блок это знак что файл загружен
        emit sgSendWithoutConfirmation("0\n", false);
    }
    emit sgProcessCommands();
}

void CPModern::fwuFinishedCommHandler(const QString &command, const QByteArray &arguments)
{
    if(fwUpdate)
    {
        emit sgDeviceMessage(DeviceMessageType::FirmwareUpdateFinished);
        fwUpdate = false;
    }
}

void CPModern::formatFinishedCommHandler(const QString &command, const QByteArray &arguments)
{
    qDebug() << __FUNCTION__;
    emit sgDeviceMessage(DeviceMessageType::FormatMemoryFinished);
    isFormatting = false;
}


void CPModern::stateCommHandler(const QString &command, const QByteArray &arguments)
{
    if(arguments.size() < sizeof(preset_data_t)*2) return;

    QByteArray baPresetData = arguments;

    quint8 count=0;
    quint8 nomByte=0;
    QString curByte;

    quint8 dataBuffer[sizeof(preset_data_t)*2];

    foreach(QChar val, baPresetData) //quint8
    {
        if((nomByte&1)==0)
        {
            curByte.clear();
            curByte.append(val);
        }
        else
        {
            curByte.append(val);
            dataBuffer[count] = curByte.toInt(nullptr, 16);
            count++;
        }
        nomByte++;
    }

    preset_data_t presetData;
    memcpy(&presetData, dataBuffer, sizeof(preset_data_t));

    MV->setValue(presetData.volume);

    NG->setValues(presetData.gate);
    CM->setValues(presetData.compressor);
    PR->setValues(presetData.preamp);
    PA->setValues(presetData.power_amp);
    EQ->setEqData(presetData.eq1);
    IR->setEnabled(presetData.cab_sim_on);
    ER->setValues(presetData.reverb);

    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        // copy preview wave data
        if(actualPreset.wavSize() != 0)
            copiedPreset = actualPreset;

        copiedPreset.setRawData(baPresetData);
        m_presetManager.returnToPreviousState();
        emit presetCopied();
        break;
    }

    case PresetState::Changing:
    {
        m_deviceParamsModified = false;
        emit deviceParamsModifiedChanged();
        emit deviceUpdatingValues();

        actualPreset.setRawData(baPresetData);
        savedPreset = actualPreset;
        m_presetListModel.updatePreset(savedPreset);
        m_presetManager.returnToPreviousState();
        emit presetSwitched();
        break;
    }

    case PresetState::Compare:
    {
        // Необходимая заглушка! не удалять
        break;
    }

    default:
    {
        actualPreset.setRawData(baPresetData);
        m_presetListModel.updatePreset(actualPreset);
        break;
    }
    }
}

//===========================================================
// Legacy comm handlers
//========================================================
void CPModern::getCCCommHandler(const QList<QByteArray> &arguments)
{
    QByteArray impulseData;
    if(arguments.length()==2)
    {
        impulseData = QByteArray::fromHex(arguments.at(1));
    }

    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        copiedPreset.setWaveData(impulseData);
        break;
    }

    case PresetState::Exporting:
    {
        actualPreset.setWaveData(impulseData);
        actualPreset.exportData();

        emit sgDeviceMessage(DeviceMessageType::PresetExportFinished, actualPreset.pathToExport());

        m_presetManager.returnToPreviousState();
        break;
    }

    default:{}
    }

    m_bytesToRecieve=0;

    emit sgEnableTimeoutTimer();
}

void CPModern::getIrNameCommHandler(const QString &command, const QByteArray &arguments)
{
    QString impulseName;

    if(arguments.size()>0)
        impulseName = arguments;
    else
        impulseName = "";

    switch(m_presetManager.currentState())
    {
    case PresetState::Compare:
    {
        IR->setImpulseName(impulseName);
        // Необходимая заглушка. Не удалять!
        break;
    }

    case PresetState::Changing:
    {
        actualPreset.setImpulseName(impulseName);
        savedPreset.setImpulseName(impulseName);
        IR->setImpulseName(impulseName);
    }

    default:
    {
        actualPreset.setImpulseName(impulseName);
        IR->setImpulseName(impulseName);
    }
    }

    m_presetListModel.updatePreset(actualPreset);
}

void CPModern::getIrListCommHandler(const QString &command, const QByteArray &arguments)
{
    m_parser.disableFullEndMode();

    QString fullList = arguments;

    QStringList separatedList = fullList.split("\n");

    m_presetsList.clear();

    quint8 bankNumber=0;
    quint8 presetNumber=0;

    QStringList::const_iterator it = separatedList.constBegin();
    while (it != separatedList.constEnd())
    {
        Preset currentListPreset(this);

        currentListPreset.setBankPreset(bankNumber, presetNumber);

        QString stringResult = *it;
        if(stringResult=="*")
            stringResult="";
        quint16 positionEndName = stringResult.lastIndexOf(".wav ");
        currentListPreset.setImpulseName(stringResult.left(positionEndName+4));
        it++;

        stringResult = *it;
        currentListPreset.setIsIrEnabled(stringResult.toInt());
        it++;

        m_presetsList.append(currentListPreset);

        if(presetNumber == m_maxPresetCount-1)
        {
            presetNumber = 0;
            bankNumber++;
        }
        else
        {
            presetNumber++;
        }
    }

    m_presetListModel.refreshModel(&m_presetsList);
}

void CPModern::getCCNameSizeCommHandler(const QList<QByteArray> &arguments)
{
    if(arguments.length()==1)
    {
        QByteArray baAnswer = arguments.at(0);
        QString wavName;
        int wavSize=0;
        if(baAnswer.indexOf("FILE_NOT_FIND") == 0)
        {
            wavName= "";
            return;
        }
        else
        {
            quint16 positionEndName = baAnswer.lastIndexOf(".wav");
            wavName = QString::fromUtf8(baAnswer.left(positionEndName+4));
            wavName.replace(" ","_");

            QByteArray baWavSize = baAnswer.mid(positionEndName+5);
            wavSize = baWavSize.toInt();
        }

        switch(m_presetManager.currentState())
        {
        //Повтор необходим. Надо чтобы обрабатывало cc/r ТОЛЬКО в состоянии копирования и экспорта
        case PresetState::Copying:
        {
            copiedPreset.setImpulseName(wavName);

            m_bytesToRecieve = wavSize;
            qDebug() << "Recieve Ir. Name:" << wavName << ", size:" << wavSize;
            break;
        }

        case PresetState::Exporting:
        {
            m_bytesToRecieve = wavSize;
            actualPreset.setImpulseName(wavName);
            break;
        }

        default:
        {
            actualPreset.setImpulseName(wavName);
        }
        }

        emit m_presetManager.currentStateChanged(); // Для того чтобы экран BUSY правильно отобразил стадию этапа
        emit sgDisableTimeoutTimer();
    }
}

void CPModern::ackCCCommHandler(const QList<QByteArray> &arguments)
{
    m_presetManager.returnToPreviousState();
    m_presetManager.setCurrentState(PresetState::SavingIr);
    emit sgDisableTimeoutTimer(); // wait for impulse saving (TODO возможно по размеру импульса посчитать время сохранения в устройстве)
}

void CPModern::endCCCommHandler(const QList<QByteArray> &arguments)
{
    emit sgEnableTimeoutTimer();
    m_presetManager.returnToPreviousState();

    emit impulseUploaded();
}

void CPModern::errorCCCommHandler(const QString &command, const QByteArray &arguments)
{
    emit sgEnableTimeoutTimer();
    m_presetManager.returnToPreviousState();
    emit sgDeviceError(DeviceErrorType::IrSaveError, actualPreset.impulseName());
}
