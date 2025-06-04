#include "cp100fx.h"

#include "core.h"
#include "eqband.h"

Cp100fx::Cp100fx(Core *parent)
    : AbstractDevice{parent}
{
    using namespace std::placeholders;

    m_parser.addCommandHandler("state", std::bind(&Cp100fx::stateCommHandler, this, _1, _2, _3));
}

Cp100fx::~Cp100fx()
{

}

void Cp100fx::updateOutputModeNames()
{

}

quint16 Cp100fx::processingUsed()
{
    return 0;
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
    // emit sgPushCommandToQueue("plist");
    // emit sgPushCommandToQueue("ls ir_library");
    // emit sgPushCommandToQueue("gm");
    pushReadPresetCommands();

    emit sgProcessCommands();
}

void Cp100fx::pushReadPresetCommands()
{
    // emit sgPushCommandToQueue("gb");
    // emit sgPushCommandToQueue("ir info");
    // emit sgPushCommandToQueue("pname get");
    // emit sgPushCommandToQueue("mconfig get");
    emit sgPushCommandToQueue("state get");

    // m_symbolsToRecieve = 27 + 8 + sizeof(preset_data_cpmodern_t) * 2;
}

void Cp100fx::saveChanges()
{

}

void Cp100fx::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{

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

void Cp100fx::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    // QString firmwareVersion = data;

    // m_firmwareName += " v." + firmwareVersion;
    // emit firmwareNameChanged();

    // qInfo() << __FUNCTION__ << firmwareVersion;

    // m_actualFirmware = new Firmware(firmwareVersion, m_deviceType, FirmwareType::DeviceInternal, "device:/internal");

    // qInfo() << "Firmware name: " << m_firmwareName
    //         << ", version control, minimal: " << m_minimalFirmware->firmwareVersion()
    //         << " actual: " << m_actualFirmware->firmwareVersion();

    // if(*m_actualFirmware > *m_minimalFirmware)
    // {
    //     bool isCheckUpdatesEnabled = appSettings->value("check_updates_enable").toBool();

    //     if(isCheckUpdatesEnabled)
    //     {
    //         // emit sgRequestNewestFirmware(deviceFirmware);
    //     }
    // }
    // else
    // {
    //     qWarning() << "firmware insufficient!";
    //     emit sgDeviceError(DeviceErrorType::FimrmwareVersionInsufficient, "", {m_actualFirmware->firmwareVersion(), m_minimalFirmware->firmwareVersion()});
    // }
}

void Cp100fx::getPresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    // m_bank = data.left(2).toUInt();
    // m_preset  = data.right(2).toUInt();

    // actualPreset.setBankPreset(m_bank, m_preset);

    // qInfo() << "bank:" << m_bank << "preset:" << m_preset;
    // emit bankPresetChanged();

    // // TODO when opens IR manager
    // QString presetPath = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);
    // emit sgPushCommandToQueue("ls " + presetPath.toUtf8() + "\r\n", false);
    // emit sgProcessCommands();
}

void Cp100fx::pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    // switch(m_presetManager.currentState())
    // {
    // case PresetState::Compare:
    // {
    //     break;
    // }
    // default:
    // {
    //     actualPreset.setPresetName(data);
    //     emit currentPresetNameChanged();
    // }
    // }
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
        copiedPreset.presetData = PresetFx::charsToPresetData(baPresetData);
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

        actualPreset.presetData = PresetFx::charsToPresetData(baPresetData);
        savedPreset = actualPreset;
        m_presetListModel.updatePreset(&savedPreset);
        m_presetManager.returnToPreviousState();
        emit presetSwitched();
        break;
    }

    case PresetState::SetCompare:
    {
        actualPreset.presetData = PresetFx::charsToPresetData(baPresetData);
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
        actualPreset.presetData = PresetFx::charsToPresetData(baPresetData);
        break;
    }
    }
    m_presetListModel.updatePreset(&actualPreset);
}
