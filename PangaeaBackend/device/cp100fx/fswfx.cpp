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

    emit paramsChanged();
}

FswMode FswFx::mode() const
{
    return m_mode;
}

void FswFx::setMode(FswMode newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    emit paramsChanged();
}

FswType FswFx::pressType() const
{
    return m_pressType;
}

void FswFx::setPressType(FswType newPressType)
{
    if (m_pressType == newPressType)
        return;
    m_pressType = newPressType;
    emit paramsChanged();
}

FswType FswFx::holdType() const
{
    return m_holdType;
}

void FswFx::setHoldType(FswType newHoldType)
{
    if (m_holdType == newHoldType)
        return;
    m_holdType = newHoldType;
    emit paramsChanged();
}

uint8_t FswFx::controllerPressNum() const
{
    return m_controllerPressNum;
}

void FswFx::setControllerPressNum(uint8_t newControllerNum)
{
    if (m_controllerPressNum == newControllerNum)
        return;
    m_controllerPressNum = newControllerNum;
    emit paramsChanged();
}

uint8_t FswFx::controllerHoldNum() const
{
    return m_controllerHoldNum;
}

void FswFx::setControllerHoldNum(uint8_t newControllerHoldNum)
{
    if (m_controllerHoldNum == newControllerHoldNum)
        return;
    m_controllerHoldNum = newControllerHoldNum;
    emit paramsChanged();
}
