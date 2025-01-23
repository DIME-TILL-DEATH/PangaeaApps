#include "lapreamp.h"

LAPreamp::LAPreamp(Core *parent)
    : CPModern{parent}
{

    using namespace std::placeholders;
    m_parser.addCommandHandler("gb", std::bind(&LAPreamp::getBankPresetLa3CommHandler, this, _1, _2, _3));
}

void LAPreamp::setDeviceType(DeviceType newDeviceType)
{
    m_deviceType = newDeviceType;

    if(m_minimalFirmware != nullptr)
    {
        delete(m_minimalFirmware);
        m_minimalFirmware = nullptr;
    }

    switch(m_deviceType)
    {
    case DeviceType::LA3:
        m_minimalFirmware = new Firmware("2.00.13", newDeviceType, FirmwareType::ApplicationPackage, ":/firmwares/firmwareLA3.ble");
        m_maxBankCount = 0;
        m_maxPresetCount = 16;
        m_firmwareName = "LA3";
        break;
    default:
        qWarning() << Q_FUNC_INFO << "Unknown device type";

        m_minimalFirmware = new Firmware("2.00.01", newDeviceType, FirmwareType::DeviceInternal, "");
        m_maxBankCount = 4;
        m_maxPresetCount = 4;
        m_firmwareName = "Unknown CP/LA device";
        break;
    }
}

void LAPreamp::readFullState()
{
    m_presetManager.setCurrentState(PresetState::Changing);

    emit sgPushCommandToQueue("amtver");
    emit sgPushCommandToQueue("la3map get");
    emit sgPushCommandToQueue("plist");
    emit sgPushCommandToQueue("ls ir_library");
    emit sgPushCommandToQueue("gm");
    pushReadPresetCommands();

    emit sgProcessCommands();
}

void LAPreamp::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if(newPreset == m_preset) return;

    quint8 realBank = newPreset / 4;
    quint8 realPreset = newPreset % 4;

    quint8 val = PresetLegacy::calcPresetFlatIndex(m_deviceType, realBank, realPreset);


    if(m_presetManager.currentState() == PresetState::Compare)
    {
        comparePreset();
    }

    m_presetListModel.updatePreset(&savedPreset); // Обновить актуальный пресет перед переключением

    emit sgPushCommandToQueue(QString("pc %2").arg(val, 2, 16, QChar('0')).toUtf8());
    emit sgProcessCommands();
}

void LAPreamp::getBankPresetLa3CommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_bank = data.left(2).toUInt();
    m_preset  = data.right(2).toUInt();
    QString presetPath = "bank_" + QString().setNum(m_bank) + "/preset_" + QString().setNum(m_preset);

    m_preset = m_bank * 4 + m_preset;
    m_bank = 0;

    actualPreset.setBankPreset(m_bank, m_preset);

    qInfo() << "LA3 preset:" << m_preset;
    emit bankPresetChanged();

    emit sgPushCommandToQueue("ls " + presetPath.toUtf8() + "\r\n", false);
    emit sgProcessCommands();
}
// m_devCommandList.append(DeviceAnswer(new MaskedParser("sm0*END\n", "111X1111"), AnswerType::la3CleanPreset, "la3 clean preset mappings"));
// m_devCommandList.append(DeviceAnswer(new MaskedParser("sm1*END\n", "111X1111"), AnswerType::la3DrivePreset, "la3 drive preset mappings"));
// m_devCommandList.append(DeviceAnswer(new MaskedParser("umx\rEND\n", "11011111"), AnswerType::la3ModeChange, "la3 hardware switch occured"));
// m_devCommandList.append(DeviceAnswer(new MaskedParser("sw1\r*\nEND\n", "1111X11111"), AnswerType::la3CurrentChannel, "la3 current mode"));

/*
            case AnswerType::la3CleanPreset:
            {
                if(parseResult.size()==1)
                {
                    quint8 bankPreset = parseResult.at(0).toUInt();

                    qInfo() << "LA3 clean preset mapping: " << bankPreset;
                    // emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_CLEAN_PRESET, bankPreset);
                }
                break;
            }

            case AnswerType::la3DrivePreset:
            {
                if(parseResult.size()==1)
                {
                    quint8 bankPreset = parseResult.at(0).toUInt();
                    qInfo() << "LA3 drive preset mapping: " << bankPreset;
                    // emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_DRIVE_PRESET, bankPreset);
                }
                break;
            }

            case AnswerType::la3ModeChange:
            {
                qInfo() << recievedCommand.description();

                // pushReadPresetCommands();
                // pushCommandToQueue("rns\n");
                pushCommandToQueue("sw1\n");
                processCommands();
                break;
            }

            case AnswerType::la3CurrentChannel:
            {
                if(parseResult.size()==1)
                {
                    QString strCurrentLa3Mode = QString::fromStdString(parseResult.at(0).toStdString());
                    quint8 currentLa3Mode = 0;
                    if(strCurrentLa3Mode == "high")
                    {
                        currentLa3Mode = 0;
                        qInfo() << "LA3 current mode: " << "clean channel";
                    }
                    else
                    {
                        currentLa3Mode = 1;
                        qInfo() << "LA3 current mode: " << "clean channel";
                    }
                    // emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_CURRENT_CHANNEL, currentLa3Mode);
                }
                break;
            }
*/

// if(controlledDevice.deviceType() == DeviceType::LA3)
// {
//     // request LA3 maps
//     pushCommandToQueue("sm0");
//     pushCommandToQueue("sm1");
//     pushCommandToQueue("sw1");
// }


// void Core::slRecieveAppAction(AppAction appParameterType, QVariantList parameters)
// {


//     case SET_LA3_MAPPINGS:
//     {
//         quint8 cleanPreset = parameters.at(0).toInt();
//         quint8 drivePreset = parameters.at(1).toInt();
//         pushCommandToQueue(QString("sm0 %2").arg(cleanPreset).toUtf8());
//         pushCommandToQueue(QString("sm1 %2").arg(drivePreset).toUtf8());
//         processCommands();
//         break;
//     }

// }


void LAPreamp::setClnPresetMap(quint8 newClnPresetMap)
{
    if (m_clnPresetMap == newClnPresetMap)
        return;
    m_clnPresetMap = newClnPresetMap;
    emit clnPresetMapChanged();
}

void LAPreamp::setDrvPresetMap(quint8 newDrvPresetMap)
{
    if (m_drvPresetMap == newDrvPresetMap)
        return;
    m_drvPresetMap = newDrvPresetMap;
    emit drvPresetMapChanged();
}

void LAPreamp::setLa3Channel(quint8 newLa3Channel)
{
    if (m_la3Channel == newLa3Channel)
        return;
    m_la3Channel = newLa3Channel;
    emit la3ChannelChanged();
}
