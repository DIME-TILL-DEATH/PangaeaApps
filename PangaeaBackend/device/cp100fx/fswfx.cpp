#include "fswfx.h"

FswFx::FswFx(quint8 num, AbstractDevice *owner)
    : QObject{owner},
    m_num{num},
    m_owner{owner}
{}

QStringList FswFx::ccNames()
{
    QStringList names;

    names.append("Off");

    for(quint8 i=0; i<127; i++)
    {
        names.append(QString::number(i));
    }
    return names;
}

void FswFx::sendData(const QByteArray &data)
{
    if(m_owner) emit m_owner->sgWriteToInterface(data + "\r\n");
}

void FswFx::setData(const TSystemSettingsFx &data)
{
    m_mode = static_cast<FswMode>(data.fswMode[m_num]);
    m_pressType = static_cast<FswType>(data.fswPressType[m_num]);
    m_holdType = static_cast<FswType>(data.fswHoldType[m_num]);

    m_controllerPressNum = data.fswControlPressCc[m_num];
    m_controllerHoldNum = data.fswControlHoldCc[m_num];

    for(int i=0; i<4; i++)
    {
        m_pressPreset[i] = data.fswPressPreset[m_num][i];
        m_holdPreset[i] = data.fswHoldPreset[m_num][i];
    }

    emit paramsChanged();
    emit fswTypeChanged();
}

void FswFx::setMode(FswMode newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    emit paramsChanged();

    sendData((QString("fsw %1 mode %2").arg(m_num, 2, 16).arg(m_mode, 2, 16)).toUtf8());
}


void FswFx::setPressType(FswType newPressType)
{
    if (m_pressType == newPressType)
        return;
    m_pressType = newPressType;
    emit paramsChanged();
    emit fswTypeChanged();

    sendData((QString("fsw %1 ptype %2").arg(m_num, 2, 16).arg(m_pressType, 2, 16)).toUtf8());
}

void FswFx::setHoldType(FswType newHoldType)
{
    if (m_holdType == newHoldType)
        return;
    m_holdType = newHoldType;
    emit paramsChanged();
    emit fswTypeChanged();

    sendData((QString("fsw %1 htype %2").arg(m_num, 2, 16).arg(m_holdType, 2, 16)).toUtf8());
}

void FswFx::setControllerPressNum(uint8_t newControllerNum)
{
    if (m_controllerPressNum == newControllerNum)
        return;
    m_controllerPressNum = newControllerNum;
    emit paramsChanged();

    sendData((QString("fsw %1 cpressnum %2").arg(m_num, 2, 16).arg(m_controllerPressNum, 2, 16)).toUtf8());
}

void FswFx::setControllerHoldNum(uint8_t newControllerHoldNum)
{
    if (m_controllerHoldNum == newControllerHoldNum)
        return;
    m_controllerHoldNum = newControllerHoldNum;
    emit paramsChanged();

    sendData((QString("fsw %1 choldnum %2").arg(m_num, 2, 16).arg(m_controllerHoldNum, 2, 16)).toUtf8());
}

void FswFx::setPressPreset1(quint8 presetNum)
{
    m_pressPreset[0] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 ppressnum 0 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setPressPreset2(quint8 presetNum)
{
    m_pressPreset[1] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 ppressnum 1 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setPressPreset3(quint8 presetNum)
{
    m_pressPreset[2] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 ppressnum 2 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setPressPreset4(quint8 presetNum)
{
    m_pressPreset[3] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 ppressnum 3 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setHoldPreset1(quint8 presetNum)
{
    m_holdPreset[0] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 pholdnum 0 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setHoldPreset2(quint8 presetNum)
{
    m_holdPreset[1] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 pholdnum 1 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setHoldPreset3(quint8 presetNum)
{
    m_holdPreset[2] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 pholdnum 2 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}

void FswFx::setHoldPreset4(quint8 presetNum)
{
    m_holdPreset[3] = presetNum;
    emit paramsChanged();

    sendData((QString("fsw %1 pholdnum 3 %2").arg(m_num, 2, 16).arg(presetNum, 2, 16)).toUtf8());
}
