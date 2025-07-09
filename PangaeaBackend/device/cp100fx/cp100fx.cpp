#include "cp100fx.h"

#include "core.h"
#include "eqband.h"
#include "systemsettingsfx.h"

Cp100fx::Cp100fx(Core *parent)
    : AbstractDevice{parent}
{
    using namespace std::placeholders;

    m_parser.addCommandHandler("amtver", std::bind(&Cp100fx::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("psave", std::bind(&Cp100fx::ackPresetSavedCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sysset", std::bind(&Cp100fx::sysSettingsCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("state", std::bind(&Cp100fx::stateCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pchange", std::bind(&Cp100fx::ackPresetChangeCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("plist", std::bind(&Cp100fx::plistCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("pnum", std::bind(&Cp100fx::pnumCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pname", std::bind(&Cp100fx::pnameCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pcomment", std::bind(&Cp100fx::pcommentCommHandler, this, _1, _2, _3));

    m_fswList.append(&m_fswDown);
    m_fswList.append(&m_fswConfirm);
    m_fswList.append(&m_fswUp);

    m_maxPresetCount = 100;
}

Cp100fx::~Cp100fx()
{

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

    RF = new ResonanceFilter(this);
    NG = new NoiseGate(this, NoiseGate::FX);
    CM = new Compressor(this, Compressor::FX);
    PR = new Preamp(this, Preamp::FX);
    PA = new PowerAmp(this, PowerAmp::FX);
    IR = new DualCabSim(this);
    EQ = new EqParametric(this, EqParametric::EqMode::Fx, 0);
    DL = new Delay(this, Delay::FX);
    PH = new Phaser(this, Phaser::FX);
    FL = new Flanger(this);
    CH = new Chorus(this, Chorus::FX);
    ER = new EarlyReflections(this, EarlyReflections::FX);
    RV = new Reverb(this);
    TR = new Tremolo(this);

    m_moduleList.append(RF);
    m_moduleList.append(NG);
    m_moduleList.append(CM);
    m_moduleList.append(PR);
    m_moduleList.append(PA);
    m_moduleList.append(IR);
    m_moduleList.append(EQ);
    m_moduleList.append(PH);
    m_moduleList.append(FL);
    m_moduleList.append(CH);
    m_moduleList.append(DL);
    m_moduleList.append(ER);
    m_moduleList.append(RV);
    m_moduleList.append(TR);

    m_modulesListModel.refreshModel(&m_moduleList);

    emit modulesListModelChanged();
    emit presetListModelChanged();
    emit sgDeviceInstanciated();
}

void Cp100fx::readFullState()
{
    m_presetManager.setCurrentState(PresetState::Changing);

    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("plist");
    emit sgPushCommandToQueue("sysset");
    // emit sgPushCommandToQueue("ls ir_library");
    // emit sgPushCommandToQueue("gm");
    pushReadPresetCommands();

    emit sgProcessCommands();
}

void Cp100fx::pushReadPresetCommands()
{
    // emit sgPushCommandToQueue("ir info");
    emit sgPushCommandToQueue("pnum");
    emit sgPushCommandToQueue("pname");
    emit sgPushCommandToQueue("pcomment");
    emit sgPushCommandToQueue("state get");

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

    savedPreset = actualPreset;
    m_presetListModel.updatePreset(&savedPreset);

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

    m_presetListModel.updatePreset(&savedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pchange %2").arg(m_preset, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void Cp100fx::comparePreset()
{

}

void Cp100fx::copyPreset()
{

}

void Cp100fx::pastePreset()
{

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

void Cp100fx::startIrUpload(QString srcFilePath, QString dstFilePath, bool trimFile)
{

}

void Cp100fx::setFirmware(QString fullFilePath)
{

}

void Cp100fx::formatMemory()
{

}

QList<QByteArray> Cp100fx::parseAnswers(QByteArray &baAnswer)
{
    QList<QByteArray> recievedCommAnswers;
    recievedCommAnswers += m_parser.parseNewData(baAnswer);

    return recievedCommAnswers;
}

QString Cp100fx::currentPresetName() const
{
    if(m_presetManager.currentState() == PresetState::Compare) return savedPreset.presetName();
    else return actualPreset.presetName();
}

void Cp100fx::setCurrentPresetName(const QString &newCurrentPresetName)
{
    if (actualPreset.presetName() == newCurrentPresetName)
        return;
    if(newCurrentPresetName.size()>15) actualPreset.setPresetName(newCurrentPresetName.left(15));
    else actualPreset.setPresetName(newCurrentPresetName);
    m_presetListModel.updatePreset(&actualPreset);

    emit currentPresetNameChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgPushCommandToQueue("pname set\r" + actualPreset.presetName().toUtf8() + "\n", false);
    emit sgProcessCommands();
}

QString Cp100fx::currentPresetComment() const
{
    if(m_presetManager.currentState() == PresetState::Compare) return savedPreset.presetComment();
    else return actualPreset.presetComment();
}

void Cp100fx::setCurrentPresetComment(const QString &newCurrentPresetComment)
{
    if (actualPreset.presetComment() == newCurrentPresetComment)
        return;
    if(newCurrentPresetComment.size()>15) actualPreset.setPresetComment(newCurrentPresetComment.left(15));
    else actualPreset.setPresetComment(newCurrentPresetComment);
    m_presetListModel.updatePreset(&actualPreset);

    emit currentPresetCommentChanged();
    m_deviceParamsModified = true;
    emit deviceParamsModifiedChanged();

    emit sgPushCommandToQueue("pcomment set\r" + actualPreset.presetComment().toUtf8() + "\n", false);
    emit sgProcessCommands();
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

        if(separatedArgs.size()<5)
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
    actualPreset.setBankPreset(0, m_preset);

    qInfo() << "preset:" << m_preset;
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
        actualPreset.setPresetName(data);
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
        actualPreset.setPresetComment(data);
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

void Cp100fx::stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(data.size() < sizeof(preset_data_fx_t)*2) return;

    QByteArray baPresetData = data;

    quint16 count=0;
    quint16 nomByte=0;
    QString curByte;

    quint8 dataBuffer[sizeof(preset_data_fx_t)*2];

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

    preset_data_fx_t presetData;
    memcpy(&presetData, dataBuffer, sizeof(preset_data_fx_t));

    // MV->setValue(presetData.preset_volume);

    foreach(AbstractModule* module, m_moduleList)
    {
        module->setValues(presetData);
    }

    emit deviceUpdatingValues();

    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
        copiedPreset = actualPreset;
        copiedPreset.setPresetData(PresetFx::charsToPresetData(baPresetData));
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

        actualPreset.setPresetData(PresetFx::charsToPresetData(baPresetData));
        savedPreset = actualPreset;
        m_presetListModel.updatePreset(&savedPreset);
        m_presetManager.returnToPreviousState();
        emit presetSwitched();
        break;
    }

    case PresetState::SetCompare:
    {
        actualPreset.setPresetData(PresetFx::charsToPresetData(baPresetData));
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
        m_presetListModel.updatePreset(&savedPreset);
        // Необходимая заглушка. Не удалять
        break;
    }

    default:
    {
        actualPreset.setPresetData(PresetFx::charsToPresetData(baPresetData));
        break;
    }
    }
    m_presetListModel.updatePreset(&actualPreset);
}
