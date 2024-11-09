#include "cplegacy.h"

#include <QDir>
#include <QStandardPaths>

#include "core.h"

#include "eqband.h"

CPLegacy::CPLegacy(Core *parent)
    : AbstractDevice{parent}
{
    m_deviceClass = DeviceClass::CP_LEGACY;

    m_avaliableOutputModes.append(QObject::tr("Phones"));
    m_avaliableOutputModes.append(QObject::tr("Line"));
    m_avaliableOutputModes.append(QObject::tr("Balanced"));
    // m_avaliableOutputModes.append(QObject::tr("L: Processed/R: Monitor"));

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtver", std::bind(&CPLegacy::amtVerCommHandler, this, _1, _2));

    m_parser.addCommandHandler("rn", std::bind(&CPLegacy::getIrNameCommHandler, this, _1, _2));
    m_parser.addCommandHandler("rns", std::bind(&CPLegacy::getIrListCommHandler, this, _1, _2));

    m_parser.addCommandHandler("gb", std::bind(&CPLegacy::getBankPresetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gm", std::bind(&CPLegacy::getOutputModeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gs", std::bind(&CPLegacy::getStateCommHandler, this, _1, _2));

    m_parser.addCommandHandler("sp", std::bind(&CPLegacy::ackSaveChanges, this, _1, _2));
    m_parser.addCommandHandler("pc", std::bind(&CPLegacy::ackPresetChangeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("esc", std::bind(&CPLegacy::ackEscCommHandler, this, _1, _2));

    m_parser.addCommandHandler("SYNC ERROR", std::bind(&CPLegacy::errorCCCommHandler, this, _1, _2));

    m_parser.addCommandHandler("REQUEST_CHUNK_SIZE", std::bind(&CPLegacy::requestNextChunkCommHandler, this, _1, _2));
    m_parser.addCommandHandler("FR_OK", std::bind(&CPLegacy::fwuFinishedCommHandler, this, _1, _2));
    m_parser.addCommandHandler("fsf", std::bind(&CPLegacy::formatFinishedCommHandler, this, _1, _2));

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif
}

void CPLegacy::initDevice(DeviceType deviceType)
{
    m_deviceType = deviceType;

    setDeviceType(m_deviceType);

    MV = new PresetVolume(this);

    NG = new NoiseGate(this);
    CM = new Compressor(this);
    PR = new Preamp(this);
    PA = new PowerAmp(this);
    IR = new CabSim(this);
    HPF = new HiPassFilter(this);
    EQ = new EqParametric(this);
    LPF = new LowPassFilter(this);
    ER = new EarlyReflections(this);

    m_modulesListModel.insertModule(NG, 0);
    m_modulesListModel.insertModule(CM, 1);
    m_modulesListModel.insertModule(PA, 2);
    m_modulesListModel.insertModule(PR, 3);
    m_modulesListModel.insertModule(IR, 4);
    m_modulesListModel.insertModule(HPF, 5);
    m_modulesListModel.insertModule(EQ, 6);
    m_modulesListModel.insertModule(LPF, 7);
    m_modulesListModel.insertModule(ER, 8);

    emit modulesListModelChanged();
    emit presetListModelChanged();

    emit sgDeviceInstanciated();
}

void CPLegacy::setDeviceType(DeviceType newDeviceType)
{
    m_deviceType = newDeviceType;

    if(m_minimalFirmware != nullptr)
    {
        delete(m_minimalFirmware);
        m_minimalFirmware = nullptr;
    }

    switch(m_deviceType)
    {
    case DeviceType::legacyCP16:
        m_minimalFirmware = new Firmware("1.04.03", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16.ble");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "CP-16M Blue";
        break;
    case DeviceType::legacyCP16PA:
        m_minimalFirmware = new Firmware("1.04.03", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16PA.ble");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "CP-16M-PA Green";
        break;
    case DeviceType::legacyCP100:
        m_minimalFirmware = new Firmware("2.08.02", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100.ble");
        m_maxBankCount = 10;
        m_maxPresetCount = 10;
        m_firmwareName = "CP-100";
        break;
    case DeviceType::legacyCP100PA:
        m_minimalFirmware = new Firmware("6.08.04", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100PA.ble");
        m_maxBankCount = 10;
        m_maxPresetCount = 10;
        m_firmwareName = "CP-100PA";
        break;

    // case DeviceType::LA3:
    //     m_minimalFirmware = new Firmware("1.05.03", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3RV.ble");
    //     m_maxBankCount = 0;
    //     m_maxPresetCount = 16;
    //     m_firmwareName = "LA3";
    //     break;
    default:
        qDebug() << __FUNCTION__ << "Unknown device type";
        break;
    }
}

void CPLegacy::readFullState()
{
    m_parser.enableFullEndMode();
    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("rns");

    emit sgPushCommandToQueue("gm");

    pushReadPresetCommands();

    emit sgProcessCommands();
}

QList<QByteArray> CPLegacy::parseAnswers(QByteArray &baAnswer)
{
    QList<QByteArray> parseResults, recievedCommAnswers;

    recievedCommAnswers += m_parser.parseNewData(baAnswer);

    if(getIrNameSize.getParse(baAnswer, &parseResults))
    {
        getIrNameSizeCommHandler(parseResults); // это лишь начало команды. Не считаем что она выполнена
        recievedCommAnswers.removeAll(QString("cc").toUtf8());

    }
    if(getIr.getParse(baAnswer, &parseResults))
    {
        getIrCommHandler(parseResults);
        recievedCommAnswers.append("cc");
    }
    if(ackCC.getParse(baAnswer, &parseResults))
    {
        ackCCCommHandler(parseResults);
        recievedCommAnswers.append("cc");
    }
    if(endCC.getParse(baAnswer, &parseResults))
    {
        endCCCommHandler(parseResults);
        recievedCommAnswers.append("cc");
    }

    return recievedCommAnswers;
}

void CPLegacy::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("gb");
    emit sgPushCommandToQueue("rn");
    emit sgPushCommandToQueue("gs");
}

void CPLegacy::setPresetData(const Preset &preset)
{
    QByteArray ba;

    ba.append("gs 1\r");
    ba.append(preset.rawData());

    emit sgPushCommandToQueue(ba);
    emit sgPushCommandToQueue("gs"); // read settled state
    emit sgProcessCommands();
}

void CPLegacy::saveChanges()
{
    qInfo()<<__FUNCTION__;

    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    emit sgPushCommandToQueue("sp");
    emit sgProcessCommands();

    // impulse data uploading after answer
    if(currentPreset.waveData() == IRWorker::flatIr())
    {
        if(m_deviceType==DeviceType::legacyCP16 || m_deviceType==DeviceType::legacyCP16PA)
            emit sgPushCommandToQueue("pwsd");
        else
            emit sgPushCommandToQueue("dcc");

        currentPreset.clearWavData();
    }
    else
    {
        uploadImpulseData(currentPreset.waveData(), false, currentPreset.impulseName());
    }

    currentSavedPreset = currentPreset;
    m_presetListModel.updatePreset(currentSavedPreset);

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
    emit deviceUpdatingValues(); // clear modified marks on control values
}

void CPLegacy::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if((newPreset == m_preset) && (newBank == m_bank)) return;

    quint8 val = Preset::calcPresetFlatIndex(m_deviceType, newBank, newPreset);


    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(currentSavedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pc %2").arg(val, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void CPLegacy::comparePreset()
{
    if(m_presetManager.currentState() != PresetState::Compare)
    {
        emit sgPushCommandToQueue("gs");
        emit sgPushCommandToQueue("esc");
    }
    else
    {
        m_presetManager.returnToPreviousState();
        setPresetData(currentPreset);
        uploadImpulseData(currentPreset.waveData(), true, currentPreset.impulseName());
    }
    emit sgProcessCommands();
}

void CPLegacy::copyPreset()
{
    m_presetManager.setCurrentState(PresetState::Copying);

    if(currentPreset.wavSize() == 0)
        emit sgPushCommandToQueue("cc");

    emit sgPushCommandToQueue("gs");
    emit sgProcessCommands();
}

void CPLegacy::pastePreset()
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

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPLegacy::importPreset(QString filePath, QString fileName)
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
        if(currentPreset.impulseName() != "")
        {
            importedPreset.setWaveData(IRWorker::flatIr());
            importedPreset.setImpulseName("");
        }
    }
    uploadImpulseData(importedPreset.waveData(), true, importedPreset.impulseName());
    currentPreset = importedPreset;

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPLegacy::exportPreset(QString filePath, QString fileName)
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

        emit sgPushCommandToQueue("gs");
        currentPreset.setPathToExport(filePath);
        emit sgPushCommandToQueue("cc");
        emit sgProcessCommands();
    }
}

void CPLegacy::restoreValue(QString name)
{
    //  TODO пока костыль для отмены сохранения пресета!!!!!
    if(name == "bank-preset")
    {
        m_bank = currentPreset.bankNumber();
        m_preset = currentPreset.presetNumber();
        emit bankPresetChanged();
    }
}

void CPLegacy::setImpulse(QString filePath)
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
        currentPreset.setWaveData(fileData);
        uploadImpulseData(fileData, true, fileName);
    }
}

void CPLegacy::uploadImpulseData(const QByteArray &impulseData, bool isPreview, QString impulseName)
{
    if(impulseData.isEmpty())
    {
        qInfo() << __FUNCTION__ << "no wave data";
        return;
    }

    m_presetManager.setCurrentState(PresetState::UploadingIr);

    impulseName.replace(QString(" "), QString("_"), Qt::CaseInsensitive);

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
    m_presetListModel.updatePreset(currentPreset);

    for(quint16 i=0; i<bytesToUpload; i++)
    {
        QString sTmp;
        quint8  chr;
        if(i>=irData.length())
            sTmp = QString("00");
        else
        {
            chr = irData.at(i);
            if(chr <= 0xF) //15
                sTmp = QString("0%1").arg(chr, 1, 16);
            else
                sTmp = QString("%1").arg (chr, 2, 16);
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

void CPLegacy::escImpulse()
{
    emit sgPushCommandToQueue("lcc");
    emit sgPushCommandToQueue("rn");
    emit sgProcessCommands();
}

void CPLegacy::setFirmware(QString fullFilePath)
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

void CPLegacy::uploadFirmware(const QByteArray& firmware)
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

void CPLegacy::formatMemory()
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
void CPLegacy::amtVerCommHandler(const QString &command, const QByteArray &arguments)
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

void CPLegacy::getBankPresetCommHandler(const QString &command, const QByteArray &arguments)
{
    m_bank = arguments.left(2).toUInt();
    m_preset  = arguments.right(2).toUInt();

    currentPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
    emit bankPresetChanged();
}

void CPLegacy::getOutputModeCommHandler(const QString &command, const QByteArray &arguments)
{
    quint8 mode = arguments.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

void CPLegacy::getStateCommHandler(const QString &command, const QByteArray &arguments)
{
    if(arguments.size() < sizeof(preset_data_legacy_t)*2) return;

    QByteArray baPresetData = arguments;

    quint8 count=0;
    quint8 nomByte=0;
    QString curByte;

    quint8 dataBuffer[256];

    preset_data_legacy_t str;
    memcpy(&str, baPresetData.data(), sizeof(preset_data_legacy_t));


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

    preset_data_legacy_t legacyData;
    memcpy(&legacyData, dataBuffer, sizeof(preset_data_legacy_t));

    MV->setValue(legacyData.preset_volume);

    NG->setValues(legacyData.gate_on, legacyData.gate_threshold, legacyData.gate_decay);
    CM->setValues(legacyData.compressor_on, legacyData.compressor_sustain, legacyData.compressor_volume);

    eq_t eqData;
    for(int i=0; i<5; i++)
    {
        eqData.band_type[i] = static_cast<quint8>(EqBand::PEAKING);
        eqData.band_vol[i] = legacyData.eq_band_vol[i];
        eqData.freq[i] = legacyData.eq_freq[i];
        eqData.Q[i] = legacyData.eq_Q[i];
    }
    EQ->setBandsData(eqData);
    HPF->setValues(legacyData.hp_on, legacyData.hp_freq);
    LPF->setValues(legacyData.lp_on, legacyData.lp_freq);
    IR->setEnabled(legacyData.cab_on);
    PA->setValues(legacyData.amp_on, legacyData.amp_volume, legacyData.presence_vol, legacyData.amp_slave, legacyData.amp_type);
    ER->setValues(legacyData.early_on, legacyData.early_volume, legacyData.early_type);


    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        // copy preview wave data
        if(currentPreset.wavSize() != 0)
            copiedPreset = currentPreset;

        copiedPreset.setRawData(baPresetData);
        m_presetManager.returnToPreviousState();
        break;
    }

    case PresetState::Changing:
    {
        m_deviceParamsModified = false;
        emit deviceParamsModifiedChanged();
        emit deviceUpdatingValues();

        currentPreset.setRawData(baPresetData);
        currentSavedPreset = currentPreset;
        m_presetListModel.updatePreset(currentSavedPreset);
        m_presetManager.returnToPreviousState();
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
}

void CPLegacy::getIrListCommHandler(const QString &command, const QByteArray &arguments)
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

void CPLegacy::getIrNameCommHandler(const QString &command, const QByteArray &arguments)
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
        // Необходимая заглушка. Не удалять!
        break;
    }

    default:
    {
        currentPreset.setImpulseName(impulseName);
    }
    }

    IR->setImpulseName(impulseName);

    m_presetListModel.updatePreset(currentPreset);
}

void CPLegacy::getIrNameSizeCommHandler(const QList<QByteArray> &arguments)
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
            currentPreset.setImpulseName(wavName);
            break;
        }

        default:
        {
            currentPreset.setImpulseName(wavName);
        }
        }

        emit m_presetManager.currentStateChanged(); // Для того чтобы экран BUSY правильно отобразил стадию этапа
        emit sgDisableTimeoutTimer();
    }
}

void CPLegacy::getIrCommHandler(const QList<QByteArray> &arguments)
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
        currentPreset.setWaveData(impulseData);
        currentPreset.exportData();

        emit sgDeviceMessage(DeviceMessageType::PresetExportFinished, currentPreset.pathToExport());

        m_presetManager.returnToPreviousState();
        break;
    }

    default:{}
    }

    m_bytesToRecieve=0;

    emit sgEnableTimeoutTimer();
}

void CPLegacy::requestNextChunkCommHandler(const QString &command, const QByteArray &arguments)
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

void CPLegacy::fwuFinishedCommHandler(const QString &command, const QByteArray &arguments)
{
    if(fwUpdate)
    {
        emit sgDeviceMessage(DeviceMessageType::FirmwareUpdateFinished);
        fwUpdate = false;
    }
}

void CPLegacy::formatFinishedCommHandler(const QString &command, const QByteArray &arguments)
{
    qDebug() << __FUNCTION__;
    emit sgDeviceMessage(DeviceMessageType::FormatMemoryFinished);
    isFormatting = false;
    // emit sgDisconnect();
}
//-------------------------------------acknowledegs------------------------------------------
void CPLegacy::ackEscCommHandler(const QString &command, const QByteArray &arguments)
{
    m_presetManager.setCurrentState(PresetState::Compare);
    emit sgPushCommandToQueue("gs");
    emit sgPushCommandToQueue("rn");
}

void CPLegacy::ackSaveChanges(const QString &command, const QByteArray &arguments)
{
    qInfo() << "Preset data saved to device";
    currentPreset.clearWavData();
}

void CPLegacy::ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments)
{
    qInfo() << __FUNCTION__ << "Preset change, updating state";
    currentPreset.clearWavData();
    m_presetManager.setCurrentState(PresetState::Changing);
    pushReadPresetCommands();
    emit sgProcessCommands();
}


void CPLegacy::ackCCCommHandler(const QList<QByteArray> &arguments)
{
    m_presetManager.returnToPreviousState();
    m_presetManager.setCurrentState(PresetState::SavingIr);
    emit sgDisableTimeoutTimer(); // wait for impulse saving (TODO возможно по размеру импульса посчитать время сохранения в устройстве)
}

void CPLegacy::endCCCommHandler(const QList<QByteArray> &arguments)
{
    emit sgEnableTimeoutTimer();
    m_presetManager.returnToPreviousState();
}

void CPLegacy::errorCCCommHandler(const QString &command, const QByteArray &arguments)
{
    emit sgEnableTimeoutTimer();
    m_presetManager.returnToPreviousState();
    emit sgDeviceError(DeviceErrorType::IrSaveError, currentPreset.impulseName());
}
