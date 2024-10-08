#include "deviceproperties.h"

DeviceProperties::DeviceProperties(Core *core, QObject *parent)
    : QObject{parent}
{
    connect(this, &DeviceProperties::sendAppAction, core, &Core::slRecieveAppAction);
    connect(core, &Core::sgSetAppParameter, this, &DeviceProperties::slSetAppParameter);
    connect(core, &Core::sgRecieveDeviceParameter, this, &DeviceProperties::slSetUiDeviceParameter);
}

void DeviceProperties::changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges)
{
    if((newPreset == m_preset) && (newBank == m_bank)) return;

    if(m_presetModified && !ignoreChanges)
    {
        emit presetNotSaved(newBank, newPreset);
        return;
    }

    emit sendAppAction(Core::AppAction::CHANGE_PRESET, {newBank, newPreset});
}

void DeviceProperties::saveChanges()
{
    m_presetModified = false;
    emit presetModifiedChanged();
    emit sendAppAction(Core::AppAction::SAVE_CHANGES, {});
}

void DeviceProperties::slSetUiDeviceParameter(DeviceParameter::Type deviceParameterType, qint32 value)
{
    switch(deviceParameterType)
    {
    case DeviceParameter::Type::BANK:
    {
        m_bank = value;
        emit bankChanged();
        break;
    }
    case DeviceParameter::Type::PRESET:
    {
        m_preset = value;
        emit presetChanged();
        break;
    }
    case DeviceParameter::Type::OUTPUT_MODE:
    {
        m_outputMode = value;
        emit outputModeChanged();
        break;
    }
    case DeviceParameter::Type::LA3_CLEAN_PRESET:
    {
        m_la3CleanPreset = value;
        emit la3CleanPresetChanged();
        emit la3ChannelChanged(); // this signal is update tumbler color too
        break;
    }
    case DeviceParameter::Type::LA3_DRIVE_PRESET:
    {
        m_la3DrivePreset = value;
        emit la3DrivePresetChanged();
        emit la3ChannelChanged(); // this signal is update tumbler color too
        break;
    }
    case DeviceParameter::Type::LA3_CURRENT_CHANNEL:
    {
        m_la3Channel = value;
        emit la3ChannelChanged();
        break;
    }
    case DeviceParameter::Type::DEVICE_TYPE:
    {
        currentDevice.setDeviceType((DeviceType)value);

        m_presetsList.clear();
        m_banksList.clear();

        for(int i=0; i<currentDevice.maxBankCount(); i++)
        {
            m_banksList.append(i);
        }

        for(int i=0; i<currentDevice.maxPresetCount(); i++)
        {
            m_presetsList.append(i);
        }

        // switch(currentDevice.deviceType())
        // {
        //     case DeviceType::CP100:
        //     {
        //         m_firmwareName = "CP-100";
        //         for(int i=0; i<10; i++)
        //         {
        //             m_banksList.append(i);
        //             m_presetsList.append(i);
        //         }
        //         break;
        //     }
        //     case DeviceType::CP16:
        //     {
        //         m_firmwareName = "CP-16M Blue";

        //         for(int i=0; i<4; i++)
        //         {
        //             m_banksList.append(i);
        //             m_presetsList.append(i);
        //         }
        //         break;
        //     }
        //     case DeviceType::CP16PA:
        //     {
        //         m_firmwareName = "CP-16M-PA Green";

        //         for(int i=0; i<4; i++)
        //         {
        //             m_banksList.append(i);
        //             m_presetsList.append(i);
        //         }
        //         break;
        //     }
        //     case DeviceType::CP100PA:
        //     {
        //         m_firmwareName = "CP-100PA";
        //         for(int i=0; i<10; i++)
        //         {
        //             m_banksList.append(i);
        //             m_presetsList.append(i);
        //         }
        //         break;
        //     }
        //     case DeviceType::LA3RV:
        //     {
        //         m_firmwareName = "LA3RV";

        //         for(int i=0; i<16; i++)
        //         {
        //             m_presetsList.append(i);
        //         }
        //         break;
        //     }
        //     case DeviceType::LA3PA:
        //     {
        //         m_firmwareName = "LA3PA";

        //         for(int i=0; i<16; i++)
        //         {
        //             m_presetsList.append(i);
        //         }
        //         break;
        //     }
        //     default: m_firmwareName = "";
        // }

        emit deviceTypeChanged();
        break;
    }

    default: break;
    }
}

void DeviceProperties::slSetAppParameter(Core::AppParameter appParameterType, QVariant content)
{
    switch (appParameterType)
    {
    case Core::AppParameter::PRESET_MODIFIED:
        m_presetModified = content.toBool();
        emit presetModifiedChanged();
        break;

    default:
        break;
    }
}

void DeviceProperties::setBank(quint8 newBank)
{
    if (m_bank == newBank)
        return;

    if(m_presetModified)
    {
        qDebug() << "PresetNotSaved" << __FUNCTION__;
        emit presetNotSaved(newBank, m_preset);
        return;
    }

    m_bank = newBank;
    emit bankChanged();
    emit sendAppAction(Core::AppAction::CHANGE_PRESET, {m_bank, m_preset});
}

void DeviceProperties::setPreset(quint8 newPreset)
{
    if (m_preset == newPreset)
        return;

    if(m_presetModified)
    {
        qDebug() << "PresetNotSaved" << __FUNCTION__;
        emit presetNotSaved(m_bank, newPreset);
        return;
    }

    m_preset = newPreset;
    emit presetChanged();
    emit sendAppAction(Core::AppAction::CHANGE_PRESET, {m_bank, m_preset});
}

bool DeviceProperties::isLa3Mode() const
{
    return (currentDevice.deviceType() == DeviceType::LA3PA) | (currentDevice.deviceType() == DeviceType::LA3RV);
}

quint8 DeviceProperties::la3CleanPreset() const
{
    return (m_la3CleanPreset/10) * 4 + m_la3CleanPreset%10;
}

quint8 DeviceProperties::la3DrivePreset() const
{
    return (m_la3DrivePreset/10) * 4 + m_la3DrivePreset%10;
}

void DeviceProperties::setLa3Mappings(quint8 cleanPreset, quint8 drivePreset)
{
    m_la3CleanPreset = cleanPreset;
    m_la3DrivePreset = drivePreset;

    quint8 recalcedCleanPreset = 10 * quint8(m_la3CleanPreset/4) + m_la3CleanPreset%4;
    quint8 recalcedDrivePreset = 10 * quint8(m_la3DrivePreset/4) + m_la3DrivePreset%4;
    emit la3ChannelChanged();
    emit sendAppAction(Core::AppAction::SET_LA3_MAPPINGS, {recalcedCleanPreset, recalcedDrivePreset});
}
