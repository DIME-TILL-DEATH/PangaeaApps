#include "fswfx.h"

FswFx::FswFx(quint8 num, QObject *parent)
    : QObject{parent},
    m_num{num}
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
}

void FswFx::setMode(FswMode newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    emit paramsChanged();
}


void FswFx::setPressType(FswType newPressType)
{
    if (m_pressType == newPressType)
        return;
    m_pressType = newPressType;
    emit paramsChanged();
}

void FswFx::setHoldType(FswType newHoldType)
{
    if (m_holdType == newHoldType)
        return;
    m_holdType = newHoldType;
    emit paramsChanged();
}

void FswFx::setControllerPressNum(uint8_t newControllerNum)
{
    if (m_controllerPressNum == newControllerNum)
        return;
    m_controllerPressNum = newControllerNum;
    emit paramsChanged();
}

void FswFx::setControllerHoldNum(uint8_t newControllerHoldNum)
{
    if (m_controllerHoldNum == newControllerHoldNum)
        return;
    m_controllerHoldNum = newControllerHoldNum;
    emit paramsChanged();
}

void FswFx::setPressPreset1(quint8 presetNum)
{
    m_pressPreset[0] = presetNum;
}

void FswFx::setPressPreset2(quint8 presetNum)
{
    m_pressPreset[1] = presetNum;
}

void FswFx::setPressPreset3(quint8 presetNum)
{
    m_pressPreset[2] = presetNum;
}

void FswFx::setPressPreset4(quint8 presetNum)
{
    m_pressPreset[3] = presetNum;
}

void FswFx::setHoldPreset1(quint8 presetNum)
{
    m_holdPreset[0] = presetNum;
}

void FswFx::setHoldPreset2(quint8 presetNum)
{
    m_holdPreset[1] = presetNum;
}

void FswFx::setHoldPreset3(quint8 presetNum)
{
    m_holdPreset[2] = presetNum;
}

void FswFx::setHoldPreset4(quint8 presetNum)
{
    m_holdPreset[3] = presetNum;
}
