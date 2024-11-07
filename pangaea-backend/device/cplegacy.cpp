#include "cplegacy.h"

#include "core.h"

#include "eqband.h"

CPLegacy::CPLegacy(Core *parent)
    : AbstractDevice{parent}
{
    m_deviceClass = DeviceClass::CP_LEGACY;


    using namespace std::placeholders;
    m_parser.addCommandHandler("rn", std::bind(&CPLegacy::getIrNameCommHandler, this, _1, _2));
    m_parser.addCommandHandler("rns", std::bind(&CPLegacy::getIrListCommHandler, this, _1, _2));

    m_parser.addCommandHandler("gb", std::bind(&CPLegacy::bankPresetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gm", std::bind(&CPLegacy::outputModeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gs", std::bind(&CPLegacy::getStateCommHandler, this, _1, _2));

    m_parser.addCommandHandler("sp", std::bind(&CPLegacy::ackSaveChanges, this, _1, _2));
    m_parser.addCommandHandler("pc", std::bind(&CPLegacy::ackPresetChangeCommHandler, this, _1, _2));
    m_parser.addCommandHandler("esc", std::bind(&CPLegacy::ackEscCommHandler, this, _1, _2));
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

void CPLegacy::setPresetData(const Preset &preset)
{
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

    if(presetManager.currentState() == PresetState::Compare)
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
        // uploadImpulseData(currentPreset.waveData(), false, currentPreset.impulseName());
    }

    qDebug() << "Current preset bp: " << currentPreset.bankNumber() << ":" << currentPreset.presetNumber() << "impulse name:" << currentPreset.impulseName();

    currentSavedPreset = currentPreset;
    m_presetListModel.updatePreset(currentSavedPreset);

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
}

void CPLegacy::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if((newPreset == m_preset) && (newBank == m_bank)) return;

    quint8 val = Preset::calcPresetFlatIndex(m_deviceType, newBank, newPreset);


    if(presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(currentSavedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pc %2").arg(val, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void CPLegacy::comparePreset()
{
    if(presetManager.currentState() != PresetState::Compare)
    {
        // emit sgSetAppParameter(AppParameter::COMPARE_STATE, true);
        emit sgPushCommandToQueue("gs");
        emit sgPushCommandToQueue("esc");
    }
    else
    {
        presetManager.returnToPreviousState();
        setPresetData(currentPreset);
        // uploadImpulseData(currentPreset.waveData(), true, currentPreset.impulseName());
        // emit sgSetAppParameter(AppParameter::COMPARE_STATE, false);
    }
    emit sgProcessCommands();
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

PresetVolume *CPLegacy::getMV()
{
    if(MV) return MV;
    else
    {
        qWarning() << "Master volume not instanciated, but qml request it";
        return nullptr;
    }
}

//=======================================================================================
//                  ***************Comm handlers************
//=======================================================================================
void CPLegacy::bankPresetCommHandler(const QString &command, const QByteArray &arguments)
{
    m_bank = arguments.left(2).toUInt();
    m_preset  = arguments.right(2).toUInt();
    emit bankPresetChanged();

    currentPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
}

void CPLegacy::outputModeCommHandler(const QString &command, const QByteArray &arguments)
{
    quint8 mode = arguments.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

void CPLegacy::getStateCommHandler(const QString &command, const QByteArray &arguments)
{
    QByteArray baPresetData = arguments;

    switch(presetManager.currentState())
    {
    case PresetState::Copying:
    {
        // copy preview wave data
        if(currentPreset.wavSize() != 0)
            copiedPreset = currentPreset;

        copiedPreset.setRawData(baPresetData);
        presetManager.returnToPreviousState();
        break;
    }

    case PresetState::Changing:
    {
        currentPreset.setRawData(baPresetData);
        currentSavedPreset = currentPreset;
        m_presetListModel.updatePreset(currentSavedPreset);
        presetManager.returnToPreviousState();
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

    emit deviceUpdatingValues();

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
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
        impulseName = "empty";

    switch(presetManager.currentState())
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

//-------------------------------------acknowledegs------------------------------------------
void CPLegacy::ackEscCommHandler(const QString &command, const QByteArray &arguments)
{
    presetManager.setCurrentState(PresetState::Compare);
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
    presetManager.setCurrentState(PresetState::Changing);
    pushReadPresetCommands();
}
