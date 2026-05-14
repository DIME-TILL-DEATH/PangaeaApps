#ifndef FSWFX_H
#define FSWFX_H

#include <QObject>
#include <QQmlEngine>

#include "abstractdevice.h"
#include "abstractmodule.h"
#include "controlvalue.h"
#include "systemsettingsfx.h"

class FswFx : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mode READ mode NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* pressType READ pressType NOTIFY fswTypeChanged FINAL)
    Q_PROPERTY(ControlValue* holdType READ holdType NOTIFY fswTypeChanged FINAL)
    Q_PROPERTY(ControlValue* controllerPressNum READ controllerPressNum NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* controllerHoldNum READ controllerHoldNum NOTIFY paramsChanged FINAL)

    Q_PROPERTY(ControlValue* pressPreset1 READ pressPreset1 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* pressPreset2 READ pressPreset2 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* pressPreset3 READ pressPreset3 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* pressPreset4 READ pressPreset4 NOTIFY paramsChanged FINAL)

    Q_PROPERTY(ControlValue* holdPreset1 READ holdPreset1 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* holdPreset2 READ holdPreset2 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* holdPreset3 READ holdPreset3 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(ControlValue* holdPreset4 READ holdPreset4 NOTIFY paramsChanged FINAL)

    Q_PROPERTY(QStringList ccNames READ ccNames CONSTANT)
public:
    explicit FswFx(quint8 num, AbstractDevice *owner);

    enum FswMode
    {
        Single,
        Double
    };
    Q_ENUM(FswMode)

    enum FswType
    {
        Default,
        Controller,
        Tuner,
        PresetMap1,
        PresetMap2,
        PresetMap3,
        PresetMap4
    };
    Q_ENUM(FswType)


    void setData(const TSystemSettingsFx& data);

    // ControlValue* геттеры
    ControlValue* mode() const;
    ControlValue* pressType() const;
    ControlValue* holdType() const;
    ControlValue* controllerPressNum() const;
    ControlValue* controllerHoldNum() const;
    ControlValue* pressPreset1() const;
    ControlValue* pressPreset2() const;
    ControlValue* pressPreset3() const;
    ControlValue* pressPreset4() const;
    ControlValue* holdPreset1() const;
    ControlValue* holdPreset2() const;
    ControlValue* holdPreset3() const;
    ControlValue* holdPreset4() const;

    static QStringList ccNames();

signals:
    void paramsChanged();
    void fswTypeChanged();

private:
    quint8 m_num;
    AbstractDevice* m_owner;

    // ControlValue* поля
    ControlValue* m_mode = nullptr;
    ControlValue* m_pressType = nullptr;
    ControlValue* m_holdType = nullptr;
    ControlValue* m_controllerPressNum = nullptr;
    ControlValue* m_controllerHoldNum = nullptr;
    ControlValue* m_pressPreset[4] = {nullptr, nullptr, nullptr, nullptr};
    ControlValue* m_holdPreset[4] = {nullptr, nullptr, nullptr, nullptr};

    // Старые поля (оставлены закомментированными)
    /*
    FswMode m_mode;
    FswType m_pressType;
    FswType m_holdType;
    quint8 m_controllerPressNum;
    quint8 m_controllerHoldNum;
    quint8 m_pressPreset[4];
    quint8 m_holdPreset[4];
    */

    void sendData(const QByteArray& data);
    // Методы-сеттеры ControlValue будут реализованы в cpp
};

typedef FswFx::FswMode FswMode;
Q_DECLARE_METATYPE(FswMode)

typedef FswFx::FswType FswType;
Q_DECLARE_METATYPE(FswType)

#endif // FSWFX_H
