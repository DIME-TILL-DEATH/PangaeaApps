#include "mockcp16legacy.h"

#include <QStandardPaths>
#include <QDir>

MockCP16Legacy::MockCP16Legacy(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : AbstractMockDevice{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_CPLegacy;

    initFolders();

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP16Legacy::amtDevCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP16Legacy::amtVerCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("gb", std::bind(&MockCP16Legacy::bankPresetCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("gm", std::bind(&MockCP16Legacy::outputModeCommHandler, this,  _1, _2, _3));
    m_parser.addCommandHandler("gs", std::bind(&MockCP16Legacy::getStateCommHandler, this,  _1, _2, _3));
    m_parser.addCommandHandler("rn", std::bind(&MockCP16Legacy::getImpulseNameCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("rns", std::bind(&MockCP16Legacy::getRnsCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sp", std::bind(&MockCP16Legacy::savePresetCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pc", std::bind(&MockCP16Legacy::presetChangeCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("cc", std::bind(&MockCP16Legacy::ccCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("lcc", std::bind(&MockCP16Legacy::lccCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("esc", std::bind(&MockCP16Legacy::escAckCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("fsf", std::bind(&MockCP16Legacy::formatMemoryCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("fwu", std::bind(&MockCP16Legacy::startFwUpdateCommHandler, this, _1, _2, _3));

    //--------------------------params handler----------------------

    setParamsHandler("mv", &currentPresetData.preset_volume);   // Type::MASTER_VOLUME: fullString += "mv";

    setParamsHandler("eo", &currentPresetData.early_on);        // Type::EARLY_ON: fullString += "eo";
    setParamsHandler("ev", &currentPresetData.early_volume);    // Type::EARLY_VOLUME: fullString += "ev";
    setParamsHandler("et", &currentPresetData.early_type);      // Type::EARLY_TYPE: fullString += "et";

    setParamsHandler("ce", &currentPresetData.cab_on);          // Type::CABINET_ENABLE: fullString += "ce";

    setParamsHandler("ao", &currentPresetData.amp_on);          // case Type::AMP_ON: fullString += "ao";
    setParamsHandler("av", &currentPresetData.amp_volume);      // case Type::AMP_VOLUME: fullString += "av";
    setParamsHandler("as", &currentPresetData.amp_slave);       // case Type::AMP_SLAVE: fullString += "as";
    setParamsHandler("at", &currentPresetData.amp_type);        // case Type::AMP_TYPE: fullString += "at";

    setParamsHandler("pro", &currentPresetData.preamp_on);      // Type::PREAMP_ON: fullString += "pro";
    setParamsHandler("prv", &currentPresetData.preamp_volume);  // Type::PREAMP_VOLUME: fullString += "prv";
    setParamsHandler("prl", &currentPresetData.preamp_low);     // Type::PREAMP_LOW: fullString += "prl";
    setParamsHandler("prm", &currentPresetData.preamp_mid);     // Type::PREAMP_MID: fullString += "prm";
    setParamsHandler("prh", &currentPresetData.preamp_high);    // Type::PREAMP_HIGH: fullString += "prh";

    setParamsHandler("go", &currentPresetData.gate_on);         // Type::GATE_ON: fullString += "go";
    setParamsHandler("gt", &currentPresetData.gate_threshold);  // Type::GATE_THRESHOLD: fullString += "gt";
    setParamsHandler("gd", &currentPresetData.gate_decay);      // Type::GATE_DECAY: fullString += "gd";

    setParamsHandler("co", &currentPresetData.compressor_on);       // Type::COMPRESSOR_ON: fullString += "co";
    setParamsHandler("cs", &currentPresetData.compressor_sustain);  // Type::COMPRESSOR_SUSTAIN: fullString += "cs";
    setParamsHandler("cv", &currentPresetData.compressor_volume);   // Type::COMPRESSOR_VOLUME: fullString += "cv";

    setParamsHandler("lv", &currentPresetData.lp_freq);     // Type::LPF_VOLUME: fullString += "lv";
    setParamsHandler("hv", &currentPresetData.hp_freq);     // Type::HPF_VOLUME: fullString += "hv";
    setParamsHandler("ho", &currentPresetData.hp_on);       // Type::HPF_ON: fullString += "ho";
    setParamsHandler("lo", &currentPresetData.lp_on);       // Type::LPF_ON: fullString += "lo";
    setParamsHandler("po", &currentPresetData.presence_on);     // Type::PRESENCE_ON: fullString += "po";
    setParamsHandler("pv", &currentPresetData.presence_vol);    // Type::PRESENCE_VOLUME: fullString += "pv";

    setParamsHandler("eqo", &currentPresetData.eq_on);          // Type::EQ_ON: fullString += "eqo";
    setEqHandler("eqv", currentPresetData.eq_band_vol);     // Type::EQ_VOLUME1: fullString += "eqv 0";
    setEqHandler("eqf", currentPresetData.eq_freq);         // Type::EQ_FREQ1: fullString += "eqf 0";
    setEqHandler("eqq", currentPresetData.eq_Q);            // Type::EQ_Q1: fullString += "eqq 0";
    setParamsHandler("eqp", &currentPresetData.eq_pre);     // Type::EQ_PRE: fullString += "eqp";

    //--------------------------------------------------------------

    QFile systemFile(m_basePath + "/system.pan");
    system_parameters_cp_t sysParameters;
    memset(&sysParameters, 0, sizeof(system_parameters_cp_t));
    if(systemFile.open(QIODevice::ReadOnly))
    {
        QByteArray readedData = systemFile.read(sizeof(system_parameters_cp_t));
        memcpy(&sysParameters, readedData.data(), sizeof(system_parameters_cp_t));
        m_outputMode = sysParameters.output_mode;
        systemFile.close();
    }

    loadPresetData(m_bank, m_preset, &currentPresetData);
}

void MockCP16Legacy::writeToDevice(const QByteArray &data)
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

void MockCP16Legacy::newDataRecieved()
{
    if(fwUpdateMode)
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

void MockCP16Legacy::initFolders()
{
#ifdef Q_OS_ANDROID
    m_basePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/";
#else
    m_basePath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) + "/";
#ifndef Q_OS_IOS
    m_basePath += "AMT/pangaeaCPPA/";
#endif
#endif
    m_basePath += "virtual_CP16Legacy";
    
    if(!QDir(m_basePath).exists()) QDir().mkpath(m_basePath);

    QFile systemFile(m_basePath + "/system.pan");
    if(!systemFile.exists())
    {
        m_outputMode = 0;
        saveSysParameters();
    }

    for(int b = 0; b <4; b++)
    {
        QString bankPath = m_basePath + "/bank_" + QString().setNum(b);
        if(!QDir().exists(bankPath)) QDir().mkpath(bankPath);

        for(int p=0; p<4; p++)
        {
            QString presetPath = bankPath + "/preset_" + QString().setNum(p);
            if(!QDir().exists(presetPath)) QDir().mkpath(presetPath);

            QFile presetFile(presetPath + "/preset.pan");
            if(!presetFile.exists())
            {
                presetFile.open(QIODevice::WriteOnly);

                preset_cplegacy_t defaultData;
                memset(&defaultData, 0, sizeof(preset_cplegacy_t));

                defaultData.preset_volume = 31;

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

                char rawData[sizeof(preset_cplegacy_t)];
                memcpy(rawData, &defaultData, sizeof(preset_cplegacy_t));

                presetFile.write(rawData, sizeof(preset_cplegacy_t));
                presetFile.close();
            }
        }
    }
}

bool MockCP16Legacy::saveSysParameters()
{
    QFile systemFile(m_basePath + "/system.pan");

    system_parameters_cp_t sysParameters;
    memset(&sysParameters, 0, sizeof(system_parameters_cp_t));
    sysParameters.output_mode = m_outputMode;
    QString ver("1.04.04");
    memcpy(sysParameters.firmware_version, ver.data(), ver.size());

    if(systemFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(system_parameters_cp_t)];
        memcpy(rawData, &sysParameters, sizeof(system_parameters_cp_t));
        systemFile.write(rawData, sizeof(system_parameters_cp_t));

        systemFile.close();
        return true;
    }
    else return false;
}

bool MockCP16Legacy::loadPresetData(quint8 prBank, quint8 prPreset, preset_cplegacy_t *presetData)
{
    QString dirPath = m_basePath + "/bank_" + QString().setNum(prBank) + "/preset_" + QString().setNum(prPreset);
    QFile presetFile(dirPath + "/preset.pan");
    if(presetFile.open(QIODevice::ReadOnly))
    {
        QByteArray readedData = presetFile.read(sizeof(preset_cplegacy_t));
        memcpy(presetData, readedData.data(), sizeof(preset_cplegacy_t));
        presetFile.close();
        return true;
    }
    else return false;
}

bool MockCP16Legacy::savePresetData(quint8 prBank, quint8 prPreset, const preset_cplegacy_t *presetData)
{
    QString dirPath = m_basePath + "/bank_" + QString().setNum(prBank) + "/preset_" + QString().setNum(prPreset);
    QFile presetFile(dirPath + "/preset.pan");
    if(presetFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(preset_cplegacy_t)];
        memcpy(rawData, &currentPresetData, sizeof(preset_cplegacy_t));

        presetFile.write(rawData, sizeof(preset_cplegacy_t));
        presetFile.close();
        return true;
    }
    else return false;
}

//===========================================================================================
//  base comm handlers
//===========================================================================================
void MockCP16Legacy::amtDevCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r3\nEND\n").toUtf8());
}

void MockCP16Legacy::amtVerCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data)
{
    emit answerReady(QString("amtver\r1.04.04\nEND\n").toUtf8());
}

void MockCP16Legacy::bankPresetCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data)
{
    QString answer = QString("gb\r")
                    + QString("0%1").arg(m_bank)
                    + QString("0%1").arg(m_preset) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Legacy::outputModeCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data)
{
    if(arguments.size()>0)
    {
        m_outputMode = QString(arguments.at(0)).toInt();
        saveSysParameters();
    }
    QString answer = "gm\r0" + QString().setNum(m_outputMode) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Legacy::getStateCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data)
{
    if(data.size() == 0)
    {
        quint8 rawData[sizeof(preset_cplegacy_t)];
        memcpy(rawData, &currentPresetData, sizeof(preset_cplegacy_t));

        QByteArray baData = QString("gs\r").toUtf8();

        for(int i=0; i < sizeof(preset_cplegacy_t);  i++)
        {
            QByteArray tempBa = QString().setNum(rawData[i], 16).toUtf8();

            if(tempBa.size() == 1) tempBa.push_front("0");
            baData.append(tempBa);
        }
        baData.append("\n");

        emit answerReady(baData);
    }
    else
    {
        QList<QByteArray> dataList = data.split('\r');
        if(dataList.size()>0)
        {
            qDebug() << "Mock device, set state arguments:" << dataList.at(1);
            quint8 rawArr[sizeof(preset_cplegacy_t)];
            for(int i = 0; i < dataList.at(0).size(); i += 2)
            {
                QString chByte = QString(dataList.at(0).at(i)) + QString(dataList.at(0).at(i+1));
                rawArr[i/2] = chByte.toInt(nullptr, 16);
            }
            memcpy(&currentPresetData, rawArr, sizeof(preset_cplegacy_t));
        }
        emit answerReady("gs 1\rgsEND\n");
    }
}

void MockCP16Legacy::getImpulseNameCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data)
{
    QString impulseName = "";

    QDir presetDir(m_basePath + "/bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset));
    presetDir.setNameFilters({"*.wav"});

    QFileInfoList filesInDir = presetDir.entryInfoList();
    if(!filesInDir.empty())
    {
        impulseName = filesInDir.first().fileName();
    }

    QByteArray baData = QString("rn\r").toUtf8() + impulseName.toUtf8() + QString("\n").toUtf8();
    emit answerReady(baData);
}

void MockCP16Legacy::getRnsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString answer("rns\r");

    for(int b = 0; b <4; b++)
    {
        for(int p=0; p<4; p++)
        {
            QString irName = "*";
            QString dirPath = m_basePath + "/bank_" + QString().setNum(b) + "/preset_" + QString().setNum(p);
            QDir presetDir(dirPath);
            presetDir.setNameFilters({"*.wav"});

            QFileInfoList filesInDir = presetDir.entryInfoList();
            if(!filesInDir.empty())
            {
                irName = filesInDir.first().fileName();
            }
            answer += irName + "\n";

            QString enabled = "00";
            preset_cplegacy_t presetData;
            if(loadPresetData(b, p, &presetData))
            {
                enabled.setNum(presetData.cab_on);
                enabled.prepend("0");
            }

            answer += enabled + "\n";
        }
    }
    answer += "END\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Legacy::savePresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    savePresetData(m_bank, m_preset, &currentPresetData);
    emit answerReady("sp\r\n");
}

void MockCP16Legacy::presetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_bank = QString(arguments.at(0)).toInt(nullptr, 16);
    m_preset = QString(arguments.at(1)).toInt(nullptr, 16);

    loadPresetData(m_bank, m_preset, &currentPresetData);

    QString answer = QString("pc %1%2\rEND\n").arg(QString().setNum(m_bank, 16)).arg(QString().setNum(m_preset, 16));
    emit answerReady(answer.toUtf8());
}

void MockCP16Legacy::ccCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString dirPath = m_basePath + "/bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset) + "/";
    if(arguments.size() == 0)
    {
        qDebug() << " Mock device: " << "cc, no arguments";
        // file request
        QByteArray answer("cc\r");
        QDir presetDir(dirPath);
        presetDir.setNameFilters({"*.wav"});

        QFileInfoList filesInDir = presetDir.entryInfoList();
        if(!filesInDir.empty())
        {
            QString irName = filesInDir.first().fileName();
            int irSize = filesInDir.first().size();

            qDebug() << "Files in dir:" << filesInDir;
            qDebug() << " Mock device: " << "cc, file name, size" << irName << irSize;
            answer += irName.toUtf8() + " " + QString().setNum(irSize).toUtf8() + "\n";

            QByteArray fileData;
            QFile irFile(dirPath + irName);
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
            answer += "\nEND\n";
        }
        else
        {
            answer += "\nFILE_NOT_FIND\n";
        }

        emit answerReady(answer);
    }
    else
    {
        // file upload(preview and save)
        QByteArray answer("cc ");

        QByteArray commArgs = arguments;
        QByteArray fileData = data.left(data.size()-1); // отрезать последнюю \r

        QList<QByteArray> argsArr = commArgs.split(' ');
        if(argsArr.size() > 1)
        {
            QString fileName = argsArr.at(0);
            qint8 mode = QString(argsArr.at(1)).toInt();
            qDebug() << "Save parameters" << fileName << mode;
            if(mode == 0)
            {
                QDir presetDir(dirPath);
                presetDir.setNameFilters({"*.wav"});

                QFileInfoList wavsInDir = presetDir.entryInfoList();
                if(!wavsInDir.empty())
                {
                    foreach(QFileInfo curFileInfo, wavsInDir)
                    {
                        QFile wavFile(curFileInfo.absoluteFilePath());
                        qDebug() << curFileInfo.absoluteFilePath() << "removed" << wavFile.remove();;
                    }
                }

                QFile irFile(dirPath + fileName);
                if(irFile.open(QIODevice::WriteOnly))
                {
                    QByteArray convertedFileData;

                    for(int i=0; i<fileData.size(); i+=2)
                    {
                        QString chStr(QString(fileData.at(i)) + QString(fileData.at(i+1)));
                        convertedFileData.append(chStr.toInt(nullptr, 16));
                    }

                    irFile.write(convertedFileData);
                    irFile.close();
                }
            }
            answer += fileName.toUtf8() + " " + QString().setNum(mode).toUtf8();
        }

        answer += "\rccEND\n";
        emit answerReady(answer);
    }
}

void MockCP16Legacy::lccCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady("lcc\r\nEND\n");
}

void MockCP16Legacy::escAckCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady("esc\r\n");
}

void MockCP16Legacy::formatMemoryCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QDir homeDir(m_basePath);
    homeDir.removeRecursively();
    initFolders();

    emit answerReady("fsf\rFR_OK\n");
}

void MockCP16Legacy::setParamsHandler(QString commStr, quint8 *commPtr)
{
    paramsMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP16Legacy::parametersCommHandler, this, _1, _2, _3));
}

void MockCP16Legacy::setEqHandler(QString commStr, quint8* commPtr)
{
    paramsMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP16Legacy::eqParametersCommHandler, this, _1, _2, _3));
}

void MockCP16Legacy::parametersCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8* paramPtr = paramsMap.value(command);

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        *paramPtr = correctedArgs.toInt(nullptr, 16);
    }
    emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r\n");
}

void MockCP16Legacy::eqParametersCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8* paramPtr = nullptr;

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        QList<QByteArray> argsList = correctedArgs.split(' ');
        paramPtr = paramsMap.value(command);
        paramPtr += QString(argsList.at(0)).toInt(); // array address
        if(argsList.size() > 1) *paramPtr = QString(argsList.at(1)).toInt(nullptr, 16);

        emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r"); // в прошивках до 1.04.04 включительно нет \n в конце команд eq(и ряде других)
    }
}

void MockCP16Legacy::startFwUpdateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    fwUpdateMode = true;
    fwChunkSize = QString(arguments).toInt();

    fwPart = m_parser.getBufferAndFlush();

    qDebug() << fwPart;

    fwFile.setFileName(m_basePath + "/firmware.bin");
    if(fwFile.open(QIODevice::WriteOnly))
    {
        fwFile.write(fwPart);
    }

    fwPart.clear();

    emit answerReady("fwu\rCHUNK_MAX_SIZE_4096\n");
    emit answerReady("REQUEST_CHUNK_SIZE\n");
}
