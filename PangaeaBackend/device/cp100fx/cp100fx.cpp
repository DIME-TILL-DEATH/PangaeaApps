#include "cp100fx.h"

#include <QDir>

#include "core.h"
#include "eqband.h"
#include "irworker.h"
#include "systemsettingsfx.h"
#include "controllerfx.h"

Cp100fx::Cp100fx(Core *parent)
    : AbstractDevice{parent}
{
    using namespace std::placeholders;

    m_parser.addCommandHandler("amtver", std::bind(&Cp100fx::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sys_settings", std::bind(&Cp100fx::sysSettingsCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("state", std::bind(&Cp100fx::stateCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("ls", std::bind(&Cp100fx::lsCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cd", std::bind(&Cp100fx::cdCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("ir", std::bind(&Cp100fx::irCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("upload", std::bind(&Cp100fx::uploadCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("psave", std::bind(&Cp100fx::ackPresetSavedCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pchange", std::bind(&Cp100fx::ackPresetChangeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("plist", std::bind(&Cp100fx::plistCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pnum", std::bind(&Cp100fx::pnumCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pname", std::bind(&Cp100fx::pnameCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pcomment", std::bind(&Cp100fx::pcommentCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("cntrls", std::bind(&Cp100fx::cntrlsCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrl_pc", std::bind(&Cp100fx::cntrlPcOutCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrl_set", std::bind(&Cp100fx::cntrlSetCommHandler, this, _1, _2, _3));

    m_fswList.append(&m_fswDown);
    m_fswList.append(&m_fswConfirm);
    m_fswList.append(&m_fswUp);

    m_maxBankCount = 0;
    m_maxPresetCount = 100;

    actualPreset = new PresetFx{this};
    actualPresetFx = dynamic_cast<PresetFx*>(actualPreset);

    savedPreset = new PresetFx{this};
    savedPresetFx = dynamic_cast<PresetFx*>(savedPreset);

    copiedPreset = new PresetFx{this};
    copiedPresetFx = dynamic_cast<PresetFx*>(copiedPreset);

    for(quint8 i=0; i < ControllersCount; i++)
    {
        m_actualControllersList.append(new ControllerFx(&actualPresetFx->controller[i], i, this));
    }
}

Cp100fx::~Cp100fx()
{
    qDeleteAll(m_actualControllersList);
    qDeleteAll(m_presetsList);

    delete(actualPreset);
    delete(savedPreset);
    delete(copiedPreset);
}

QStringList Cp100fx::strPresetNumbers()
{
    QStringList numbers;

    for(quint8 i=1; i < m_maxPresetCount; i++)
    {
        numbers.append(QString::number(i));
    }
    return numbers;
}

void Cp100fx::initDevice(DeviceType deviceType)
{
    m_deviceType = deviceType;

    RF = new ResonanceFilter(this, &actualPresetFx->presetData);
    NG = new NoiseGate(this, &actualPresetFx->presetData);
    CM = new Compressor(this, &actualPresetFx->presetData);
    PR = new Preamp(this, &actualPresetFx->presetData);
    PA = new PowerAmp(this, &actualPresetFx->presetData);
    IR = new DualCabSim(this, &actualPresetFx->presetData);
    EQ = new EqParametric(this, &actualPresetFx->presetData);
    PH = new Phaser(this, &actualPresetFx->presetData);
    FL = new Flanger(this, &actualPresetFx->presetData);
    CH = new Chorus(this, &actualPresetFx->presetData);
    DL = new Delay(this, &actualPresetFx->presetData);
    ER = new EarlyReflections(this, &actualPresetFx->presetData);
    RV = new Reverb(this, &actualPresetFx->presetData);
    TR = new Tremolo(this, &actualPresetFx->presetData);

    connect(EQ, &AbstractModule::positionChanged, this, &Cp100fx::modulesChangedPosition);
    connect(PH, &AbstractModule::positionChanged, this, &Cp100fx::modulesChangedPosition);
    connect(FL, &AbstractModule::positionChanged, this, &Cp100fx::modulesChangedPosition);

    emit presetListModelChanged();
    emit sgDeviceInstanciated();
}

QList<QByteArray> Cp100fx::parseAnswers(QByteArray baAnswer)
{
    QList<QByteArray> recievedCommAnswers;
    recievedCommAnswers += m_parser.parseNewData(baAnswer);

    emit sgCommandsRecieved(recievedCommAnswers);

    return recievedCommAnswers;
}

void Cp100fx::readFullState()
{
    m_presetManager.setCurrentState(PresetState::Changing);

    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("plist");
    emit sgPushCommandToQueue("sys_settings");

    pushReadPresetCommands();

    emit sgPushCommandToQueue("ls");
    emit sgPushCommandToQueue("cd");

    emit sgProcessCommands();
}

void Cp100fx::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("ir info");
    emit sgPushCommandToQueue("pnum");
    emit sgPushCommandToQueue("pname");
    emit sgPushCommandToQueue("pcomment");

    emit sgPushCommandToQueue("state get");
    emit sgPushCommandToQueue("ir 0");
    emit sgPushCommandToQueue("ir 1");

    emit sgPushCommandToQueue("cntrls");
    emit sgPushCommandToQueue("cntrl_pc");
    emit sgPushCommandToQueue("cntrl_set");


    // m_symbolsToRecieve = 27 + 8 + sizeof(preset_data_cpmodern_t) * 2;
}

void Cp100fx::saveChanges()
{
    qInfo()<<__FUNCTION__;

    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    emit sgPushCommandToQueue("psave");
    emit sgProcessCommands();

    *savedPresetFx = *actualPresetFx;
    m_presetListModel.updatePreset(savedPreset);

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
    emit deviceUpdatingValues(); // clear modified marks on control values
}

void Cp100fx::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if(newPreset == m_preset) return;

    m_preset = newPreset;

    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(savedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pchange %2").arg(m_preset, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void Cp100fx::copyPresetTo(quint8 presetNumber, QVariantList selectionMask)
{
    QByteArray command;
    command.append("copy_to ");
    command.append(QByteArray::number(presetNumber, 16));
    command.append("\r");
    foreach(auto selElement, selectionMask)
    {
        command.append(selElement.toByteArray());
    }
    command.append("\n");
    emit sgPushCommandToQueue(command, false);
    emit sgProcessCommands();
}

void Cp100fx::importPreset(QString filePath, QString fileName)
{

}

void Cp100fx::exportPreset(QString filePath, QString fileName)
{

}

void Cp100fx::restoreValue(QString name)
{

}

void Cp100fx::previewIr(QString srcFilePath)
{

}

void Cp100fx::setFirmware(QString fullFilePath)
{

}

void Cp100fx::formatMemory()
{

}

void Cp100fx::selectFsObject(QString name, FileBrowserModel::FsObjectType type, quint8 cabNum)
{
    switch(type)
    {
        case FileBrowserModel::FsObjectType::Dir:
        {
            QByteArray command;
            command.append("cd\r");
            command.append(name.toUtf8() + "\n");
            emit sgPushCommandToQueue(command, false);
            emit sgPushCommandToQueue("ls");
            emit sgProcessCommands();
            break;
        }

        case FileBrowserModel::FsObjectType::File:
        {
            userModifiedModules();

            QByteArray command;
            command.append("ir ");
            command.append(QByteArray::number(cabNum));
            command.append(" set\r");
            command.append(name.toUtf8());
            emit sgPushCommandToQueue(command);
            emit sgProcessCommands();
            break;
        }

        default: break;
    }
}

void Cp100fx::createDir(QString dirName)
{
    QByteArray command;
    command.append("mkdir\r");
    command.append(dirName.toUtf8());
    command.append("\n");
    emit sgPushCommandToQueue(command, false);
    emit sgPushCommandToQueue("ls");
    emit sgProcessCommands();
}

void Cp100fx::startIrUpload(QString srcFilePath, QString dstFilePath, bool trimFile)
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
        qWarning() << "Can't find file :" << srcFilePath;
        emit sgDeviceError(DeviceErrorType::FileNotFound, "", {fileName});
        return;
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

        QByteArray fileData = irWorker.formFileData();
        if(trimFile)
        {
            if(fileData.size() > maxIrSize()) fileData = fileData.left(maxIrSize());
        }

        uploadIrData(fileName, fileData);
        m_presetManager.setCurrentState(PresetState::UploadingIr);
    }
}

void Cp100fx::uploadIrData(const QString& irName, const QByteArray& irData)
{
    emit m_presetManager.currentStateChanged();

    m_rawIrData = irData;
    QByteArray command = QString("upload start\r").toUtf8() + irName.toUtf8() + "\n";

    qint32 symbolsToSend = command.size() + m_rawIrData.size() + 20 * (m_rawIrData.size() / uploadBlockSize + 1); // header: ir part_upload\r*\n = 16
    qint32 symbolsToRecieve = 0;

    emit sgSendWithoutConfirmation(command, symbolsToSend, symbolsToRecieve);
    emit sgProcessCommands();
}

QString Cp100fx::currentPresetName() const
{
    if(m_presetManager.currentState() == PresetState::Compare) return savedPreset->presetName();
    else return actualPreset->presetName();
}

void Cp100fx::setCurrentPresetName(const QString &newCurrentPresetName)
{
    if (actualPreset->presetName() == newCurrentPresetName)
        return;
    if(newCurrentPresetName.size()>15) actualPreset->setPresetName(newCurrentPresetName.left(15));
    else actualPreset->setPresetName(newCurrentPresetName);
    m_presetListModel.updatePreset(actualPreset);

    emit currentPresetNameChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgPushCommandToQueue("pname set\r" + actualPreset->presetName().toUtf8() + "\n", false);
    emit sgProcessCommands();
}

QString Cp100fx::currentPresetComment() const
{
    if(m_presetManager.currentState() == PresetState::Compare) return savedPresetFx->presetComment();
    else return actualPresetFx->presetComment();
}

void Cp100fx::setCurrentPresetComment(const QString &newCurrentPresetComment)
{
    if (actualPresetFx->presetComment() == newCurrentPresetComment)
        return;
    if(newCurrentPresetComment.size()>15) actualPresetFx->setPresetComment(newCurrentPresetComment.left(15));
    else actualPresetFx->setPresetComment(newCurrentPresetComment);
    m_presetListModel.updatePreset(actualPreset);

    emit currentPresetCommentChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgPushCommandToQueue("pcomment set\r" + actualPresetFx->presetComment().toUtf8() + "\n", false);
    emit sgProcessCommands();
}

void Cp100fx::setCntrlPcOut(quint8 newCntrlPcOut)
{
    if (actualPresetFx->cntrlPcOut() == newCntrlPcOut)
        return;
    actualPresetFx->setCntrlPcOut(newCntrlPcOut);
    emit cntrlPcOutChanged();

    emit sgWriteToInterface("cntrl_pc " + QByteArray::number(newCntrlPcOut, 16) + "\r\n");
}

void Cp100fx::setCntrlSet(quint8 newCntrlSet)
{
    if(actualPresetFx->cntrlSet() == newCntrlSet)
        return;
    actualPresetFx->setCntrlSet(newCntrlSet);
    emit cntrlSetChanged();

    emit sgWriteToInterface("cntrl_set " + QByteArray::number(newCntrlSet, 16) + "\r\n");
}

void Cp100fx::setPresetVolumeControl(quint8 newPresetVolumeControl)
{
    if (actualPresetFx->presetData.volume_control == newPresetVolumeControl)
        return;
    actualPresetFx->presetData.volume_control = newPresetVolumeControl;
    emit presetVolumeControlChanged();

    emit sgWriteToInterface("vl_pr_cntrl " + QByteArray::number(newPresetVolumeControl) + "\r\n");
}

void Cp100fx::setModulePositions()
{
    m_moduleList.clear();

    m_moduleList.append(RF);
    m_moduleList.append(NG);
    m_moduleList.append(CM);
    m_moduleList.append(PR);
    m_moduleList.append(PA);
    if(actualPresetFx->presetData.eq_pre_post) m_moduleList.append(EQ);
    if(actualPresetFx->presetData.phaser_pre_post) m_moduleList.append(PH);
    if(actualPresetFx->presetData.flanger_pre_post) m_moduleList.append(FL);
    m_moduleList.append(IR);
    if(!actualPresetFx->presetData.eq_pre_post) m_moduleList.append(EQ);
    if(!actualPresetFx->presetData.phaser_pre_post) m_moduleList.append(PH);
    if(!actualPresetFx->presetData.flanger_pre_post) m_moduleList.append(FL);
    m_moduleList.append(CH);
    m_moduleList.append(DL);
    m_moduleList.append(ER);
    m_moduleList.append(RV);
    m_moduleList.append(TR);

    m_modulesListModel.refreshModel(&m_moduleList);

    emit modulesListModelChanged();
}

void Cp100fx::modulesChangedPosition()
{
    quint8 from, to;

    QObject* senderObj = QObject::sender();
    AbstractModule* moduleSender;
    if(senderObj)
    {
        moduleSender = qobject_cast<AbstractModule*>(senderObj);
    }
    else return;


    switch(moduleSender->moduleType())
    {
    case ModuleTypeEnum::EQ1:
    {
        from = getModulePosition(ModuleType::EQ1);
        if(actualPresetFx->presetData.eq_pre_post)
        {
            to = getModulePosition(ModuleType::IR_STEREO);

            if(actualPresetFx->presetData.flanger_pre_post)
                to = getModulePosition(ModuleType::FL);

            if(actualPresetFx->presetData.phaser_pre_post)
                to = getModulePosition(ModuleType::PH);
        }
        else
        {
            to = getModulePosition(ModuleType::IR_STEREO);
        }
        break;
    }
    case ModuleTypeEnum::PH:
    {
        from = getModulePosition(ModuleType::PH);
        if(actualPresetFx->presetData.phaser_pre_post)
        {
            to = getModulePosition(ModuleType::IR_STEREO);
            if(actualPresetFx->presetData.flanger_pre_post) to = getModulePosition(ModuleType::FL);
        }
        else
        {
            to = getModulePosition(ModuleType::IR_STEREO);
            if(!actualPresetFx->presetData.eq_pre_post) to = getModulePosition(ModuleType::EQ1);
        }
        break;
    }

    case ModuleTypeEnum::FL:
    {
        from = getModulePosition(ModuleType::FL);
        if(actualPresetFx->presetData.flanger_pre_post)
        {
            to = getModulePosition(ModuleType::IR_STEREO);
        }
        else
        {
            to = getModulePosition(ModuleType::IR_STEREO);
            if(!actualPresetFx->presetData.eq_pre_post) to = getModulePosition(ModuleType::EQ1);
            if(!actualPresetFx->presetData.phaser_pre_post) to = getModulePosition(ModuleType::PH);
        }
        break;
    }

    default: return;
    }

    m_modulesListModel.moveModule(from, to);
}

qint8 Cp100fx::getModulePosition(ModuleType moduleType)
{
    for(int i=0; i< m_moduleList.size(); i++)
    {
        if(m_moduleList.at(i)->moduleType() == moduleType)
        {
            return i;
        }
    }
    return -1;
}
//=======================================================================================
//                  ***************Comm handlers************
//=======================================================================================
void Cp100fx::ackPresetChangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_presetManager.returnToPreviousState(); // for correct hardware changing
    m_presetManager.setCurrentState(PresetState::Changing);
    pushReadPresetCommands();
    emit sgProcessCommands();
}

void Cp100fx::ackPresetSavedCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit presetSaved();
}

void Cp100fx::lsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> objList = data.split('|');

    QList<FileBrowserModel::FsObject> fsList;
    foreach(QByteArray fsRecord, objList)
    {
        QList<QByteArray> splitedRecord = fsRecord.split(':');
        if(splitedRecord.size() == 2)
        {
            FileBrowserModel::FsObject fsObject;

            fsObject.type = static_cast<FileBrowserModel::FsObjectType>(splitedRecord.at(0).toInt());
            fsObject.name = splitedRecord.at(1);

            fsList.append(fsObject);
        }
    }

    m_fileBrowser.updateFsObjectsList(fsList);
}

void Cp100fx::cdCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_fileBrowser.setCurrentFolder(data);
}

void Cp100fx::irCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{

    QList<QByteArray> argList = arguments.split(' ');


    if(argList.size() > 1)
    {
        if(argList.at(1) == "error")
        {
            emit sgDeviceError(DeviceErrorType::NotIrFile, data);
            return;
        }
    }

    if(argList.size() > 0)
    {
        quint8 cabNum = argList.at(0).toInt();
        if(cabNum == 0) m_ir1Name = data;
        else m_ir2Name = data;

        emit irNamesChanged();
    }
}

void Cp100fx::uploadCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "request_part")
    {
        QByteArray answer;
        if(m_rawIrData.length() > 0)
        {
            QByteArray baTmp = m_rawIrData.left(uploadBlockSize);
            m_rawIrData.remove(0, baTmp.length());

            answer = "upload part " + QString().setNum(baTmp.length()).toUtf8() + "\r";
            answer += baTmp;
            answer += "\n";
            emit sgSendWithoutConfirmation(answer);
        }
        else
        {
            emit impulseUploaded();
            switch(m_presetManager.currentState())
            {
            default:
            {
                m_presetManager.returnToPreviousState();
            }
            }

            emit sgPushCommandToQueue("ls\r\n", false);
        }
        emit sgProcessCommands();
    }

    if(arguments == "error")
    {
        emit sgEnableTimeoutTimer();
        emit sgDeviceError(DeviceErrorType::IrSaveError, data);
        m_presetManager.returnToPreviousState();
    }
}

void Cp100fx::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString firmwareVersion = data;

    m_firmwareName += "CP100FX v." + firmwareVersion;
    emit firmwareNameChanged();

    qInfo() << __FUNCTION__ << firmwareVersion;
}

void Cp100fx::plistCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString fullList = data;

    QStringList separatedList = fullList.split("\r");

    m_presetsList.clear();


    QStringList::const_iterator it = separatedList.constBegin();
    while (it != separatedList.constEnd())
    {
        PresetFx* currentListPreset = new PresetFx(this);

        QStringList separatedArgs = (*it).split("|");

        QString presetName, presetComment, ir1Name, ir2Name;
        quint8 strPnum;

        if(separatedArgs.size()<6)
        {
            qWarning() << "incorrect list record format: " << *it;
            delete(currentListPreset);
            goto ending;
        }

        strPnum = separatedArgs.at(0).toInt();
        presetName = separatedArgs.at(1);
        presetComment = separatedArgs.at(2);
        ir1Name = separatedArgs.at(3);
        ir2Name = separatedArgs.at(4);

        currentListPreset->setBankPreset(0, strPnum);
        currentListPreset->setPresetName(presetName);
        currentListPreset->setPresetComment(presetComment);
        currentListPreset->setIr1Name(ir1Name);
        currentListPreset->setIr2Name(ir2Name);

        currentListPreset->setActiveModules(separatedArgs.at(5).toUtf8());

        m_presetsList.append(currentListPreset);
    ending:
        it++;
    }

    m_presetListModel.refreshModel(&m_presetsList);
}

void Cp100fx::pnumCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    bool ok;
    m_preset = data.toInt(&ok, 16);
    actualPreset->setBankPreset(0, m_preset);
    m_presetListModel.updatePreset(actualPreset);

    emit bankPresetChanged();
}

void Cp100fx::pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    switch(m_presetManager.currentState())
    {
    case PresetState::Compare:
    {
        break;
    }
    default:
    {
        actualPreset->setPresetName(data);
        emit currentPresetNameChanged();
    }
    }
}

void Cp100fx::pcommentCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    switch(m_presetManager.currentState())
    {
    case PresetState::Compare:
    {
        break;
    }
    default:
    {
        actualPresetFx->setPresetComment(data);
        emit currentPresetCommentChanged();
    }
    }
}

void Cp100fx::sysSettingsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(data.size() < sizeof(TSystemSettingsFx)*2) return;

    QByteArray baPresetData = data;

    quint16 count=0;
    quint16 nomByte=0;
    QString curByte;

    quint8 dataBuffer[sizeof(TSystemSettingsFx)*2];

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

    TSystemSettingsFx sysSettings;
    memcpy(&sysSettings, dataBuffer, sizeof(TSystemSettingsFx));

    m_systemSettings.setSettings(sysSettings);

    m_fswDown.setData(sysSettings);
    m_fswConfirm.setData(sysSettings);
    m_fswUp.setData(sysSettings);

    m_attenuatorVolume.setValue(sysSettings.attenuator);
    m_masterVolume.setValue(sysSettings.masterVolume);
    m_phonesVolume.setValue(sysSettings.phonesVolume);

    m_masterEq.setValues(sysSettings);
    emit systemSettingsChanged();
}

void Cp100fx::getPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{

}

void Cp100fx::stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(data.size() < sizeof(modules_data_fx_t)*2) return;

    QByteArray baPresetData = data;

    quint16 count=0;
    quint16 nomByte=0;
    QString curByte;

    quint8 dataBuffer[sizeof(modules_data_fx_t)*2];

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

    modules_data_fx_t presetData;
    memcpy(&presetData, dataBuffer, sizeof(modules_data_fx_t));

    emit deviceUpdatingValues();

    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        *copiedPreset = *actualPreset;
        copiedPresetFx->setPresetData(PresetFx::charsToPresetData(baPresetData));
        // copiedPreset.irFile.setIrLinkPath("ir_library"); // Скопированные пресеты могут ссылаться только на библиотеку
        m_presetManager.returnToPreviousState();
        emit presetCopied();
        break;
    }

    case PresetState::Changing:
    {
        m_symbolsToRecieve = 0;
        m_deviceParamsModified = false;
        emit deviceParamsModifiedChanged();

        m_presetVolume.setValue(presetData.preset_volume);
        emit presetVolumeControlChanged();

        actualPresetFx->setPresetData(PresetFx::charsToPresetData(baPresetData));
        *savedPresetFx = *actualPresetFx;
        m_presetListModel.updatePreset(savedPreset);

        setModulePositions();
        foreach(AbstractModule* module, m_moduleList)
        {
            module->setValues(presetData);
        }

        m_presetManager.returnToPreviousState();
        emit presetSwitched();
        break;
    }

    case PresetState::SetCompare:
    {
        actualPresetFx->setPresetData(PresetFx::charsToPresetData(baPresetData));
        // setPresetData(savedPreset);

        emit sgPushCommandToQueue("state get");
        m_presetManager.returnToPreviousState();
        m_presetManager.setCurrentState(PresetState::Compare);
        // emit currentPresetNameChanged(); // Меняется только отображаемое имя. В устройство писать не обязательно
        emit sgProcessCommands();
        break;
    }

    case PresetState::Importing:
    {
        m_presetManager.returnToPreviousState();
        break;
    }

    case PresetState::Compare:
    {
        // configModules(savedPreset);
        m_presetListModel.updatePreset(savedPreset);
        // Необходимая заглушка. Не удалять
        break;
    }

    default:
    {
        m_presetVolume.setValue(presetData.preset_volume);
        emit presetVolumeControlChanged();
        actualPresetFx->setPresetData(PresetFx::charsToPresetData(baPresetData));

        setModulePositions();
        foreach(AbstractModule* module, m_moduleList)
        {
            module->setValues(presetData);
        }
        break;
    }
    }

    m_presetListModel.updatePreset(actualPreset);
}

void Cp100fx::cntrlsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(data.size() < sizeof(controller_fx_t) * ControllersCount * 2) return;

    QByteArray baPresetData = data;

    quint16 count=0;
    quint16 nomByte=0;
    QString curByte;

    quint8 dataBuffer[sizeof(controller_fx_t) * ControllersCount * 2];

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

    controller_fx_t cntrlsData[32];
    memcpy(&cntrlsData, dataBuffer, sizeof(controller_fx_t) * ControllersCount);

    for(int i=0; i<32; i++)
    {
        actualPresetFx->controller[i] = cntrlsData[i];
    }

    emit controllersChanged();
}

void Cp100fx::cntrlPcOutCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    bool ok;
    quint8 value = data.toInt(&ok, 16);

    actualPresetFx->setCntrlPcOut(value);
    emit cntrlPcOutChanged();
}

void Cp100fx::cntrlSetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    bool ok;
    quint8 value = data.toInt(&ok, 16);

    actualPresetFx->setCntrlSet(value);
    emit cntrlSetChanged();
}
