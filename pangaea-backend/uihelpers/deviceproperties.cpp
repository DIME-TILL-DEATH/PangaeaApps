#include "deviceproperties.h"

DeviceProperties::DeviceProperties(QObject *parent)
    : QObject{parent}
{}

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
    case DeviceParameter::Type::DEVICE_TYPE:
    {
        m_deviceType = (DeviceType)value;
        emit deviceTypeChanged();

        m_presetsList.clear();
        m_banksList.clear();

        switch(m_deviceType)
        {
        case DeviceType::CP100:
        {
            m_firmwareName = "CP-100";
            for(int i=0; i<10; i++)
            {
                m_banksList.append(i);
                m_presetsList.append(i);
            }
            break;
        }
        case DeviceType::CP16:
        {
            m_firmwareName = "CP-16M Blue";
            for(int i=0; i<4; i++)
            {
                m_banksList.append(i);
                m_presetsList.append(i);
            }
            break;
        }
        case DeviceType::CP16PA:
        {
            m_firmwareName = "CP-16M-PA Green";

            for(int i=0; i<4; i++)
            {
                m_banksList.append(i);
                m_presetsList.append(i);
            }
            break;
        }
        case DeviceType::CP100PA:
        {
            m_firmwareName = "CP-100PA";
            for(int i=0; i<10; i++)
            {
                m_banksList.append(i);
                m_presetsList.append(i);
            }
            break;
        }
        case DeviceType::LA3RV:
        {
            m_firmwareName = "LA3RV";

            m_banksList.append("CL0");
            m_banksList.append("CL1");
            m_banksList.append("DS0");
            m_banksList.append("DS1");
            for(int i=0; i<4; i++)
            {
                m_presetsList.append(i);
            }
            break;
        }
        case DeviceType::LA3PA:
        {
            m_firmwareName = "LA3PA";

            m_banksList.append("CL0");
            m_banksList.append("CL1");
            m_banksList.append("DS0");
            m_banksList.append("DS1");
            for(int i=0; i<4; i++)
            {
                m_presetsList.append(i);
            }
            break;
        }
        default: m_firmwareName = "";
        }

        emit isLa3ModeChanged();
        emit firmwareNameChanged();
        emit banksPresetsListChanged();
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
        emit presetNotSaved(m_bank, newPreset);
        return;
    }

    m_preset = newPreset;
    emit presetChanged();
    emit sendAppAction(Core::AppAction::CHANGE_PRESET, {m_bank, m_preset});
}

bool DeviceProperties::isLa3Mode() const
{
    return (m_deviceType == DeviceType::LA3PA) | (m_deviceType == DeviceType::LA3RV);
}
