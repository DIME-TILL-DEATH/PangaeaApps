#include "cplegacy.h"

#include <QDir>


#include "core.h"

#include "eqband.h"

CPLegacy::CPLegacy(Core *parent)
    : AbstractDevice{parent}
{
    m_deviceClass = DeviceClass::CP_LEGACY;

    using namespace std::placeholders;
    m_parser.addCommandHandler("rn", std::bind(&CPLegacy::getIrNameCommHandler, this, _1, _2));
    m_parser.addCommandHandler("rns", std::bind(&CPLegacy::getIrListCommHandler, this, _1, _2));

    m_parser.addCommandHandler("gb", std::bind(&CPLegacy::getBankPresetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gm", std::bind(&CPLegacy::getOutputModeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gs", std::bind(&CPLegacy::getStateCommHandler, this, _1, _2));

    m_parser.addCommandHandler("sp", std::bind(&CPLegacy::ackSaveChanges, this, _1, _2));
    m_parser.addCommandHandler("pc", std::bind(&CPLegacy::ackPresetChangeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("esc", std::bind(&CPLegacy::ackEscCommHandler, this, _1, _2));

    m_parser.addCommandHandler("SYNC ERROR", std::bind(&CPLegacy::errorCCCommHandler, this, _1, _2));
}

void CPLegacy::initDevice(DeviceType deviceType)
{
    m_deviceType = deviceType;

    MV = new PresetVolume(this);

    NG = new NoiseGate(this);
    PA = new PowerAmp(this);
    IR = new CabSim(this);
    EQ = new EqParametric(this);

    m_modulesListModel.insertModule(NG, 0);
    m_modulesListModel.insertModule(PA, 1);
    m_modulesListModel.insertModule(IR, 2);
    m_modulesListModel.insertModule(EQ, 3);

    emit modulesListModelChanged();
    emit presetListModelChanged();

    emit sgDeviceInstanciated(DeviceType::LEGACY_DEVICES);  
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
    qDebug() << "Settling preset data: " << preset.rawData();
    QByteArray ba;

    ba.append("gs 1\r");
    ba.append(preset.rawData());

    emit sgPushCommandToQueue(ba);
    emit sgPushCommandToQueue("gs"); // read settled state
    emit sgProcessCommands();
}


void CPLegacy::sendCommandToCP(const QByteArray &command)
{
    emit sgPushCommandToQueue(command);
    emit sgProcessCommands();
}

void CPLegacy::saveChanges()
{
    qDebug()<<__FUNCTION__;

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

    qDebug() << "Current preset bp: " << currentPreset.bankNumber() << ":" << currentPreset.presetNumber() << "impulse name:" << currentPreset.impulseName();

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
        // emit sgSetAppParameter(AppParameter::COMPARE_STATE, true);
        emit sgPushCommandToQueue("gs");
        emit sgPushCommandToQueue("esc");
    }
    else
    {
        m_presetManager.returnToPreviousState();
        setPresetData(currentPreset);
        uploadImpulseData(currentPreset.waveData(), true, currentPreset.impulseName());
        // emit sgSetAppParameter(AppParameter::COMPARE_STATE, false);
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

}

void CPLegacy::exportPreset(QString filePath, QString fileName)
{

}

void CPLegacy::restoreValue(QString name)
{
    //  TODO пока костыль для отмены сохранения пресета!!!!!
    if(name == "bank-preset")
    {
        m_bank = currentPreset.bankNumber();
        m_preset = currentPreset.presetNumber();
        emit bankPresetChanged();
        //emit deviceUpdatingValues(); // так стираются марки модификации
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
        emit sgDeviceError(DeviceError::IrFormatNotSupported, QString().setNum(wavHead.sampleRate)+" Hz/"+
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

//===================================Setters-getters=================
void CPLegacy::setOutputMode(quint8 newOutputMode)
{
    if (m_outputMode == newOutputMode)
        return;
    m_outputMode = newOutputMode;
    emit outputModeChanged();

    sendCommandToCP(QString("gm %1").arg(m_outputMode, 0, 16).toUtf8());
}

//=======================================================================================
//                  ***************Comm handlers************
//=======================================================================================
void CPLegacy::getBankPresetCommHandler(const QString &command, const QByteArray &arguments)
{
    m_bank = arguments.left(2).toUInt();
    m_preset  = arguments.right(2).toUInt();
    emit bankPresetChanged();

    currentPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
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

    eq_t eqData;
    for(int i=0; i<5; i++)
    {
        eqData.band_type[i] = static_cast<quint8>(EqBand::PEAKING);
        eqData.band_vol[i] = legacyData.eq_band_vol[i];
        eqData.freq[i] = legacyData.eq_freq[i];
        eqData.Q[i] = legacyData.eq_Q[i];
    }
    EQ->setBandsData(eqData);
    IR->setEnabled(legacyData.cab_on);
    PA->setValues(legacyData.amp_on, legacyData.amp_volume, legacyData.presence_vol, legacyData.amp_slave, legacyData.amp_type);


    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        // copy preview wave data
        if(currentPreset.wavSize() != 0)
            copiedPreset = currentPreset;

        copiedPreset.setRawData(baPresetData);
        m_presetManager.returnToPreviousState();
        qDebug() << "copied preset recieve raw data" << copiedPreset.rawData();
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

        // emit sgSetUIText("preset_exported", currentPreset.pathToExport());

        m_presetManager.returnToPreviousState();
        break;
    }

    default:{}
    }

    m_bytesToRecieve=0;

    emit sgEnableTimeoutTimer();
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
    emit sgDeviceError(DeviceError::IrSaveError, currentPreset.impulseName());
}
