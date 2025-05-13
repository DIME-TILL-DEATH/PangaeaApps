#include "cp100fx.h"

#include "core.h"
#include "eqband.h"

Cp100fx::Cp100fx(Core *parent)
    : AbstractDevice{parent}
{

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
    PR = new Preamp(this);
    PA = new PowerAmp(this, PowerAmp::FX);
    IR = new DualCabSim(this);
    EQ = new EqParametric(this, EqParametric::EqMode::Modern, 0);
    DL = new Delay(this, Delay::FX);
    PH = new Phaser(this, Phaser::FX);
    FL = new Flanger(this);
    CH = new Chorus(this, Chorus::FX);
    ER = new EarlyReflections(this);
    RV = new Reverb(this);
    TR = new Tremolo(this);

    m_moduleList.append(RF);
    m_moduleList.append(NG);
    m_moduleList.append(CM);
    m_moduleList.append(PR);
    m_moduleList.append(PA);
    m_moduleList.append(IR);
    m_moduleList.append(EQ);
    m_moduleList.append(DL);
    m_moduleList.append(PH);
    m_moduleList.append(FL);
    m_moduleList.append(CH);
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
    return recievedCommAnswers;
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
    if(data.size() < sizeof(preset_data_cpmodern_t)*2) return;

    QByteArray baPresetData = data;

    quint8 count=0;
    quint8 nomByte=0;
    QString curByte;

    quint8 dataBuffer[sizeof(preset_data_cpmodern_t)*2];

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
    memcpy(&presetData, dataBuffer, sizeof(preset_data_cpmodern_t));

    MV->setValue(presetData.preset_volume);

    NG->setValues(presetData.switches.gate, presetData.gate);
    CM->setValues(presetData.switches.compressor, presetData.compressor);
    PR->setValues(presetData.switches.preamp, presetData.preamp);
    PA->setValues(presetData.switches.amp, presetData.pa);
    // EQ->setEqData(presetData.eq1);
    // IR->setEnabled(presetData.cab_sim_on);
    // IR->setSendLevel(presetData.ir_send_level);
    TR->setValues(presetData.switches.tremolo, presetData.tremolo, presetData.tremolo_tap, presetData.tremolo_lfo_type);
    CH->setValues(presetData.switches.chorus, presetData.chorus, presetData.hpf_chorus);
    PH->setValues(presetData.switches.phaser, presetData.phaser, presetData.hpf_phaser);
    ER->setValues(presetData.switches.early_reflections, presetData.early_reflections);

    uint16_t time = presetData.delay_time_hi << 8 || presetData.delay_time_lo;
    DL->setValues(presetData.switches.delay, presetData.delay, time,
                  presetData.delay_tap, presetData.delay_tail);
    emit deviceUpdatingValues();

    switch(m_presetManager.currentState())
    {
    case PresetState::Copying:
    {
/*        copiedPreset = actualPreset;
        copiedPreset.presetData = PresetModern::charsToPresetData(baPresetData);
        copiedPreset.irFile.setIrLinkPath("ir_library");*/ // Скопированные пресеты могут ссылаться только на библиотеку
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
