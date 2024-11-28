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
    m_parser.addCommandHandler("amtver", std::bind(&CPModern::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("plist", std::bind(&CPModern::getPresetListCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("state", std::bind(&CPModern::stateCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("ir", std::bind(&CPModern::irCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pname", std::bind(&CPModern::pnameCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("gb", std::bind(&CPModern::getBankPresetCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("gm", std::bind(&CPModern::getOutputModeCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sp", std::bind(&CPModern::ackSaveChanges, this, _1, _2, _3));
    m_parser.addCommandHandler("pc", std::bind(&CPModern::ackPresetChangeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("esc", std::bind(&CPModern::ackEscCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("SYNC ERROR", std::bind(&CPModern::errorCCCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("REQUEST_CHUNK_SIZE", std::bind(&CPModern::requestNextChunkCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("FR_OK", std::bind(&CPModern::fwuFinishedCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("fsf", std::bind(&CPModern::formatFinishedCommHandler, this, _1, _2, _3));

#ifdef Q_OS_ANDROID
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#else
    appSettings = new QSettings();
#endif
}

CPModern::~CPModern()
{
    qDeleteAll(m_presetsList);
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

    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("plist");
    emit sgPushCommandToQueue("gm");
    pushReadPresetCommands();

    emit sgProcessCommands();
}

void CPModern::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("gb");
    emit sgPushCommandToQueue("ir info");
    emit sgPushCommandToQueue("pname");
    emit sgPushCommandToQueue("state");
}

QList<QByteArray> CPModern::parseAnswers(QByteArray &baAnswer)
{
    QList<QByteArray> recievedCommAnswers;

    recievedCommAnswers += m_parser.parseNewData(baAnswer);

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

 /*   // impulse data uploading after answer
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
        uploadImpulseData(actualPreset.waveData(), false, actualPreset.irName());
    }*/

    savedPreset = actualPreset;
    m_presetListModel.updatePreset(&savedPreset);

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
    emit deviceUpdatingValues(); // clear modified marks on control values
}

void CPModern::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
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

void CPModern::comparePreset()
{
    if(m_presetManager.currentState() != PresetState::Compare)
    {
        emit sgPushCommandToQueue("pname");
        emit sgPushCommandToQueue("state");
        emit sgPushCommandToQueue("esc");
    }
    else
    {
        m_presetManager.returnToPreviousState();
        setPresetData(actualPreset);
        // uploadImpulseData(actualPreset.waveData(), true, actualPreset.irName());
    }
    emit sgProcessCommands();
}

void CPModern::copyPreset()
{
    m_presetManager.setCurrentState(PresetState::Copying);

 /*   if(actualPreset.wavSize() == 0)
    {
        //TODO copy to lib!
        emit sgPushCommandToQueue("ir info");
        QString pathToIr = "/bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset) + "/" + actualPreset.irName();
        emit sgPushCommandToQueue("ir download\r" + pathToIr.toUtf8() + "\n", false);
    }*/

    emit sgPushCommandToQueue("pname");
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
/*    if(copiedPreset.waveData().isEmpty())
    {
        if(actualPreset.irName() != "")
        {
            copiedPreset.setWaveData(IRWorker::flatIr());
            copiedPreset.setIrName("");
        }
    }
    uploadImpulseData(copiedPreset.waveData(), true, copiedPreset.irName());*/
    actualPreset = copiedPreset;
    actualPreset.setBankPreset(currentBankNumber, currentPresetNumber);

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
}

void CPModern::importPreset(QString filePath, QString fileName)
{
    // Q_UNUSED(fileName)

    // PresetModern importedPreset{this};

    // if(!importedPreset.importData(filePath))
    // {
    //     emit sgDeviceError(DeviceErrorType::PresetImportUnsuccesfull);
    //     return;
    // }

    // setPresetData(importedPreset);

    // if(importedPreset.waveData().isEmpty())
    // {
    //     if(actualPreset.irName() != "")
    //     {
    //         importedPreset.setWaveData(IRWorker::flatIr());
    //         importedPreset.setIrName("");
    //     }
    // }
    // uploadImpulseData(importedPreset.waveData(), true, importedPreset.irName());
    // actualPreset = importedPreset;

    // m_deviceParamsModified = true;
    // emit deviceParamsModifiedChanged();
}

void CPModern::exportPreset(QString filePath, QString fileName)
{
    if(m_presetManager.currentState() != PresetState::Exporting)
    {
        // QString folderPath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/presets/";

        // qInfo() << __FUNCTION__ << "Preset name: " << fileName;

        // m_presetManager.setCurrentState(PresetState::Exporting);

        // if(!QDir(folderPath).exists())
        // {
        //     QDir().mkpath(folderPath);
        // }

        // emit sgPushCommandToQueue("pname");
        // emit sgPushCommandToQueue("state");
        // actualPreset.setPathToExport(filePath);

        // emit sgPushCommandToQueue("ir info");
        // QString pathToIr = "/bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset) + "/" + actualPreset.irName();
        // emit sgPushCommandToQueue("ir download\r" + pathToIr.toUtf8() + "\n", false);
        // emit sgProcessCommands();
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

void CPModern::setCurrentPresetName(const QString &newCurrentPresetName)
{
    if (actualPreset.presetName() == newCurrentPresetName)
        return;
    if(newCurrentPresetName.size()>64) actualPreset.setPresetName(newCurrentPresetName.left(64));
    else actualPreset.setPresetName(newCurrentPresetName);

    emit currentPresetNameChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgWriteToInterface("pname\r" + actualPreset.presetName().toUtf8() + "\n");
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
        // actualPreset.setWaveData(fileData);
        /*if(!preview)*/ uploadImpulseData(fileName, fileData);
        // else previewIr(fileData);
    }
}

void CPModern::uploadImpulseData(QString& irName, const QByteArray& irData)
{
    // if(impulseData.isEmpty())
    // {
    //     qInfo() << __FUNCTION__ << "no wave data";
    //     return;
    // }

    m_presetManager.setCurrentState(PresetState::UploadingIr);

    irName.replace(QString(" "), QString("_"), Qt::CaseInsensitive);

    actualPreset.setIrName(irName);

    QByteArray baSend;//, irData;
    quint16 bytesToUpload;

    // if(isPreview)
    // {
    //     baSend.append(QString("ir preview\r").toUtf8());
    //     bytesToUpload = 984*3;
    //     irData = impulseData.mid(44); //cut wav header
    // }
    // else
    // {
        QByteArray destPath = QString("/bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset) + "/").toUtf8();
        baSend.append(QString("ir upload\r").toUtf8() + irName.toUtf8() + "\r" + destPath + "\r");
        // irData = irData;
        bytesToUpload = irData.size();
        actualPreset.setIrName(irName);
    // }
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
    emit sgPushCommandToQueue(baSend + "\n", false);
    emit sgProcessCommands();

    // m_deviceParamsModified = true;
    // emit deviceParamsModifiedChanged();

    // IR->setImpulseName(impulseName);
    // IR->setEnabled(true); // загрузка импульса автоматом включает модуль в устройстве, отобразить это визуально
}

// Для десктоп версии и просмотра из диалога выбора
void CPModern::previewIr(const QByteArray &irData)
{
    m_presetManager.setCurrentState(PresetState::UploadingIr);

    QByteArray baSend, trimIrData;
    quint16 bytesToUpload;

    baSend.append(QString("ir preview\r").toUtf8());
    bytesToUpload = 984*3;
    trimIrData = irData.mid(44); //cut wav header

    for(quint16 i=0; i<bytesToUpload; i++)
    {
        QString sTmp;
        quint8  chr;
        if(i>=trimIrData.length())
            sTmp = QString("00");
        else
        {
            chr = trimIrData.at(i);
            sTmp = QString("%1").arg (chr, 2, 16, QChar('0'));
        }
        baSend.append(sTmp.toUtf8());
    }
    emit sgPushCommandToQueue(baSend + "\n", false);
    emit sgProcessCommands();
}

void CPModern::escImpulse()
{
    emit sgPushCommandToQueue("lcc");
    emit sgPushCommandToQueue("ir info");
    emit sgProcessCommands();
}

void CPModern::setPresetData(const PresetModern &preset)
{
    QByteArray ba;

    ba.append("state\r");
    ba.append(PresetModern::presetDatatoChars(preset.presetData));

    emit sgPushCommandToQueue(ba);
    emit sgPushCommandToQueue("pname");
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

void CPModern::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
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

void CPModern::getBankPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_bank = data.left(2).toUInt();
    m_preset  = data.right(2).toUInt();

    actualPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
    emit bankPresetChanged();
}

void CPModern::getOutputModeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 mode = arguments.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

void CPModern::pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    actualPreset.setPresetName(data);
    emit currentPresetNameChanged();
}


void CPModern::stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(data.size() < sizeof(preset_data_t)*2) return;

    QByteArray baPresetData = data;

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
        // if(actualPreset.wavSize() != 0)
            copiedPreset = actualPreset;

        // copiedPreset.setRawData(baPresetData);
        copiedPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        m_presetManager.returnToPreviousState();
        emit presetCopied();
        break;
    }

    case PresetState::Changing:
    {
        m_deviceParamsModified = false;
        emit deviceParamsModifiedChanged();
        emit deviceUpdatingValues();

        // actualPreset.setRawData(baPresetData);
        actualPreset.presetData = PresetModern::charsToPresetData(baPresetData);
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
        actualPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        m_presetListModel.updatePreset(&actualPreset);
        break;
    }
    }
}


void CPModern::irCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "info")
    {
        recieveIrInfo(data);
    }

    if(arguments == "download")
    {
        if(data.size()>0)
        {
            QList<QByteArray> dataList = data.split('\r');
            if(dataList.length()==2)
            {
                irDownloaded(dataList.at(0), dataList.at(1));
            }
        }
        m_bytesToRecieve=0;
        emit sgEnableTimeoutTimer();
    }

    if(arguments == "request_chunk")
    {

    }
}

void CPModern::recieveIrInfo(const QByteArray &data)
{
    QList<QByteArray> dataList = data.split('\r');

    QString wavName = dataList.at(0);
    qint64 wavSize = m_bytesToRecieve = QString(dataList.at(1)).toInt();
    qDebug() << "IR name:" << wavName << ", size:" << wavSize;
    switch(m_presetManager.currentState())
    {
        case PresetState::Compare:
        {
            IR->setImpulseName(wavName);
            break;
        }

        case PresetState::Changing:
        {
            actualPreset.setIrName(wavName);
            savedPreset.setIrName(wavName);
            IR->setImpulseName(wavName);
        }

        case PresetState::Copying:
        {
            emit m_presetManager.currentStateChanged(); // Для того чтобы экран BUSY правильно отобразил стадию этапа
            emit sgDisableTimeoutTimer();

            copiedPreset.setIrName(wavName);
            m_bytesToRecieve = wavSize;
            break;
        }

        case PresetState::Exporting:
        {
            emit m_presetManager.currentStateChanged(); // Для того чтобы экран BUSY правильно отобразил стадию этапа
            emit sgDisableTimeoutTimer();

            m_bytesToRecieve = wavSize;
            actualPreset.setIrName(wavName);
            break;
        }

        default:
        {
            actualPreset.setIrName(wavName);
            IR->setImpulseName(wavName);
        }
    }
    m_presetListModel.updatePreset(&actualPreset);
}

void CPModern::irDownloaded(const QString &irPath, const QByteArray &data)
{
    QByteArray impulseData = QByteArray::fromHex(data);

    switch(m_presetManager.currentState())
    {
        case PresetState::Exporting:
        {
            // actualPreset.setWaveData(impulseData);
            // actualPreset.exportData();

            // emit sgDeviceMessage(DeviceMessageType::PresetExportFinished, actualPreset.pathToExport());

            // m_presetManager.returnToPreviousState();
            break;
        }

        default:{}
    }
}

void CPModern::getPresetListCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString fullList = data;

    QStringList separatedList = fullList.split("\r");

    m_presetsList.clear();

    quint8 bankNumber=0;
    quint8 presetNumber=0;

    QStringList::const_iterator it = separatedList.constBegin();
    while (it != separatedList.constEnd())
    {
        PresetModern* currentListPreset = new PresetModern(this);

        currentListPreset->setBankPreset(bankNumber, presetNumber);

        QStringList separatedArgs = (*it).split("|");

        QString irName;

        if(separatedArgs.size()<3)
        {
            qWarning() << "incorrect list record format: " << *it;
            goto ending;
        }

        irName = separatedArgs.at(0);
        currentListPreset->setIrName(irName.left(irName.lastIndexOf(".wav ") + 4));
        currentListPreset->setIsIrEnabled(separatedArgs.at(1).toInt());
        currentListPreset->setPresetName(separatedArgs.at(2));

    ending:
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

        it++;
    }

    m_presetListModel.refreshModel(&m_presetsList);
}

void CPModern::requestNextChunkCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
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
//-------------------------------------acknowledegs------------------------------------------
void CPModern::ackEscCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_presetManager.setCurrentState(PresetState::Compare);
    setPresetData(savedPreset);
    emit sgPushCommandToQueue("rn");
    emit sgProcessCommands();
}

void CPModern::ackSaveChanges(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    // actualPreset.clearWavData();
}

void CPModern::ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    // actualPreset.clearWavData();
    m_presetManager.setCurrentState(PresetState::Changing);
    pushReadPresetCommands();
    emit sgProcessCommands();
}

void CPModern::fwuFinishedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(fwUpdate)
    {
        emit sgDeviceMessage(DeviceMessageType::FirmwareUpdateFinished);
        fwUpdate = false;
    }
}

void CPModern::formatFinishedCommHandler(const QString &command, const QByteArray &argument, const QByteArray &data)
{
    qDebug() << __FUNCTION__;
    emit sgDeviceMessage(DeviceMessageType::FormatMemoryFinished);
    isFormatting = false;
}


//===========================================================
// Legacy comm handlers
//========================================================

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

void CPModern::errorCCCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit sgEnableTimeoutTimer();
    m_presetManager.returnToPreviousState();
    emit sgDeviceError(DeviceErrorType::IrSaveError, actualPreset.irName());
}
