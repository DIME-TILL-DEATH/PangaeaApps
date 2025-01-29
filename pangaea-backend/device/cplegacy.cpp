#include "cplegacy.h"

#include <QDir>
#include <QUrl>
#include <QStandardPaths>

#include "core.h"

#include "eqband.h"

CPLegacy::CPLegacy(Core *parent)
    : AbstractDevice{parent}
{
    CPLegacy::updateOutputModeNames();

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtver", std::bind(&CPLegacy::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("rn", std::bind(&CPLegacy::getIrNameCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("rns", std::bind(&CPLegacy::getIrListCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("gb", std::bind(&CPLegacy::getBankPresetCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("gm", std::bind(&CPLegacy::getOutputModeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("gs", std::bind(&CPLegacy::getStateCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sp", std::bind(&CPLegacy::ackSaveChanges, this, _1, _2, _3));
    m_parser.addCommandHandler("pc", std::bind(&CPLegacy::ackPresetChangeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("esc", std::bind(&CPLegacy::ackEscCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("SYNC ERROR", std::bind(&CPLegacy::errorCCCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("REQUEST_CHUNK_SIZE", std::bind(&CPLegacy::requestNextChunkCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("FR_OK", std::bind(&CPLegacy::fwuFinishedCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("fsf", std::bind(&CPLegacy::formatFinishedCommHandler, this, _1, _2, _3));

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif
    // cure forn non-\n commands
    m_parser.addCureParser("gm", new MaskedParser("gm x\r", "111X1"));

    m_parser.addCureParser("mv", new MaskedParser("mv x\r", "111X1"));
    m_parser.addCureParser("ce", new MaskedParser("ce x\r", "111X1"));
    m_parser.addCureParser("eqo", new MaskedParser("eqo x\r", "111X1"));
    m_parser.addCureParser("eqv", new MaskedParser("eqv x x\r", "1111X1X1"));
    m_parser.addCureParser("eqf", new MaskedParser("eqf x x\r", "1111X1X1"));
    m_parser.addCureParser("eqq", new MaskedParser("eqq x x\r", "1111X1X1"));

    m_parser.addCureParser("ho", new MaskedParser("ho x\r", "111X1"));
    m_parser.addCureParser("lo", new MaskedParser("lo x\r", "111X1"));
    m_parser.addCureParser("hv", new MaskedParser("hv x\r", "111X1"));
    m_parser.addCureParser("lv", new MaskedParser("lv x\r", "111X1"));

    m_parser.addCureParser("eo", new MaskedParser("eo x\r", "111X1"));
    m_parser.addCureParser("ev", new MaskedParser("ev x\r", "111X1"));
    m_parser.addCureParser("et", new MaskedParser("et x\r", "111X1"));
}

CPLegacy::~CPLegacy()
{
    qDeleteAll(m_presetsList);
}

void CPLegacy::updateOutputModeNames()
{
    m_avaliableOutputModes.clear();
    m_avaliableOutputModes.append(AbstractDevice::tr("Phones"));
    m_avaliableOutputModes.append(AbstractDevice::tr("Line"));
    m_avaliableOutputModes.append(AbstractDevice::tr("Balanced"));

    emit avaliableOutputModesChanged();
    emit outputModeChanged();
}

void CPLegacy::initDevice(DeviceType deviceType)
{
    m_deviceType = deviceType;
    setDeviceType(m_deviceType);

    // MV = new PresetVolume(this);

    NG = new NoiseGate(this);
    CM = new Compressor(this);
    PR = new Preamp(this);
    PA = new PowerAmp(this);
    PS = new Presence(this);
    IR = new CabSim(this);
    connect(IR, &CabSim::dataChanged, this, &CPLegacy::slIrEnabledChanged);
    HPF = new HiPassFilter(this);
    EQ = new EqParametric(this);
    LPF = new LowPassFilter(this);
    ER = new EarlyReflections(this);

    m_moduleList.append(NG);
    m_moduleList.append(CM);
    m_moduleList.append(PR);
    if(m_deviceType == DeviceType::LEGACY_CP16PA || m_deviceType == DeviceType::LEGACY_CP100PA)
    {
        m_moduleList.append(PA);
        connect(this, &CPLegacy::isPreEqChanged, this, &CPLegacy::arrangePrePost);
    }
    else
    {
        m_moduleList.append(PS);
    }
    m_moduleList.append(IR);
    m_moduleList.append(HPF);
    m_moduleList.append(EQ);
    m_moduleList.append(LPF);
    m_moduleList.append(ER);

    m_modulesListModel.refreshModel(&m_moduleList);

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
    case DeviceType::LEGACY_CP16:
        m_minimalFirmware = new Firmware("1.04.03", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16.ble");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "CP-16M Blue";
        m_isPaFw = false;
        break;
    case DeviceType::LEGACY_CP16PA:
        m_minimalFirmware = new Firmware("1.04.03", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP16PA.ble");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "CP-16M-PA Green";
        m_isPaFw = true;
        break;
    case DeviceType::LEGACY_CP100:
        m_minimalFirmware = new Firmware("2.08.02", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100.ble");
        m_maxBankCount = 10;
        m_maxPresetCount = 10;
        m_firmwareName = "CP-100";
        m_isPaFw = false;
        break;
    case DeviceType::LEGACY_CP100PA:
        m_minimalFirmware = new Firmware("6.08.04", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareCP100PA.ble");
        m_maxBankCount = 10;
        m_maxPresetCount = 10;
        m_firmwareName = "CP-100PA";
        m_isPaFw = true;
        break;

    // case DeviceType::LA3:
    //     m_minimalFirmware = new Firmware("1.05.03", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3RV.ble");
    //     m_maxBankCount = 0;
    //     m_maxPresetCount = 16;
    //     m_firmwareName = "LA3";
    //     break;
    default:
        qWarning() << __FUNCTION__ << "Unknown device type";

        m_minimalFirmware = new Firmware("1.04.03", newDeviceType, FirmwareType::DeviceInternal, "");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "Unknown CP device";
        break;
        break;
    }
}

void CPLegacy::readFullState()
{
    m_presetManager.setCurrentState(PresetState::Changing);
    m_parser.enableFullEndMode();
    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("rns");

    emit sgPushCommandToQueue("gm");

    pushReadPresetCommands();

    emit sgProcessCommands();
}

void CPLegacy::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("gb");
    emit sgPushCommandToQueue("rn");
    emit sgPushCommandToQueue("gs");
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

    QList<QByteArray> args;
    if(undefCommParser.getParse(baAnswer, &args))
    {
        QByteArray data;
        if(!args.isEmpty()) data = args.first();
        undefinedCommandCommHandler("", data);
    }

    return recievedCommAnswers;
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
    if(actualPreset.waveData() == IRWorker::flatIr())
    {
        if(m_deviceType==DeviceType::LEGACY_CP16 || m_deviceType==DeviceType::LEGACY_CP16PA)
            emit sgPushCommandToQueue("pwsd");
        else
            emit sgPushCommandToQueue("dcc");

        actualPreset.clearWavData();
    }
    else
    {
        uploadImpulseData(actualPreset.waveData(), false, actualPreset.irName());
    }

    savedPreset = actualPreset;
    m_presetListModel.updatePreset(&savedPreset);

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
    emit deviceUpdatingValues(); // clear modified marks on control values
}

void CPLegacy::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if((newPreset == m_preset) && (newBank == m_bank)) return;

    quint8 val = PresetLegacy::calcPresetFlatIndex(m_deviceType, newBank, newPreset);


    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(&savedPreset); // Обновить актуальный пресет перед переключением

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
        setPresetData(actualPreset);
        uploadImpulseData(actualPreset.waveData(), true, actualPreset.irName());
    }
    emit sgProcessCommands();
}

void CPLegacy::copyPreset()
{
    m_presetManager.setCurrentState(PresetState::Copying);

    if(actualPreset.wavSize() == 0)
        emit sgPushCommandToQueue("cc");

    emit sgPushCommandToQueue("gs");
    emit sgProcessCommands();
}

void CPLegacy::pastePreset()
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
        if(actualPreset.irName() != "")
        {
            copiedPreset.setWaveData(IRWorker::flatIr());
            copiedPreset.setIrName("");
        }
    }
    uploadImpulseData(copiedPreset.waveData(), true, copiedPreset.irName());
    actualPreset = copiedPreset;
    actualPreset.setBankPreset(currentBankNumber, currentPresetNumber);

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPLegacy::importPreset(QString filePath, QString fileName)
{
    Q_UNUSED(fileName)

    PresetLegacy importedPreset{this};

    if(!importedPreset.importData(filePath))
    {
        emit sgDeviceError(DeviceErrorType::PresetImportUnsuccesfull);
        return;
    }

    setPresetData(importedPreset);

    if(importedPreset.waveData().isEmpty())
    {
        if(actualPreset.irName() != "")
        {
            importedPreset.setWaveData(IRWorker::flatIr());
            importedPreset.setIrName("");
        }
    }
    uploadImpulseData(importedPreset.waveData(), true, importedPreset.irName());
    actualPreset = importedPreset;

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPLegacy::exportPreset(QString filePath, QString fileName)
{
    if(m_presetManager.currentState() != PresetState::Exporting)
    {
        // QString folderPath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/presets/";

        qInfo() << __FUNCTION__ << "Preset name: " << fileName;

        m_presetManager.setCurrentState(PresetState::Exporting);

        // if(!QDir(folderPath).exists())
        // {
        //     QDir().mkpath(folderPath);
        // }

        emit sgPushCommandToQueue("gs");
        actualPreset.setPathToExport(filePath);
        emit sgPushCommandToQueue("cc");
        emit sgProcessCommands();
    }
}

void CPLegacy::restoreValue(QString name)
{
    //  TODO пока костыль для отмены сохранения пресета!!!!!
    if(name == "bank-preset")
    {
        m_bank = actualPreset.bankNumber();
        m_preset = actualPreset.presetNumber();
        emit bankPresetChanged();
    }
}

void CPLegacy::startIrUpload(QString srcFilePath, QString dstFilePath, bool trimFile)
{
    QString fileName;
    QFileInfo fileInfo(srcFilePath);

    if(fileInfo.isFile())
    {
        fileInfo.absoluteDir();
        fileName = fileInfo.fileName();
    }
    else return;

    stWavHeader wavHead = IRWorker::getFormatWav(srcFilePath);

    if((wavHead.sampleRate != 48000) || (wavHead.bitsPerSample != 24) || (wavHead.numChannels != 1))
    {
        qWarning() << __FUNCTION__ << "Not supported wav format";
        emit sgDeviceError(DeviceErrorType::IrFormatNotSupported, QString().setNum(wavHead.sampleRate)+" Hz/"+
                                                            QString().setNum(wavHead.bitsPerSample)+" bits/"+
                                                            QString().setNum(wavHead.numChannels)+" channel");
    }
    else
    {
        irWorker.decodeWav(srcFilePath);

        QByteArray fileData = irWorker.formFileData();
        if(trimFile)
        {
            if(fileData.size() > maxIrSize()) fileData = fileData.left(maxIrSize());
        }
        actualPreset.setWaveData(fileData);
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

    actualPreset.setIrName(impulseName);

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
        actualPreset.setIrName(impulseName);
    }
    m_presetListModel.updatePreset(&actualPreset);

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

void CPLegacy::escImpulse()
{
    emit sgPushCommandToQueue("lcc");
    emit sgPushCommandToQueue("rn");
    emit sgProcessCommands();
}

void CPLegacy::setPresetData(const PresetLegacy &preset)
{
    QByteArray ba;

    ba.append("gs 1\r");
    ba.append(preset.rawData());

    emit sgPushCommandToQueue(ba);
    emit sgPushCommandToQueue("gs"); // read settled state
    emit sgProcessCommands();
}

void CPLegacy::setFirmware(QString fullFilePath)
{
    qDebug()<< __FUNCTION__ << "fullFilePath" << fullFilePath;
    QUrl url(fullFilePath);

    emit sgDeviceMessage(DeviceMessageType::FirmwareFilePath, url.path());

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

        qDebug() << Q_FUNC_INFO << symbolsToSend;

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

void CPLegacy::arrangePrePost()
{
    int prePosition = 2;
    int postPosition = 6;
    if(m_isPreEq)
    {
        if(m_moduleList.at(prePosition)->moduleType() != ModuleType::EQ1)
        {
            m_modulesListModel.moveModule(postPosition, prePosition);
        }
    }
    else
    {
        if(m_moduleList.at(postPosition)->moduleType() != ModuleType::EQ1)
        {
            m_modulesListModel.moveModule(prePosition, postPosition);
        }
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

void CPLegacy::slIrEnabledChanged()
{
    actualPreset.setIsIrEnabled(IR->moduleEnabled());
    m_presetListModel.updatePreset(&actualPreset);
}

void CPLegacy::setIsPreEq(bool newIsPreEq)
{
    if (m_isPreEq == newIsPreEq)
        return;
    m_isPreEq = newIsPreEq;
    emit isPreEqChanged();

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgWriteToInterface(QString("eqp %1\r\n").arg(m_isPreEq).toUtf8());
}

//=======================================================================================
//                  ***************Comm handlers************
//=======================================================================================
void CPLegacy::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString firmwareVersion = data;

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

void CPLegacy::getBankPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_bank = data.left(2).toUInt();
    m_preset  = data.right(2).toUInt();

    actualPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
    emit bankPresetChanged();
}

void CPLegacy::getOutputModeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 mode = data.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

void CPLegacy::getStateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(data.size() < sizeof(preset_data_legacy_t)*2) return;

    QByteArray baPresetData = data;

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
    PR->setValues(legacyData.preamp_on, legacyData.preamp_volume, legacyData.preamp_low, legacyData.preamp_mid, legacyData.preamp_high);
    PA->setValues(legacyData.amp_on, legacyData.amp_volume, legacyData.presence_vol, legacyData.amp_slave, legacyData.amp_type);
    PS->setValues(legacyData.presence_on, legacyData.presence_vol);

    eq_t eqData;
    for(int i=0; i<5; i++)
    {
        eqData.band_type[i] = static_cast<quint8>(FilterType::PEAKING);
        eqData.gain[i] = legacyData.eq_band_vol[i];
        eqData.freq[i] = legacyData.eq_freq[i];
        eqData.Q[i] = legacyData.eq_Q[i];
    }
    eqData.parametric_on = legacyData.eq_on;
    eqData.hp_freq = legacyData.hp_freq;
    eqData.hp_on = legacyData.hp_on;
    eqData.lp_freq = legacyData.lp_freq;
    eqData.lp_on = legacyData.lp_on;

    EQ->setEqData(eqData);
    HPF->setValues(legacyData.hp_on, legacyData.hp_freq);
    LPF->setValues(legacyData.lp_on, legacyData.lp_freq);
    IR->setEnabled(legacyData.cab_on);
    ER->setValues(legacyData.early_on, legacyData.early_volume, legacyData.early_type);

    m_isPreEq = legacyData.eq_pre;
    emit isPreEqChanged();


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
        m_presetListModel.updatePreset(&savedPreset);
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
        m_presetListModel.updatePreset(&actualPreset);
        break;
    }
    }
}

void CPLegacy::getIrListCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_parser.disableFullEndMode();

    QString fullList = data;

    QStringList separatedList = fullList.split("\n");

    m_presetsList.clear();

    quint8 bankNumber=0;
    quint8 presetNumber=0;

    QStringList::const_iterator it = separatedList.constBegin();
    while (it != separatedList.constEnd())
    {
        PresetLegacy* currentListPreset = new PresetLegacy(this);

        currentListPreset->setBankPreset(bankNumber, presetNumber);

        QString stringResult = *it;
        if(stringResult=="*")
            stringResult="";
        quint16 positionEndName = stringResult.lastIndexOf(".wav ");
        currentListPreset->setIrName(stringResult.left(positionEndName+4));
        it++;

        stringResult = *it;
        currentListPreset->setIsIrEnabled(stringResult.toInt());
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

void CPLegacy::getIrNameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString impulseName;

    if(data.size()>0)
        impulseName = data;
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
        actualPreset.setIrName(impulseName);
        savedPreset.setIrName(impulseName);
        IR->setImpulseName(impulseName);
    }

    default:
    {
        actualPreset.setIrName(impulseName);
        IR->setImpulseName(impulseName);
    }
    }

    m_presetListModel.updatePreset(&actualPreset);
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
            copiedPreset.setIrName(wavName);

            m_symbolsToRecieve = wavSize;
            qDebug() << "Recieve Ir. Name:" << wavName << ", size:" << wavSize;
            break;
        }

        case PresetState::Exporting:
        {
            m_symbolsToRecieve = wavSize;
            actualPreset.setIrName(wavName);
            break;
        }

        default:
        {
            actualPreset.setIrName(wavName);
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
        actualPreset.setWaveData(impulseData);
        actualPreset.exportData();

        QUrl url(actualPreset.pathToExport());

        emit sgDeviceMessage(DeviceMessageType::PresetExportFinished, url.path());

        m_presetManager.returnToPreviousState();
        break;
    }

    default:{}
    }

    m_symbolsToRecieve=0;

    emit sgEnableTimeoutTimer();
}

void CPLegacy::requestNextChunkCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
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
        emit sgSendWithoutConfirmation("0\n");
    }
    emit sgProcessCommands();
}

void CPLegacy::fwuFinishedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(fwUpdate)
    {
        emit sgDeviceMessage(DeviceMessageType::FirmwareUpdateFinished);
        fwUpdate = false;
    }
}

void CPLegacy::formatFinishedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    qDebug() << __FUNCTION__;
    emit sgDeviceMessage(DeviceMessageType::FormatMemoryFinished);
    isFormatting = false;
    // emit sgDisconnect();
}
//-------------------------------------acknowledegs------------------------------------------
void CPLegacy::ackEscCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_presetManager.setCurrentState(PresetState::Compare);
    setPresetData(savedPreset);
    emit sgPushCommandToQueue("rn");
    emit sgProcessCommands();
}

void CPLegacy::ackSaveChanges(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    actualPreset.clearWavData();
    emit presetSaved();
}

void CPLegacy::ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    actualPreset.clearWavData();
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

    emit impulseUploaded();
}

void CPLegacy::errorCCCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit sgEnableTimeoutTimer();
    m_presetManager.returnToPreviousState();
    emit sgDeviceError(DeviceErrorType::IrSaveError, actualPreset.irName());
}
