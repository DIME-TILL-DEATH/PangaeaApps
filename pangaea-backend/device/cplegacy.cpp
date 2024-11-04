#include "cplegacy.h"

#include <QtQuick/QQuickView>

#include "core.h"

#include "poweramp.h"
#include "eqresponse.h"


CPLegacy::CPLegacy(Core *parent)
    : AbstractDevice{parent}
{
    m_deviceClass = DeviceClass::CP_LEGACY;

    m_parser.addCommandHandler("gb", std::bind(&CPLegacy::bankPresetCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("gm", std::bind(&CPLegacy::outputModeCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("gs", std::bind(&CPLegacy::getStateCommHandler, this, std::placeholders::_1));
}

void CPLegacy::parseAnswers(QByteArray &baAnswer)
{
    m_parser.parseNewData(baAnswer);
}

void CPLegacy::initDevice()
{
    // TODO auto creation in main thread. QObject?
    PA = new PowerAmp(this);
    MV = new PresetVolume(this);

    PA->moveToThread(this->thread());
    MV->moveToThread(this->thread());

    qDebug() << "Device thread:" << this->thread();
    qDebug() << "PA thread: " << PA->thread();
    qDebug() << "model thread: " << m_modulesListModel.thread();

    m_modulesListModel.insertModule(PA, 0);
    m_modulesListModel.insertModule(PA, 1);
    // m_modulesListModel.insertModule(new EqResponse(this), 2);

    qmlRegisterSingletonInstance("CppObjects", 1, 0, "ModulesListModel", &m_modulesListModel);

    emit sgDeviceInstanciated(DeviceType::LEGACY_DEVICES);

    emit sgPushCommandToQueue("amtver");

    pushReadPresetCommands();

    // emit sgPushCommandToQueue("rns");
    emit sgPushCommandToQueue("gm");

    emit sgProcessCommands();
}

void CPLegacy::pushReadPresetCommands()
{
    emit sgPushCommandToQueue("gb");
    emit sgPushCommandToQueue("rn");
    emit sgPushCommandToQueue("gs");
}

void CPLegacy::sendCommandToCP(const QByteArray &command)
{
    emit sgPushCommandToQueue(command);
    emit sgProcessCommands();
}

void CPLegacy::saveChanges()
{
    // qDebug()<<__FUNCTION__;

    // if(presetManager.currentState() == PresetState::Compare)
    // {
    //     comparePreset();
    // }

    // pushCommandToQueue("sp");
    // processCommands();
    // // impulse data uploading after answer
    // if(currentPreset.waveData() == IRWorker::flatIr())
    // {
    //     if(controlledDevice.deviceType()==DeviceType::legacyCP16 || controlledDevice.deviceType()==DeviceType::legacyCP16PA)
    //         pushCommandToQueue("pwsd");
    //     else
    //         pushCommandToQueue("dcc");

    //     currentPreset.clearWavData();
    // }
    // else
    // {
    //     uploadImpulseData(currentPreset.waveData(), false, currentPreset.impulseName());
    // }

    // qDebug() << "Current preset bp: " << currentPreset.bankNumber() << ":" << currentPreset.presetNumber() << "impulse name:" << currentPreset.impulseName();

    // currentSavedPreset = currentPreset;
    // emit sgUpdatePreset(currentSavedPreset);
    // isPresetEdited = false;
    // emit sgSetAppParameter(AppParameter::PRESET_MODIFIED, isPresetEdited);
}

void CPLegacy::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if((newPreset == m_preset) && (newBank == m_bank)) return;

    // if(m_deviceParamsModified && !ignoreChanges)
    // {
    //     emit presetNotSaved(newBank, newPreset);
    //     return;
    // }

    // emit sendAppAction(Core::AppAction::CHANGE_PRESET, {newBank, newPreset});
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

//===============================Comm handlers==========================
void CPLegacy::bankPresetCommHandler(const QByteArray &arguments)
{
    m_bank = arguments.left(2).toUInt();
    m_preset  = arguments.right(2).toUInt();
    emit bankPresetChanged();

    // currentPreset.setBankPreset(static_cast<quint8>(bank), static_cast<quint8>(preset));

    qInfo() << "bank:" << m_bank << "preset:" << m_preset;
}

void CPLegacy::outputModeCommHandler(const QByteArray &arguments)
{
    quint8 mode = arguments.toUInt();
    m_outputMode = mode;
    emit outputModeChanged();
}

void CPLegacy::getStateCommHandler(const QByteArray &arguments)
{
    qDebug() << "GS triggeered";

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
        // emit sgUpdatePreset(currentSavedPreset);
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
        // emit sgUpdatePreset(currentPreset);
        break;
    }
    }

    quint8 count=0;
    quint8 nomByte=0;
    QString sss;

    quint8 dataBuffer[256];

    preset_data_legacy_t str;
    memcpy(&str, baPresetData.data(), sizeof(preset_data_legacy_t));


    foreach(QChar val, baPresetData) //quint8
    {
        if((nomByte&1)==0)
        {
            sss.clear();
            sss.append(val);
        }
        else
        {
            sss.append(val);

            // DeviceParameter::Type paramType = static_cast<DeviceParameter::Type>(count);
            // if(DeviceParameter::isSigned(paramType))
            // {
            //     emit sgRecieveDeviceParameter(paramType, (qint8)sss.toInt(nullptr, 16));
            // }
            // else
            // {
            //     emit sgRecieveDeviceParameter(paramType, (qint16)sss.toInt(nullptr, 16));
            // }
            dataBuffer[count] = sss.toInt(nullptr, 16);

            count++;
        }
        nomByte++;
    }

    preset_data_legacy_t legacyData;
    memcpy(&legacyData, dataBuffer, sizeof(preset_data_legacy_t));

    MV->setValue(legacyData.preset_volume);
    PA->setValues(legacyData.amp_on, legacyData.amp_volume, legacyData.presence_vol, legacyData.amp_slave, legacyData.amp_type);

    emit deviceUpdatingValues();

    m_deviceParamsModified = false;
    emit deviceParamsModifiedChanged();
}
