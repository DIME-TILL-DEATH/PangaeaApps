
#include "fswfx.h"
#include "modules/controlvalue.h"

FswFx::FswFx(quint8 num, AbstractDevice *owner)
    : AbstractModule(owner, ModuleType::SYSTEM, QString("Footswitch %1").arg(num), ""),
    m_num{num},
    m_owner{owner}
{
    // ControlValue* инициализация
    // mode
    m_mode = new ControlValue(this, nullptr, QString("fsw %1 mode").arg(m_num), "Mode");
    // m_mode->setControlSetter([this](qint32 value){
    //     sendData((QString("fsw %1 mode %2").arg(m_num, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
    //     emit paramsChanged();
    // });
    // pressType
    m_pressType = new ControlValue(this, nullptr, QString("fsw %1 ptype").arg(m_num), "Press type:");
    // m_pressType->setControlSetter([this](qint32 value){
    //     sendData((QString("fsw %1 ptype %2").arg(m_num, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
    //     emit paramsChanged();
    //     emit fswTypeChanged();
    // });
    // holdType
    m_holdType = new ControlValue(this, nullptr, QString("fsw %1 htype").arg(m_num), "Hold type:");
    // m_holdType->setControlSetter([this](qint32 value){
    //     sendData((QString("fsw %1 htype %2").arg(m_num, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
    //     emit paramsChanged();
    //     emit fswTypeChanged();
    // });
    // controllerPressNum
    m_controllerPressNum = new ControlValue(this, nullptr, QString("fsw %1 cpressnum").arg(m_num), "Press CC#:");
    // m_controllerPressNum->setControlSetter([this](qint32 value){
    //     sendData((QString("fsw %1 cpressnum %2").arg(m_num, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
    //     emit paramsChanged();
    // });
    // controllerHoldNum
    m_controllerHoldNum = new ControlValue(this, nullptr, QString("fsw %1 choldnum").arg(m_num), "Hold CC#:");
    // m_controllerHoldNum->setControlSetter([this](qint32 value){
    //     sendData((QString("fsw %1 choldnum %2").arg(m_num, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
    //     emit paramsChanged();
    // });
    // pressPreset1-4
    for(int i=0; i<4; ++i) {
        m_pressPreset[i] = new ControlValue(this, nullptr, QString("fsw %1 ppressnum %2").arg(m_num).arg(i), "");
        // m_pressPreset[i]->setControlSetter([this,i](qint32 value){
        //     sendData((QString("fsw %1 ppressnum %2 %3").arg(m_num, 2, 16, QChar('0')).arg(i, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
        //     emit paramsChanged();
        // });
    }
    // holdPreset1-4
    for(int i=0; i<4; ++i) {
        m_holdPreset[i] = new ControlValue(this, nullptr, QString("fsw %1 pholdnum %2").arg(m_num).arg(i), "");
        // m_holdPreset[i]->setControlSetter([this,i](qint32 value){
        //     sendData((QString("fsw %1 pholdnum %2 %3").arg(m_num, 2, 16, QChar('0')).arg(i, 2, 16, QChar('0')).arg(value, 2, 16, QChar('0'))).toUtf8());
        //     emit paramsChanged();
        // });
    }
}

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
    // Старый код:
    /*
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
    */
    // Новый код:
    m_mode->setControlValue(data.fswMode[m_num]);
    m_pressType->setControlValue(data.fswPressType[m_num]);
    m_holdType->setControlValue(data.fswHoldType[m_num]);
    m_controllerPressNum->setControlValue(data.fswControlPressCc[m_num]);
    m_controllerHoldNum->setControlValue(data.fswControlHoldCc[m_num]);
    for(int i=0; i<4; i++) {
        m_pressPreset[i]->setControlValue(data.fswPressPreset[m_num][i]);
        m_holdPreset[i]->setControlValue(data.fswHoldPreset[m_num][i]);
    }
    emit paramsChanged();
    emit fswTypeChanged();
}

// Все старые set* методы закомментированы выше

// ControlValue* геттеры
ControlValue* FswFx::mode() const { return m_mode; }
ControlValue* FswFx::pressType() const { return m_pressType; }
ControlValue* FswFx::holdType() const { return m_holdType; }
ControlValue* FswFx::controllerPressNum() const { return m_controllerPressNum; }
ControlValue* FswFx::controllerHoldNum() const { return m_controllerHoldNum; }
ControlValue* FswFx::pressPreset1() const { return m_pressPreset[0]; }
ControlValue* FswFx::pressPreset2() const { return m_pressPreset[1]; }
ControlValue* FswFx::pressPreset3() const { return m_pressPreset[2]; }
ControlValue* FswFx::pressPreset4() const { return m_pressPreset[3]; }
ControlValue* FswFx::holdPreset1() const { return m_holdPreset[0]; }
ControlValue* FswFx::holdPreset2() const { return m_holdPreset[1]; }
ControlValue* FswFx::holdPreset3() const { return m_holdPreset[2]; }
ControlValue* FswFx::holdPreset4() const { return m_holdPreset[3]; }
