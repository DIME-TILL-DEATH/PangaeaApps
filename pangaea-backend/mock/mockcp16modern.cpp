#include "mockcp16modern.h"

#include "presetmodern.h"

#include <QStandardPaths>
#include <QDir>
#include <QThread>

MockCP16Modern::MockCP16Modern(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : AbstractMockDevice{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_CPModern;

    initFolders();

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP16Modern::amtDevCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP16Modern::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("gb", std::bind(&MockCP16Modern::bankPresetCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("gm", std::bind(&MockCP16Modern::outputModeCommHandler, this,  _1, _2, _3));
    m_parser.addCommandHandler("outmode", std::bind(&MockCP16Modern::outputModeCommHandler, this,  _1, _2, _3));

    m_parser.addCommandHandler("state", std::bind(&MockCP16Modern::stateCommHandler, this,  _1, _2, _3));
    m_parser.addCommandHandler("pname", std::bind(&MockCP16Modern::pnameCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("ir", std::bind(&MockCP16Modern::irCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("ls", std::bind(&MockCP16Modern::listCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("list", std::bind(&MockCP16Modern::listCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("pls", std::bind(&MockCP16Modern::getPresetListCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("plist", std::bind(&MockCP16Modern::getPresetListCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("mconfig", std::bind(&MockCP16Modern::mconfigCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("rvconfig", std::bind(&MockCP16Modern::rvconfigCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sp", std::bind(&MockCP16Modern::savePresetCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pc", std::bind(&MockCP16Modern::presetChangeCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("esc", std::bind(&MockCP16Modern::escAckCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("copy", std::bind(&MockCP16Modern::copyCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("fsf", std::bind(&MockCP16Modern::formatMemoryCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("fwu", std::bind(&MockCP16Modern::startFwUpdateCommHandler, this, _1, _2, _3));

    //--------------------------params handler----------------------
    m_parser.addCommandHandler("eq0", std::bind(&MockCP16Modern::eqParametersCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("eq1", std::bind(&MockCP16Modern::eqParametersCommHandler, this, _1, _2, _3));

    setParamsHandler("mv", &currentPresetData.volume);   // Type::MASTER_VOLUME: fullString += "mv";

    setParamsHandler("eo", &currentPresetData.reverb.on);        // Type::EARLY_ON: fullString += "eo";
    setParamsHandler("ev", &currentPresetData.reverb.volume);    // Type::EARLY_VOLUME: fullString += "ev";
    setParamsHandler("et", &currentPresetData.reverb.type);      // Type::EARLY_TYPE: fullString += "et";

    setParamsHandler("ce", &currentPresetData.cab_sim_on);          // Type::CABINET_ENABLE: fullString += "ce";

    setParamsHandler("ao", &currentPresetData.power_amp.on);          // case Type::AMP_ON: fullString += "ao";
    setParamsHandler("av", &currentPresetData.power_amp.volume);      // case Type::AMP_VOLUME: fullString += "av";
    setParamsHandler("as", &currentPresetData.power_amp.slave);       // case Type::AMP_SLAVE: fullString += "as";
    setParamsHandler("at", &currentPresetData.power_amp.type);        // case Type::AMP_TYPE: fullString += "at";
    setParamsHandler("pv", &currentPresetData.power_amp.presence_vol);    // Type::PRESENCE_VOLUME: fullString += "pv";

    setParamsHandler("pro", &currentPresetData.preamp.on);      // Type::PREAMP_ON: fullString += "pro";
    setParamsHandler("prv", &currentPresetData.preamp.volume);  // Type::PREAMP_VOLUME: fullString += "prv";
    setParamsHandler("prl", &currentPresetData.preamp.low);     // Type::PREAMP_LOW: fullString += "prl";
    setParamsHandler("prm", &currentPresetData.preamp.mid);     // Type::PREAMP_MID: fullString += "prm";
    setParamsHandler("prh", &currentPresetData.preamp.high);    // Type::PREAMP_HIGH: fullString += "prh";

    setParamsHandler("go", &currentPresetData.gate.on);         // Type::GATE_ON: fullString += "go";
    setParamsHandler("gt", &currentPresetData.gate.threshold);  // Type::GATE_THRESHOLD: fullString += "gt";
    setParamsHandler("gd", &currentPresetData.gate.decay);      // Type::GATE_DECAY: fullString += "gd";

    setParamsHandler("co", &currentPresetData.compressor.on);       // Type::COMPRESSOR_ON: fullString += "co";
    setParamsHandler("cs", &currentPresetData.compressor.sustain);  // Type::COMPRESSOR_SUSTAIN: fullString += "cs";
    setParamsHandler("cv", &currentPresetData.compressor.volume);   // Type::COMPRESSOR_VOLUME: fullString += "cv";

    //---------------------New modules----------------------------
    setParamsHandler("tr_on", &currentPresetData.tremolo.on);
    setParamsHandler("tr_dp", &currentPresetData.tremolo.depth);
    setParamsHandler("tr_rt", &currentPresetData.tremolo.rate);
    setParamsHandler("tr_fm", &currentPresetData.tremolo.type);

    setParamsHandler("ch_on", &currentPresetData.chorus.on);
    setParamsHandler("ch_mx", &currentPresetData.chorus.mix);
    setParamsHandler("ch_rt", &currentPresetData.chorus.rate);
    setParamsHandler("ch_wd", &currentPresetData.chorus.width);
    setParamsHandler("ch_hp", &currentPresetData.chorus.hpf);

    setParamsHandler("ph_on", &currentPresetData.phaser.on);
    setParamsHandler("ph_mx", &currentPresetData.phaser.mix);
    setParamsHandler("ph_rt", &currentPresetData.phaser.rate);
    setParamsHandler("ph_wd", &currentPresetData.phaser.width);
    setParamsHandler("ph_cn", &currentPresetData.phaser.center);
    setParamsHandler("ph_fb", &currentPresetData.phaser.feedback);
    setParamsHandler("ph_st", &currentPresetData.phaser.stages);
    setParamsHandler("ph_hp", &currentPresetData.phaser.hpf);

    setParamsHandler("ch_on", &currentPresetData.delay.on);
    setParamsHandler("ch_mx", &currentPresetData.delay.mix);
    setParamsHandler("ch_rt", &currentPresetData.delay.time);
    setParamsHandler("ch_wd", &currentPresetData.delay.feedback);
    setParamsHandler("ch_hp", &currentPresetData.delay.hpf);
    setParamsHandler("ch_hp", &currentPresetData.delay.lpf);

    //--------------------------------------------------------------

    QFile systemFile(basePath + "/system.pan");
    system_parameters_t sysParameters;
    memset(&sysParameters, 0, sizeof(system_parameters_t));
    if(systemFile.open(QIODevice::ReadOnly))
    {
        QByteArray readedData = systemFile.read(sizeof(system_parameters_t));
        memcpy(&sysParameters, readedData.data(), sizeof(system_parameters_t));
        m_outputMode = sysParameters.output_mode;
        systemFile.close();
    }

    // save_data_t saveData{0};
    // loadPresetData(m_bank, m_preset, &saveData);
    // currentPresetName = QString(saveData.name);
    // memcpy(&currentPresetData, &saveData.parametersData, sizeof(preset_data_t));

    changePreset(0, 0);
}

void MockCP16Modern::writeToDevice(const QByteArray &data)
{
    if(fwUpdateMode)
    {
        fwPart.append(data);
        if(fwPart.indexOf('\n') != -1)
        {
            qint64 separatorPos = fwPart.indexOf('\n');
            fwChunkSize = QString(fwPart.left(separatorPos)).toInt();
            fwPart.remove(0, separatorPos + 1);
        }

        if(fwChunkSize == 0)
        {
            fwFile.close();
            fwUpdateMode = false;
            emit answerReady("FR_OK\n");
        }
        else
        {
            if(fwPart.size() >= fwChunkSize)
            {
                qint64 written = fwFile.write(fwPart);
                qDebug() << "Bytes written: " << written << "file size" << fwFile.size();
                fwPart.clear();
                emit answerReady("REQUEST_CHUNK_SIZE\n");
            }
        }
    }
    else
    {
        m_parser.parseNewData(data);
    }
}

void MockCP16Modern::newDataRecieved()
{
    if(fwUpdateMode) // TODO change to threaded loading
    {
        fwPart.append(*m_uartBuffer);
        if(fwPart.indexOf('\n') != -1)
        {
            qint64 separatorPos = fwPart.indexOf('\n');
            fwChunkSize = QString(fwPart.left(separatorPos)).toInt();
            fwPart.remove(0, separatorPos + 1);
        }

        if(fwChunkSize == 0)
        {
            fwFile.close();
            fwUpdateMode = false;
            emit answerReady("FR_OK\n");
        }
        else
        {
            if(fwPart.size() >= fwChunkSize)
            {
                qint64 written = fwFile.write(fwPart);
                qDebug() << "Bytes written: " << written << "file size" << fwFile.size();
                fwPart.clear();
                emit answerReady("REQUEST_CHUNK_SIZE\n");
            }
        }
    }
    else
    {
        m_parser.parseNewData(*m_uartBuffer);
    }

    QMutexLocker locker(m_mutex);
    m_uartBuffer->clear();
}

void MockCP16Modern::initFolders()
{
#ifdef Q_OS_ANDROID
    basePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/";
#else
    QString basePath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) + "/AMT/pangaeaCPPA/";

#endif
    if(!QDir(basePath).exists()) QDir().mkpath(basePath);

    basePath += m_mockName;

    QDir::setCurrent(basePath);
    qInfo() << "Current mock directory:" << QDir::current();

    if(!QDir(basePath).exists()) QDir().mkpath(basePath);

    QString libPath = basePath + "/ir_library/";
    if(!QDir().exists(libPath)) QDir().mkpath(libPath);

    QFile systemFile(basePath + "/system.pan");
    if(!systemFile.exists())
    {
        m_systemParameters.output_mode = 0;
        m_systemParameters.la3_cln_preset = 0x00;
        m_systemParameters.la3_drv_preset = 0x20;
        saveSysParameters();
    }
    else
    {
        if(systemFile.open(QIODevice::ReadOnly))
        {
            QByteArray baFileData = systemFile.readAll();
            memcpy(&m_systemParameters, baFileData.data(), sizeof(system_parameters_t));
            systemFile.close();
        }
    }

    for(int b = 0; b <4; b++)
    {
        QString bankPath = "bank_" + QString().setNum(b);
        if(!QDir().exists(bankPath)) QDir().mkpath(bankPath);

        for(int p=0; p<4; p++)
        {
            QString presetPath = bankPath + "/preset_" + QString().setNum(p);
            if(!QDir().exists(presetPath)) QDir().mkpath(presetPath);

            QFile presetFile(presetPath + "/preset.pan");
            if(!presetFile.exists())
            {
                presetFile.open(QIODevice::WriteOnly);

                preset_data_legacy_t defaultData;
                memset(&defaultData, 0, sizeof(preset_data_legacy_t));

                defaultData.preset_volume = 20;

                defaultData.eq_band_vol[0] = 15;
                defaultData.eq_band_vol[1] = 15;
                defaultData.eq_band_vol[2] = 15;
                defaultData.eq_band_vol[3] = 15;
                defaultData.eq_band_vol[4] = 15;

                defaultData.presence_vol = 31;

                defaultData.cab_on = 1;

                defaultData.amp_volume = 16;
                defaultData.amp_slave = 16;
                defaultData.preamp_volume = 25;

                char rawData[sizeof(preset_data_legacy_t)];
                memcpy(rawData, &defaultData, sizeof(preset_data_legacy_t));

                presetFile.write(rawData, sizeof(preset_data_legacy_t));
                presetFile.close();
            }
        }
    }
}

bool MockCP16Modern::saveSysParameters()
{
    QFile systemFile(basePath + "/system.pan");

    QString ver("2.01.00");
    memcpy(m_systemParameters.firmware_version, ver.data(), ver.size());

    if(systemFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(system_parameters_t)];
        memcpy(rawData, &m_systemParameters, sizeof(system_parameters_t));
        systemFile.write(rawData, sizeof(system_parameters_t));

        systemFile.close();
        return true;
    }
    else return false;
}

bool MockCP16Modern::loadPresetData(quint8 prBank, quint8 prPreset, save_data_t *presetSavedData)
{
    QString dirPath = "bank_" + QString().setNum(prBank) + "/preset_" + QString().setNum(prPreset);
    QFile presetFile(dirPath + "/preset.pa2");
    if(presetFile.open(QIODevice::ReadOnly))
    {
        QByteArray readedData = presetFile.read(sizeof(save_data_t));
        memcpy(presetSavedData, readedData.data(), sizeof(save_data_t));
        presetFile.close();
        return true;
    }
    else
    {
        QFile presetFile(dirPath + "/preset.pan");
        if(presetFile.open(QIODevice::ReadOnly))
        {
            preset_data_legacy_t legacydata;
            QByteArray readedData = presetFile.read(sizeof(preset_data_legacy_t));
            memcpy(&legacydata, readedData.data(), sizeof(preset_data_legacy_t));
            presetFile.close();
            memset(presetSavedData->name, 0, 64);
            presetSavedData->parametersData = HardwarePreset::convertLegacyToModern(legacydata);
            return true;
        }
        else
        {
            qWarning() << "MOCK DEVICE: can't open any preset file, b:" << prBank << "p:" << prPreset;
            return false;
        }
    }
}

bool MockCP16Modern::savePresetData(quint8 prBank, quint8 prPreset, const save_data_t* presetSaveData)
{
    QString dirPath = "bank_" + QString().setNum(prBank) + "/preset_" + QString().setNum(prPreset);
    QFile presetFile(dirPath + "/preset.pa2");
    QFile linkFile(dirPath + "/ir.lnk");
    if(presetFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(save_data_t)];
        memcpy(rawData, presetSaveData, sizeof(save_data_t));
        presetFile.write(rawData, sizeof(save_data_t));
        presetFile.close();

    }
    else return false;

    if(linkedIr.isLinked())
    {
        if(linkFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            linkFile.write(linkedIr.irName().toUtf8());
            linkFile.write("\n");
            linkFile.write(linkedIr.irLinkPath().toUtf8());
            linkFile.close();
        }
    }
    return true;
}

//===========================================================================================
//  comm handlers
//===========================================================================================

void MockCP16Modern::amtDevCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r16\nEND\n").toUtf8());
}

void MockCP16Modern::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtver\r2.01.00\nEND\n").toUtf8());
}

void MockCP16Modern::bankPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString answer = QString("gb\r")
    + QString("0%1").arg(m_bank)
        + QString("0%1").arg(m_preset) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Modern::outputModeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments.size()>0)
    {
        m_outputMode = QString(arguments.at(0)).toInt();
        saveSysParameters();
    }
    QString answer = "gm\r0" + QString().setNum(m_outputMode) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Modern::pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "set")
    {
        currentPresetName = data;
    }
    QString answer = "pname\r" + currentPresetName + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Modern::stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray answer = QString("state").toUtf8();

    if(arguments == "set")
    {
        currentPresetData = PresetModern::charsToPresetData(data);
        answer += " set\r\n";
    }
    else
    {
        answer.append("\r");
        answer.append(PresetModern::presetDataToChars(currentPresetData));
        answer.append("\n");
    }
    emit answerReady(answer);
}

void MockCP16Modern::irCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "info")
    {
        getIrInfo();
    }

    if(arguments == "download")
    {
        irDownload(data);
    }

    if(arguments == "preview")
    {
        emit answerReady("ir preview\rOK\n");
    }

    if(arguments == "start_upload")
    {
        QByteArray answer;

        QList<QByteArray> dataList = data.split('\r');
        qDebug() << "DataList size" << dataList.size();
        if(dataList.size() == 2)
        {
            QString fileName = dataList.at(0);
            QString destPath = dataList.at(1);

            uploadingIrFile.setFileName(destPath + fileName);
            qDebug() << "MOCK device: start uploading to: " << destPath + fileName;
            if(uploadingIrFile.open(QIODevice::WriteOnly))
            {
                uploadingIrFile.close();
                answer = "ir request_part\r\n";
            }
            else
            {
                answer = "ir error\rDST_PATH_INCORRECT\n";
            }
            emit answerReady(answer);
        }
        else
        {
            emit answerReady("ir error\rCOMMAND_INCORRECT\n");
        }
    }

    QStringList argsList = QString(arguments).split(" ");
    if(argsList.size() > 1)
    {
        quint16 partSize = argsList.at(1).toInt();

        if(argsList.at(0) == "part_upload")
        {
            QStringList argsList = QString(arguments).split(" ");

            quint16 partSize;
            if(argsList.size() > 1)
                partSize = argsList.at(1).toInt();

            while (m_uartBuffer->size() < partSize)
            {
                QThread::sleep(100);
            }
            QMutexLocker locker(m_mutex);

            QByteArray recievedPart = m_uartBuffer->right(partSize+1);
            recievedPart.chop(1); // remove \n
            m_uartBuffer->clear();

            if(uploadingIrFile.open(QIODevice::Append))
            {
                uploadingIrFile.write(recievedPart);
                uploadingIrFile.close();
                emit answerReady("ir request_part\r\n");
            }
        }
    }

    if(arguments == "link")
    {
        if(data.size()>0)
        {
            QList<QByteArray> dataList = data.split('\r');

            linkedIr.setIrName(dataList.at(0));
            linkedIr.setIrLinkPath(dataList.at(1));
        }

        QByteArray answer("ir link\r");
        answer += linkedIr.irName().toUtf8();
        answer += "\r";
        answer += linkedIr.irLinkPath().toUtf8();
        answer += "\n";
        emit answerReady(answer);
    }

    if(arguments == "delete")
    {
        QByteArray answer("ir delete\r");

        QFile file(data);
        if(file.remove())
        {
            answer += "OK\n";
        }
        else
        {
            answer += "ERROR\n";
        }
        emit answerReady(answer);
    }
}

void MockCP16Modern::getIrInfo()
{
    QString irLinkPath;
    QString irName = "";
    qint64 irSize = -1;

    QString pathToDir = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);

    if(linkedIr.isLinked())
    {
        irName = linkedIr.irName();
                 irLinkPath = linkedIr.irLinkPath();
        irSize = QFileInfo(irLinkPath + "/" + irName).size();
    }
    else
    {
        QDir presetDir(pathToDir);
        presetDir.setNameFilters({"*.wav"});
        QFileInfoList filesInDir = presetDir.entryInfoList();
        if(!filesInDir.empty())
        {
            irName = filesInDir.first().fileName();
            irSize = filesInDir.first().size();
        }
    }

    QByteArray baData = QString("ir info\r").toUtf8() + irLinkPath.toUtf8() + QString("\r").toUtf8() +
                        irName.toUtf8() + QString("\r").toUtf8() +
                        QString().setNum(irSize).toUtf8() + QString("\n").toUtf8();
    emit answerReady(baData);
}

void MockCP16Modern::irDownload(const QString& pathToIr)
{
    QString filePath = basePath + "/" + pathToIr;
    qDebug() << " Mock device ir download, path to file:" << filePath;

    QByteArray answer("ir download\r" + pathToIr.toUtf8() + "\r");

    QByteArray fileData;
    QFile irFile(filePath);
    if(irFile.open(QIODevice::ReadOnly))
    {
        fileData = irFile.readAll();
        irFile.close();
        QByteArray convertedBa;
        for(quint16 i=0; i<fileData.size(); i++)
        {
            QString sTmp;
            quint8  chr;

            chr = fileData.at(i);
            sTmp = QString("%1").arg (chr, 2, 16, QChar('0'));

            convertedBa.append(sTmp.toUtf8());
        }
        answer += convertedBa;
    }
    answer += "\n";
    emit answerReady(answer);
}

void MockCP16Modern::listCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray answer(command.toUtf8() + " " + arguments);

    QDir lsDir(arguments);
    lsDir.setNameFilters({"*.wav"});

    QFileInfoList filesInDir = lsDir.entryInfoList();

    if(filesInDir.isEmpty())
    {
        answer += "\r";
    }
    else
    {
        foreach(QFileInfo wavFile, filesInDir)
        {
            answer += "\r" + wavFile.fileName().toUtf8();
        }
    }
    answer += "\n";
    emit answerReady(answer);
}

void MockCP16Modern::mconfigCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> argsList = arguments.split('\r');
    if(argsList.size()>0)
    {
        if(argsList.first() == "set")
        {
            memset(currentPresetData.modules_order, 0, MAX_PROCESSING_STAGES);
            QByteArray listData = QByteArray::fromHex(data);
            memcpy(currentPresetData.modules_order, listData.data(), listData.size());
        }
    }

    QByteArray baOrder;
    for(int i=0; i < MAX_PROCESSING_STAGES;  i++)
    {
        QByteArray tempBa = QString().setNum(currentPresetData.modules_order[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baOrder.append(tempBa);
    }

    emit answerReady("mconfig\r" + baOrder + "\n");
}

void MockCP16Modern::rvconfigCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> argsList = arguments.split('\r');
    if(argsList.size()>0)
    {
        if(argsList.first() == "set")
        {
            memset(currentPresetData.reverb_config, 0, 2);
            QByteArray listData = QByteArray::fromHex(data);
            memcpy(currentPresetData.reverb_config, listData.data(), listData.size());
        }
    }

    QByteArray baOrder;
    for(int i=0; i < 2;  i++)
    {
        QByteArray tempBa = QString().setNum(currentPresetData.reverb_config[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baOrder.append(tempBa);
    }

    emit answerReady("rvconfig\r" + baOrder + "\n");
}

void MockCP16Modern::getPresetListCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString answer("plist");

    for(int b = 0; b <4; b++)
    {
        for(int p=0; p<4; p++)
        {
            answer += "\r";

            QString irLinkName, irFolderName;
            QString dirPath = "bank_" + QString().setNum(b) + "/preset_" + QString().setNum(p);

            QFile linkFile(dirPath + "/ir.lnk");
            if(linkFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString fileData = linkFile.readAll();
                QStringList dataList = fileData.split("\n");
                if(dataList.size()>1)
                {
                    QFile checkIrFile(dataList.at(1) + "/" + dataList.at(0));
                    if(checkIrFile.exists()) irLinkName = dataList.at(0);
                }
                else
                {
                    qWarning() << "lnk file corrupted";
                }
                linkFile.close();
            }

            QDir presetDir(dirPath);
            presetDir.setNameFilters({"*.wav"});

            QFileInfoList filesInDir = presetDir.entryInfoList();
            if(!filesInDir.empty()) irFolderName = filesInDir.first().fileName();


            if(irLinkName.isEmpty()) answer += irFolderName + "|";
            else answer += irLinkName + "|";


            QString enabled = "00";
            QString presetName;
            save_data_t presetData;
            if(loadPresetData(b, p, &presetData))
            {
                enabled.setNum(presetData.parametersData.cab_sim_on);
                enabled.prepend("0");

                presetName = presetData.name;
            }

            answer += enabled + "|";
            answer += presetName;
        }
    }
    answer += "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Modern::savePresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    save_data_t saveData;
    QByteArray bytesName = currentPresetName.toUtf8();

    memcpy(&saveData.name, bytesName.data(), 64);
    saveData.parametersData = currentPresetData;

    savePresetData(m_bank, m_preset, &saveData);
    emit answerReady("sp\r\n");
}

void MockCP16Modern::presetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 bank = QString(arguments.at(0)).toInt(nullptr, 16);
    quint8 preset = QString(arguments.at(1)).toInt(nullptr, 16);

    changePreset(bank, preset);

    QString answer = QString("pc %1%2\rEND\n").arg(QString().setNum(m_bank, 16)).arg(QString().setNum(m_preset, 16));
    emit answerReady(answer.toUtf8());
}

void MockCP16Modern::copyCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> dataList = data.split('\r');
    if(dataList.size() == 2)
    {
        QString srcPath = dataList.at(0);
        QString dstPath = dataList.at(1);

        QFile srcFile(srcPath);
        if(srcFile.copy(dstPath))
        {
            emit answerReady("copy complete\r\n");
        }
        else
        {
            emit answerReady("copy error\r\n");
        }
    }
    else
    {
        qWarning() << "MOCK device: copy command format incorrect";
        emit answerReady("copy error\r\n");
    }
}

void MockCP16Modern::changePreset(quint8 bank, quint8 preset)
{
    m_bank = bank;
    m_preset = preset;

    QString pathToDir = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);
    QFile linkFile(pathToDir + "/ir.lnk");
    if(linkFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString irFilename, irLinkPath;
        QString fileData = linkFile.readAll();
        QStringList dataList = fileData.split("\n");
        if(dataList.size()>1)
        {
            irFilename = dataList.at(0);
            irLinkPath = dataList.at(1);
            QFile checkIrFile(irLinkPath + "/" + irFilename);
            if(checkIrFile.exists())
            {
                linkedIr.setIrName(irFilename);
                linkedIr.setIrLinkPath(irLinkPath);
                linkFile.close();
            }
            else
            {
                qWarning() << "MOCK: IR file in link not found." << irFilename;
                linkedIr.clear();
                linkFile.close();
                linkFile.remove();
            }
        }
        else
        {
            qWarning() << "lnk file corrupted";
            linkedIr.clear();
            linkFile.close();
        }
    }
    else
    {
        linkedIr.clear();
    }

    save_data_t saveData{0};
    loadPresetData(m_bank, m_preset, &saveData);
    currentPresetName = QString(saveData.name);
    memcpy(&currentPresetData, &saveData.parametersData, sizeof(preset_data_t));
}

void MockCP16Modern::formatMemoryCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QDir homeDir(basePath);
    homeDir.removeRecursively();
    initFolders();

    emit answerReady("fsf\rFR_OK\n");
}

void MockCP16Modern::escAckCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady("esc\r\n");
}

void MockCP16Modern::setParamsHandler(QString commStr, quint8 *commPtr)
{
    paramsByteMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP16Modern::parametersByteCommHandler, this, _1, _2, _3));
}

void MockCP16Modern::setParamsHandler(QString commStr, quint16 *commPtr)
{
    paramsWordMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP16Modern::parametersWordCommHandler, this, _1, _2, _3));
}

void MockCP16Modern::parametersByteCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8* paramPtr = paramsByteMap.value(command);

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        *paramPtr = correctedArgs.toInt(nullptr, 16);
    }
    emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r\n");
}

void MockCP16Modern::parametersWordCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint16* paramPtr = paramsWordMap.value(command);

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        *paramPtr = correctedArgs.toInt(nullptr, 16);
    }
    emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r\n");
}

void MockCP16Modern::eqParametersCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8* paramPtr = nullptr;

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        QList<QByteArray> argsList = correctedArgs.split(' ');
        if(argsList.size() < 3)
        {
            qWarning() << "MOCK DEVICE: incomplete eq args:" << argsList;
        }

        qint8 band = -1;
        if(argsList.at(0) == "b0") band=0;
        if(argsList.at(0) == "b1") band=1;
        if(argsList.at(0) == "b2") band=2;
        if(argsList.at(0) == "b3") band=3;
        if(argsList.at(0) == "b4") band=4;

        qint8 val = argsList.at(2).toInt(nullptr, 16);
        qint16 freqBand = argsList.at(2).toInt(nullptr, 16);

        if(command == "eq0")
        {
            if(band != -1)
            {
                if(argsList.at(1) == "f") currentPresetData.eq1.freq[band] = freqBand;
                if(argsList.at(1) == "g") currentPresetData.eq1.gain[band] = val;
                if(argsList.at(1) == "q") currentPresetData.eq1.Q[band] = val;
                if(argsList.at(1) == "t") currentPresetData.eq1.band_type[band] = val;
            }
            else
            {
                if(argsList.at(0) == "hp")
                {
                    if(argsList.at(1) == "o") currentPresetData.eq1.hp_on = val;
                    if(argsList.at(1) == "f") currentPresetData.eq1.hp_freq = freqBand;
                }
                if(argsList.at(0) == "lp")
                {
                    if(argsList.at(1) == "o") currentPresetData.eq1.lp_on = val;
                    if(argsList.at(1) == "f") currentPresetData.eq1.lp_freq = freqBand;
                }
                if(argsList.at(0) == "par")
                {
                    currentPresetData.eq1.parametric_on = val;
                }
            }
        }
        else
        {
            if(band != -1)
            {
                if(argsList.at(1) == "f") currentPresetData.eq2.freq[band] = freqBand;
                if(argsList.at(1) == "g") currentPresetData.eq2.gain[band] = val;
                if(argsList.at(1) == "q") currentPresetData.eq2.Q[band] = val;
                if(argsList.at(1) == "t") currentPresetData.eq2.band_type[band] = val;
            }
            else
            {
                if(argsList.at(0) == "hp")
                {
                    if(argsList.at(1) == "o") currentPresetData.eq2.hp_on = val;
                    if(argsList.at(1) == "f") currentPresetData.eq2.hp_freq = freqBand;
                }
                if(argsList.at(0) == "lp")
                {
                    if(argsList.at(1) == "o") currentPresetData.eq2.lp_on = val;
                    if(argsList.at(1) == "f") currentPresetData.eq2.lp_freq = freqBand;
                }
                if(argsList.at(0) == "par")
                {
                    currentPresetData.eq2.parametric_on = val;
                }
            }
        }
    }
}

void MockCP16Modern::startFwUpdateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    fwUpdateMode = true;
    fwChunkSize = QString(arguments).toInt();

    fwPart = m_parser.getBufferAndFlush();

    qDebug() << fwPart;

    fwFile.setFileName(basePath + "/firmware.bin");
    if(fwFile.open(QIODevice::WriteOnly))
    {
        fwFile.write(fwPart);
    }

    fwPart.clear();

    emit answerReady("fwu\rCHUNK_MAX_SIZE_4096\n");
    emit answerReady("REQUEST_CHUNK_SIZE\n");

    // while (m_uartBuffer->size() < fwChunkSize)
    // {
    //     QThread::sleep(100);
    // }
    // QMutexLocker locker(m_mutex);

    // QByteArray recievedPart = m_uartBuffer->right(partSize+1);
    // recievedPart.chop(1); // remove \n
    // m_uartBuffer->clear();
}
