#include "systemsettingsfx.h"

SystemSettingsFx::SystemSettingsFx(AbstractDevice *owner)
    : QObject{owner},
    m_owner{owner}
{
    for(int i=0; i<127; i++)
    {
        m_midiPcMap.append(i%98);
    }
}

void SystemSettingsFx::setSettings(TSystemSettingsFx settings)
{
    m_mode = settings.cabSimDisabled;
    m_midiChannel = settings.midiChannel;
    m_cabNumber = settings.cabSimConfig;
    m_exprOn = (settings.expressionType & 0x80) ? 1 : 0;
    m_exprType = settings.expressionType & 0x7F;
    m_exprCC = settings.exprCC;
    m_exprStoreLevel = settings.storeExprLevel;
    m_spdif = settings.spdifOutType;
    m_tempo = settings.tapType;
    m_tunerControl = settings.tunerExternal & 0x80 ? 1 : 0;
    m_tunerCC = settings.tunerExternal & 0x7F;
    m_timeFormat = settings.timeFormat;
    m_swapConf = settings.swapSwitch;
    m_tunerSpeed = settings.tunerSpeed;
    m_fswSpeed = settings.fswSpeed;

    m_midiPcMap.clear();

    for(int i=0; i<127; i++)
    {
        m_midiPcMap.append(settings.midiMap[i]);
    }
    emit settingsChanged();
}

quint8 SystemSettingsFx::mode() const
{
    return m_mode;
}

void SystemSettingsFx::setMode(quint8 newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    emit settingsChanged();


    sendData(QByteArray("sys_cab_mode ") + QByteArray::number(m_mode, 16));
}

quint8 SystemSettingsFx::cabNumber() const
{
    return m_cabNumber;
}

void SystemSettingsFx::setCabNumber(quint8 newCabNumber)
{
    if (m_cabNumber == newCabNumber)
        return;
    m_cabNumber = newCabNumber;
    emit settingsChanged();

    sendData(QByteArray("sys_cab_num ") + QByteArray::number(m_cabNumber, 16));
}

quint8 SystemSettingsFx::midiChannel() const
{
    return m_midiChannel;
}

void SystemSettingsFx::setMidiChannel(quint8 newMidiChannel)
{
    if (m_midiChannel == newMidiChannel)
        return;
    m_midiChannel = newMidiChannel;
    emit settingsChanged();

    sendData(QByteArray("sys_midi_ch ") + QByteArray::number(m_midiChannel, 16));
}


quint8 SystemSettingsFx::exprOn() const
{
    return m_exprOn;
}

void SystemSettingsFx::setExprOn(quint8 newExprOn)
{
    if (m_exprOn == newExprOn)
        return;
    m_exprOn = newExprOn;
    emit settingsChanged();

    sendData(QByteArray("sys_expr_on ") + QByteArray::number(m_exprOn));
}

quint8 SystemSettingsFx::exprType() const
{
    return m_exprType - 1;
}

void SystemSettingsFx::setExprType(quint8 newExprType)
{
    newExprType++;

    if (m_exprType == newExprType)
        return;
    m_exprType = newExprType;
    emit settingsChanged();

    sendData(QByteArray("sys_expr_type ") + QByteArray::number(m_exprType, 16));
}

quint8 SystemSettingsFx::exprCC() const
{
    return m_exprCC;
}

void SystemSettingsFx::setExprCC(quint8 newExprCC)
{
    if (m_exprCC == newExprCC)
        return;
    m_exprCC = newExprCC;
    emit settingsChanged();

    sendData(QByteArray("sys_expr_cc ") + QByteArray::number(m_exprCC, 16));
}

quint8 SystemSettingsFx::exprStoreLevel() const
{
    return m_exprStoreLevel;
}

void SystemSettingsFx::setExprStoreLevel(quint8 newExprStoreLevel)
{
    if (m_exprStoreLevel == newExprStoreLevel)
        return;
    m_exprStoreLevel = newExprStoreLevel;
    emit settingsChanged();

    sendData(QByteArray("sys_expr_slev ") + QByteArray::number(m_exprStoreLevel, 16));
}

quint8 SystemSettingsFx::spdif() const
{
    return m_spdif;
}

void SystemSettingsFx::setSpdif(quint8 newSpdif)
{
    if (m_spdif == newSpdif)
        return;
    m_spdif = newSpdif;
    emit settingsChanged();

    sendData(QByteArray("sys_spdif ") + QByteArray::number(m_spdif, 16));
}

quint8 SystemSettingsFx::tempo() const
{
    return m_tempo;
}

void SystemSettingsFx::setTempo(quint8 newTempo)
{
    if (m_tempo == newTempo)
        return;
    m_tempo = newTempo;
    emit settingsChanged();

    sendData(QByteArray("sys_tempo ") + QByteArray::number(m_tempo, 16));
}

quint8 SystemSettingsFx::tunerControl() const
{
    return m_tunerControl;
}

void SystemSettingsFx::setTunerControl(quint8 newTunerControl)
{
    if (m_tunerControl == newTunerControl)
        return;
    m_tunerControl = newTunerControl;
    emit settingsChanged();

    sendData(QByteArray("sys_tuner_ctrl ") + QByteArray::number(m_tunerControl, 16));
}

quint8 SystemSettingsFx::tunerSpeed() const
{
    return m_tunerSpeed;
}

void SystemSettingsFx::setTunerSpeed(quint8 newTunerSpeed)
{
    if (m_tunerSpeed == newTunerSpeed)
        return;
    m_tunerSpeed = newTunerSpeed;
    emit settingsChanged();

    sendData(QByteArray("sys_tuner_speed ") + QByteArray::number(m_tunerSpeed, 16));
}

quint8 SystemSettingsFx::tunerCC() const
{
    return m_tunerCC - 1;
}

void SystemSettingsFx::setTunerCC(quint8 newTunerCC)
{
    newTunerCC++;

    if (m_tunerCC == newTunerCC)
        return;
    m_tunerCC = newTunerCC;
    emit settingsChanged();

    sendData(QByteArray("sys_tuner_cc ") + QByteArray::number(m_tunerCC, 16));
}


quint8 SystemSettingsFx::timeFormat() const
{
    return m_timeFormat;
}

void SystemSettingsFx::setTimeFormat(quint8 newTimeFormat)
{
    if (m_timeFormat == newTimeFormat)
        return;
    m_timeFormat = newTimeFormat;
    emit settingsChanged();

    sendData(QByteArray("sys_time_format ") + QByteArray::number(m_timeFormat, 16));
}

quint8 SystemSettingsFx::swapConf() const
{
    return m_swapConf;
}

void SystemSettingsFx::setSwapConf(quint8 newSwapConf)
{
    if (m_swapConf == newSwapConf)
        return;
    m_swapConf = newSwapConf;
    emit settingsChanged();

    sendData(QByteArray("sys_swap_conf ") + QByteArray::number(m_swapConf, 16));
}

quint8 SystemSettingsFx::fswSpeed() const
{
    return m_fswSpeed;
}

void SystemSettingsFx::setFswSpeed(quint8 newFswSpeed)
{
    if (m_fswSpeed == newFswSpeed)
        return;
    m_fswSpeed = newFswSpeed;
    emit settingsChanged();

    sendData(QByteArray("sys_fsw_speed ") + QByteArray::number(m_fswSpeed, 16));
}

QList<quint8> SystemSettingsFx::midiPcMap() const
{
    return m_midiPcMap;
}

void SystemSettingsFx::setMidiPcMap(quint8 pcNumber, quint8 presetNumber)
{
    m_midiPcMap.replace(pcNumber, presetNumber);
    emit settingsChanged();

    sendData((QString("midi_map %1 %2").arg(pcNumber, 2, 16).arg(presetNumber, 2, 16)).toUtf8());
}

void SystemSettingsFx::sendData(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data + "\r\n");
}
