#include "cpmodern.h"

#include <QDir>
#include <QStandardPaths>

#include "core.h"

#include "eqband.h"

CPModern::CPModern(Core *parent)
    :AbstractDevice{parent}
{
    m_deviceClass = DeviceClass::CP_MODERN;

    CPModern::updateOutputModeNames();

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtver", std::bind(&CPModern::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("plist", std::bind(&CPModern::getPresetListCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("state", std::bind(&CPModern::stateCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("ir", std::bind(&CPModern::irCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pname", std::bind(&CPModern::pnameCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("ls", std::bind(&CPModern::listCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("list", std::bind(&CPModern::listCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("gb", std::bind(&CPModern::getBankPresetCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("gm", std::bind(&CPModern::getOutputModeCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("pc", std::bind(&CPModern::ackPresetChangeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("sp", std::bind(&CPModern::ackPresetSavedCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("REQUEST_CHUNK_SIZE", std::bind(&CPModern::requestNextChunkCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("FR_OK", std::bind(&CPModern::fwuFinishedCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("fsf", std::bind(&CPModern::formatFinishedCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("copy", std::bind(&CPModern::copyCommHandler, this, _1, _2, _3));

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

void CPModern::updateOutputModeNames()
{
    m_avaliableOutputModes.clear();
    m_avaliableOutputModes.append(AbstractDevice::tr("Phones"));
    m_avaliableOutputModes.append(AbstractDevice::tr("Line"));
    m_avaliableOutputModes.append(AbstractDevice::tr("Balanced"));
    m_avaliableOutputModes.append(AbstractDevice::tr("L: Processed/R: Monitor"));

    emit avaliableOutputModesChanged();
    emit outputModeChanged();
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
    connect(IR, &CabSim::dataChanged, this, &CPModern::slIrEnabledChanged);
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
    emit sgPushCommandToQueue("ls ir_library\r\n", false);
    emit sgPushCommandToQueue("gm");
    pushReadPresetCommands();

    emit sgProcessCommands();
}

void CPModern::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("gb");
    emit sgPushCommandToQueue("ir info");
    emit sgPushCommandToQueue("pname get");
    emit sgPushCommandToQueue("state get");

    m_symbolsToRecieve = 27 + 8 + sizeof(preset_data_t) * 2;
}

QList<QByteArray> CPModern::parseAnswers(QByteArray &baAnswer)
{
    QList<QByteArray> recievedCommAnswers;

    recievedCommAnswers += m_parser.parseNewData(baAnswer);

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
        m_presetManager.setCurrentState(PresetState::SetCompare);
        emit sgPushCommandToQueue("state get");
    }
    else
    {
        m_presetManager.returnToPreviousState();
        emit currentPresetNameChanged();
        setPresetData(actualPreset);
        emit sgPushCommandToQueue("state get\r\n");
    }
    emit sgProcessCommands();
}

void CPModern::copyPreset()
{
    m_presetManager.setCurrentState(PresetState::Copying);

    if(actualPreset.irFile.irLinkPath().indexOf("ir_library") == -1)
    {
        qDebug() << "IR file not linked to library";

        if(!isFileInLibrary(actualPreset.irFile.irName()))
        {
            QString pathToIr;
            if(actualPreset.irFile.irLinkPath().isEmpty())
                pathToIr = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset) + "/" + actualPreset.irFile.irName();
            else
                pathToIr = actualPreset.irFile.irLinkPath() + "/" + actualPreset.irFile.irName();

            QString destPath = "ir_library/" + actualPreset.irFile.irName();
            emit sgPushCommandToQueue("copy\r" + pathToIr.toUtf8() + "\r" + destPath.toUtf8() + "\n", false);
        }
    }
    emit sgPushCommandToQueue("state get");
    emit sgProcessCommands();
}

void CPModern::pastePreset()
{
    quint8 currentBankNumber = actualPreset.bankNumber();
    quint8 currentPresetNumber = actualPreset.presetNumber();

    actualPreset = copiedPreset;
    actualPreset.setBankPreset(currentBankNumber, currentPresetNumber);
    setPresetData(actualPreset);
    m_presetListModel.updatePreset(&actualPreset);

    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();
    emit currentIrFileChanged();
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

void CPModern::startIrUpload(QString srcFilePath, QString dstFilePath, bool trimFile)
{
    QString fileName;
    QFileInfo fileInfo(srcFilePath);

    if(fileInfo.isFile())
    {
        fileInfo.absoluteDir();
        fileName = fileInfo.fileName();
    }
    else
    {
        qWarning() << "Can't open file";
        emit sgDeviceError(DeviceErrorType::FileNotFound, "", {fileName});
        return;
    }

    if(dstFilePath.indexOf("ir_library") != -1)
    {
        if(isFileInLibrary(fileName))
        {
            emit sgDeviceError(DeviceErrorType::FileExists, "", {fileName});
            return;
        }
    }
    else
    {
        if(isFileInFolder(fileName))
        {
            emit sgDeviceError(DeviceErrorType::FileExists, "", {fileName});
            return;
        }
    }

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

        // fileName.replace(QString(" "), QString("_"), Qt::CaseInsensitive);

        QByteArray fileData = irWorker.formFileData();
        if(trimFile)
        {
            if(fileData.size() > maxIrSize()) fileData = fileData.left(maxIrSize());
        }
        uploadIrData(fileName, dstFilePath, fileData);
    }
}

void CPModern::uploadIrData(const QString& irName, const QString& dstPath, const QByteArray& irData)
{
    m_presetManager.setCurrentState(PresetState::UploadingIr);
    emit m_presetManager.currentStateChanged();

    m_rawIrData = irData;
    QByteArray command = QString("ir start_upload\r").toUtf8() + irName.toUtf8() + "\r" + dstPath.toUtf8() + "\n";

    qint64 symbolsToSend = command.size() + m_rawIrData.size() + 20 * (m_rawIrData.size() / uploadBlockSize + 1); // header: ir part_upload\r*\n = 16
    qDebug() << "Patrs: " << m_rawIrData.size() / uploadBlockSize + 1 << "Full size: " << symbolsToSend;
    qint64 symbolsToRecieve = 0;

    emit sgSendWithoutConfirmation(command, symbolsToSend, symbolsToRecieve);
    emit sgProcessCommands();
}

void CPModern::escImpulse()
{
    emit sgPushCommandToQueue("lcc");
    emit sgPushCommandToQueue("ir info");
    emit sgProcessCommands();
}

bool CPModern::isFileInLibrary(const QString &fileName)
{
    foreach (IrFile irLibFile, m_irsInLibrary)
    {
        if(irLibFile.irName() == fileName) return true;
    }
    return false;
}

bool CPModern::isFileInFolder(const QString &fileName)
{
    foreach (IrFile irFolderFile, m_irsInFolder)
    {
        if(irFolderFile.irName() == fileName) return true;
    }
    return false;
}

QString CPModern::currentPresetName() const
{
    if(m_presetManager.currentState() == PresetState::Compare) return savedPreset.presetName();
    else return actualPreset.presetName();
}

void CPModern::setPresetData(const PresetModern &preset)
{
    QByteArray ba;

    ba.append("state set\r");
    ba.append(PresetModern::presetDatatoChars(preset.presetData));

    emit sgPushCommandToQueue(ba + "\n", false);
    IR->setImpulseName(preset.irFile.irName());

    emit sgPushCommandToQueue("ir link\r" + preset.irFile.irName().toUtf8() + "\r" +
                                  preset.irFile.irLinkPath().toUtf8() + "\n", false);
    emit sgPushCommandToQueue("pname set\r" + preset.presetName().toUtf8() + "\n");
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

        qint64 symbolsToSend = m_rawFirmwareData.size() + 4 * m_rawFirmwareData.size() / uploadBlockSize + 2; // 128\n = 4 * (parts num and 0\n = 2
        qint64 symbolsToRecieve = 0; //QString("REQUEST_NEXT_CHUNK\n").size() * m_rawFirmwareData.size() / fwUploadBlockSize; // REQUEST_CHUNK_SIZE\n = 18

        qDebug() << Q_FUNC_INFO << symbolsToSend;

        emit sgDeviceMessage(DeviceMessageType::FirmwareUpdateStarted);
        emit sgDisableTimeoutTimer();

        fwUpdate = true;

        QByteArray baTmp, baSend;
        baSend.append("fwu\r");

        baTmp = m_rawFirmwareData.left(uploadBlockSize);
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

void CPModern::slIrEnabledChanged()
{
    actualPreset.setIsIrEnabled(IR->moduleEnabled());
    m_presetListModel.updatePreset(&actualPreset);
}

void CPModern::setCurrentIrFile(const IrFile &newCurrentIrFile)
{
    actualPreset.irFile = newCurrentIrFile;
    emit currentIrFileChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    IR->setImpulseName(actualPreset.irFile.irName());

    emit sgPushCommandToQueue("ir link\r" + actualPreset.irFile.irName().toUtf8() + "\r" +
                                            actualPreset.irFile.irLinkPath().toUtf8() + "\n", false);
    m_presetListModel.updatePreset(&actualPreset);
    emit sgProcessCommands();
}

void CPModern::setCurrentPresetName(const QString &newCurrentPresetName)
{
    if (actualPreset.presetName() == newCurrentPresetName)
        return;
    if(newCurrentPresetName.size()>64) actualPreset.setPresetName(newCurrentPresetName.left(64));
    else actualPreset.setPresetName(newCurrentPresetName);
    m_presetListModel.updatePreset(&actualPreset);

    emit currentPresetNameChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgPushCommandToQueue("pname set\r" + actualPreset.presetName().toUtf8() + "\n", false);
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

    // TODO when opens IR manager
    QString presetPath = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);
    emit sgPushCommandToQueue("ls " + presetPath.toUtf8() + "\r\n", false);
    emit sgProcessCommands();
}

void CPModern::getOutputModeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 mode = data.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

void CPModern::pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    switch(m_presetManager.currentState())
    {
    case PresetState::Compare:
    {
        break;
    }
    default:
    {
        actualPreset.setPresetName(data);
        emit currentPresetNameChanged();
    }
    }
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
    emit deviceUpdatingValues();

    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        copiedPreset = actualPreset;
        copiedPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        copiedPreset.irFile.setIrLinkPath("ir_library"); // Скопированные пресеты могут ссылаться только на библиотеку
        m_presetManager.returnToPreviousState();
        emit presetCopied();
        break;
    }

    case PresetState::Changing:
    {
        m_symbolsToRecieve = 0;
        m_deviceParamsModified = false;
        emit deviceParamsModifiedChanged();

        actualPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        savedPreset = actualPreset;
        m_presetListModel.updatePreset(&savedPreset);
        m_presetManager.returnToPreviousState();
        emit presetSwitched();
        break;
    }

    case PresetState::SetCompare:
    {
        actualPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        setPresetData(savedPreset);

        emit sgPushCommandToQueue("state get");
        m_presetManager.returnToPreviousState();
        m_presetManager.setCurrentState(PresetState::Compare);
        emit currentPresetNameChanged(); // Меняется только отображаемое имя. В устройство писать не обязательно
        emit sgProcessCommands();
        break;
    }

    case PresetState::Compare:
    {
        m_presetListModel.updatePreset(&savedPreset);
        // Необходимая заглушка. Не удалять
        break;
    }

    default:
    {
        actualPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        break;
    }
    }
    m_presetListModel.updatePreset(&actualPreset);
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
        m_symbolsToRecieve=0;
        emit sgEnableTimeoutTimer();
    }

    if(arguments == "link")
    {
        switch(m_presetManager.currentState())
        {
        case PresetState::Compare:
        {
            break;
        }
        default:
        {
            QList<QByteArray> dataList = data.split('\r');
            if(dataList.length()==2)
            {
                actualPreset.irFile.setIrName(dataList.at(0));
                actualPreset.irFile.setIrLinkPath(dataList.at(1));
            }
        }
        }
    }

    if(arguments == "request_part")
    {
        part++;
        qDebug() << "Send part: " << part;
        QByteArray answer;
        if(m_rawIrData.length() > 0)
        {
            QByteArray baTmp = m_rawIrData.left(uploadBlockSize);
            m_rawIrData.remove(0, baTmp.length());

            answer = "ir part_upload " + QString().setNum(baTmp.length()).toUtf8() + "\r";
            answer += baTmp;
            answer += "\n";
            emit sgSendWithoutConfirmation(answer);
        }
        else
        {
            part=0;
            m_presetManager.returnToPreviousState();
            emit impulseUploaded();
            emit sgPushCommandToQueue("ls ir_library\r\n", false);
            QString presetPath = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);
            emit sgPushCommandToQueue("ls " + presetPath.toUtf8() + "\r\n", false);
        }
        emit sgProcessCommands();
    }

    if(arguments == "error")
    {
        emit sgEnableTimeoutTimer();
        m_presetManager.returnToPreviousState();
    }

    if(arguments == "request_chunk")
    {

    }
}

void CPModern::recieveIrInfo(const QByteArray &data)
{
    QList<QByteArray> dataList = data.split('\r');

    if(dataList.size()<3)
    {
        qWarning() << "ir info comm answer has incorrect count of arguments: " << dataList.size();
        return;
    }

    QString irLinkPath = dataList.at(0);
    QString wavName = dataList.at(1);
    qint64 wavSize = m_symbolsToRecieve = QString(dataList.at(2)).toInt();
    qDebug() << "IR name:" << wavName << ", size:" << wavSize;
    switch(m_presetManager.currentState())
    {
        case PresetState::SetCompare:
        case PresetState::Compare:
        {
            IR->setImpulseName(wavName);
            break;
        }

        case PresetState::Changing:
        {
            actualPreset.irFile.setIrName(wavName);
            actualPreset.irFile.setIrLinkPath(irLinkPath);

            savedPreset.irFile = actualPreset.irFile;
            IR->setImpulseName(wavName);
        }

        case PresetState::Exporting:
        {
            emit m_presetManager.currentStateChanged(); // Для того чтобы экран BUSY правильно отобразил стадию этапа
            emit sgDisableTimeoutTimer();

            m_symbolsToRecieve = wavSize;
            actualPreset.setIrName(wavName);
            break;
        }

        default:
        {
            actualPreset.setIrName(wavName);
            IR->setImpulseName(wavName);
        }
    }
    emit currentIrFileChanged();
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
        currentListPreset->setIrName(irName);
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

void CPModern::listCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments.indexOf("INCORRECT_ARGUMENTS") != -1)
    {
        qWarning() << "Incorrect ls request";
        return;
    }

    if(arguments.indexOf("OPEN_DIR_FAILED") != -1)
    {
        qWarning() << "Device can't open requested directory";
        return;
    }

    QList<QByteArray> dataList = data.split('\r');
    if(arguments.indexOf("ir_library") != -1)
    {
        m_irsInLibrary.clear();
        foreach(auto ba, dataList)
        {
            m_irsInLibrary.append({ba, arguments});
        }
    }
    else
    {
        m_irsInFolder.clear();
        foreach(auto ba, dataList)
        {
            m_irsInFolder.append({ba, arguments});
        }
    }
    emit irsListChanged();
}

void CPModern::requestNextChunkCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    qDebug() << "remaining part " << m_rawFirmwareData.size();

    if(m_rawFirmwareData.length() > 0)
    {
        QByteArray baSend, baTmp;
        baTmp = m_rawFirmwareData.left(uploadBlockSize);
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
//-------------------------------------acknowledegs------------------------------------------

void CPModern::ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    // actualPreset.clearWavData();
    m_presetManager.returnToPreviousState(); // for correct hardware changing
    m_presetManager.setCurrentState(PresetState::Changing);
    pushReadPresetCommands();
    emit sgProcessCommands();
}

void CPModern::ackPresetSavedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit presetSaved();
}

void CPModern::copyCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "complete")
    {
        emit sgPushCommandToQueue("ls ir_library\r\n", false);
        pushReadPresetCommands();
    }
    else
    {
        qWarning() << "Copy error"; //TODO сообщение пользователю
        emit sgDeviceError(DeviceErrorType::CopyFileError);
    }
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
