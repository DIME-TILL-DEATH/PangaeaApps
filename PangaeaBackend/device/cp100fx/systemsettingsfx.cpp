#include "systemsettingsfx.h"

SystemSettingsFx::SystemSettingsFx(AbstractDevice *owner)
    : AbstractModule(owner, ModuleType::SYSTEM, "System settings", ""),
    m_owner{owner}
{
    m_mode = new ControlValue(this, nullptr, "sys_cab_mode", "Mode: ");
    m_cabNumber = new ControlValue(this, nullptr, "sys_cab_num", "Cab. num: ");
    m_midiChannel = new ControlValue(this, nullptr, "sys_midi_ch", "MIDI channel: ");

    m_exprOn = new ControlValue(this, nullptr, "sys_expr_on", "Expr. pedal: ");
    m_exprOn->setControlSetter(std::bind(&SystemSettingsFx::exprOnControlSetter, this, std::placeholders::_1));
    m_exprOn->setDisplaySetter(std::bind(&SystemSettingsFx::exprOnDisplaySetter, this, std::placeholders::_1));

    m_exprType = new ControlValue(this, nullptr, "sys_expr_type", "Expr. type: ", "", 1, 4, 0, 3);
    m_exprType->setControlSetter(std::bind(&SystemSettingsFx::exprTypeControlSetter, this, std::placeholders::_1));

    m_exprCC = new ControlValue(this, nullptr, "sys_expr_cc", "Expr. CC#: ");
    m_exprStoreLevel = new ControlValue(this, nullptr, "sys_expr_slev", "Expr. store level: ");
    m_spdif = new ControlValue(this, nullptr, "sys_spdif", "S/PDIF: ");
    m_tempo = new ControlValue(this, nullptr, "sys_tempo", "Tempo: ");

    m_tunerControl = new ControlValue(this, nullptr, "sys_tuner_ctrl", "Tuner control: ");
    m_tunerControl->setControlSetter(std::bind(&SystemSettingsFx::tunerCtrlValueSetter, this, std::placeholders::_1));
    m_tunerControl->setDisplaySetter(std::bind(&SystemSettingsFx::tunerCtrlDisplaySetter, this, std::placeholders::_1));

    m_tunerCC = new ControlValue(this, nullptr, "sys_tuner_cc", "Tuner on CC#: ");
    m_tunerCC->setControlSetter(std::bind(&SystemSettingsFx::tunerCcSetter, this, std::placeholders::_1));

    m_timeFormat = new ControlValue(this, nullptr, "sys_time_format", "Time format: ");
    m_swapConf = new ControlValue(this, nullptr, "sys_swap_conf", "Swap FSW: ");
    m_tunerSpeed = new ControlValue(this, nullptr, "sys_tuner_speed", "Tuner speed: ", "", 0, 127, 0, 127);
    m_fswSpeed = new ControlValue(this, nullptr, "sys_fsw_speed", "FSW speed", "", 0, 127, 0, 127);

    for(int i=0; i<127; i++)
    {
        m_midiPcMap.append(i%98);
    }
}

void SystemSettingsFx::setSettings(TSystemSettingsFx settings)
{
    m_mode->setControlValue(settings.cabSimDisabled);
    m_midiChannel->setControlValue(settings.midiChannel);
    m_cabNumber->setControlValue(settings.cabSimConfig);
    m_exprOn->setControlValue(settings.expressionType);
    m_exprType->setControlValue(settings.expressionType);
    m_exprCC->setControlValue(settings.exprCC);
    m_exprStoreLevel->setControlValue(settings.storeExprLevel);
    m_spdif->setControlValue(settings.spdifOutType);
    m_tempo->setControlValue(settings.tapType);
    m_tunerControl->setControlValue(settings.tunerExternal);
    m_tunerCC->setControlValue(settings.tunerExternal);
    m_timeFormat->setControlValue(settings.timeFormat);
    m_swapConf->setControlValue(settings.swapSwitch);
    m_tunerSpeed->setControlValue(settings.tunerSpeed);
    m_fswSpeed->setControlValue(settings.fswSpeed);

    m_midiPcMap.clear();

    for(int i=0; i<127; i++)
    {
        m_midiPcMap.append(settings.midiMap[i]);
    }
    emit settingsChanged();
}

ControlValue* SystemSettingsFx::mode() const
{
    return m_mode;
}

ControlValue* SystemSettingsFx::cabNumber() const
{
    return m_cabNumber;
}

ControlValue* SystemSettingsFx::midiChannel() const
{
    return m_midiChannel;
}

ControlValue* SystemSettingsFx::exprOn() const
{
    return m_exprOn;
}

ControlValue* SystemSettingsFx::exprType() const
{
    return m_exprType;
}

ControlValue* SystemSettingsFx::exprCC() const
{
    return m_exprCC;
}

ControlValue* SystemSettingsFx::exprStoreLevel() const
{
    return m_exprStoreLevel;
}

ControlValue* SystemSettingsFx::spdif() const
{
    return m_spdif;
}

ControlValue* SystemSettingsFx::tempo() const
{
    return m_tempo;
}

ControlValue* SystemSettingsFx::tunerControl() const
{
    return m_tunerControl;
}

ControlValue* SystemSettingsFx::tunerSpeed() const
{
    return m_tunerSpeed;
}

ControlValue* SystemSettingsFx::tunerCC() const
{
    return m_tunerCC;
}

ControlValue* SystemSettingsFx::timeFormat() const
{
    return m_timeFormat;
}

ControlValue* SystemSettingsFx::swapConf() const
{
    return m_swapConf;
}

ControlValue* SystemSettingsFx::fswSpeed() const
{
    return m_fswSpeed;
}

QList<quint8> SystemSettingsFx::midiPcMap() const
{
    return m_midiPcMap;
}

void SystemSettingsFx::setMidiPcMap(quint8 pcNumber, quint8 presetNumber)
{
    m_midiPcMap.replace(pcNumber, presetNumber);
    emit settingsChanged();

    sendData((QString("midi_map %1 %2").arg(pcNumber, 2, 16, QChar('0')).arg(presetNumber, 2, 16, QChar('0'))).toUtf8());
}

void SystemSettingsFx::sendData(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data + "\r\n");
}

void SystemSettingsFx::exprOnControlSetter(qint32 value)
{
    m_exprOn->modifyDisplayValue((value & 0x80) ? 1 : 0);
    emit m_exprOn->displayValueChanged();
}

void SystemSettingsFx::exprOnDisplaySetter(double value)
{
    m_exprOn->modifyDisplayValue(value);
    emit m_exprOn->displayValueChanged();

    sendData(QByteArray(m_exprOn->commandString().toUtf8() + " " + (value ? "80" : "00")));
}

void SystemSettingsFx::exprTypeControlSetter(qint32 value)
{
    m_exprType->modifyDisplayValue((value & 0x7F) - 1);
    emit m_exprType->displayValueChanged();
}

void SystemSettingsFx::tunerCtrlValueSetter(qint32 value)
{
    m_tunerControl->modifyDisplayValue((value & 0x80) ? 1 : 0);
    emit m_tunerControl->displayValueChanged();

    qDebug() << "Tuner control setter" << value << ((value & 0x80) ? 1 : 0);
}

void SystemSettingsFx::tunerCtrlDisplaySetter(double value)
{
    m_tunerControl->modifyDisplayValue(value);
    emit m_tunerControl->displayValueChanged();

    sendData(QByteArray(m_tunerControl->commandString().toUtf8() + " " + (value ? "80" : "00")));
}

void SystemSettingsFx::tunerCcSetter(qint32 value)
{
    m_tunerCC->modifyDisplayValue((value & 0x7F));
    emit m_tunerCC->displayValueChanged();
}
