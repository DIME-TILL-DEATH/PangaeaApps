#ifndef FSWFX_H
#define FSWFX_H

#include <QObject>
#include <QQmlEngine>

#include "systemsettingsfx.h"

class FswFx : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(FswMode mode READ mode WRITE setMode NOTIFY paramsChanged FINAL)
    Q_PROPERTY(FswType pressType READ pressType WRITE setPressType NOTIFY paramsChanged FINAL)
    Q_PROPERTY(FswType holdType READ holdType WRITE setHoldType NOTIFY paramsChanged FINAL)
    Q_PROPERTY(uint8_t controllerPressNum READ controllerPressNum WRITE setControllerPressNum NOTIFY paramsChanged FINAL)
    Q_PROPERTY(uint8_t controllerHoldNum READ controllerHoldNum WRITE setControllerHoldNum NOTIFY paramsChanged FINAL)

    Q_PROPERTY(quint8 pressPreset1 READ pressPreset1 WRITE setPressPreset1 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(quint8 pressPreset2 READ pressPreset2 WRITE setPressPreset2 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(quint8 pressPreset3 READ pressPreset3 WRITE setPressPreset3 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(quint8 pressPreset4 READ pressPreset4 WRITE setPressPreset4 NOTIFY paramsChanged FINAL)

    Q_PROPERTY(quint8 holdPreset1 READ holdPreset1 WRITE setHoldPreset1 NOTIFY paramsChanged FINAL)
    Q_PROPERTY(quint8 holdPreset2 READ holdPreset2 WRITE setHoldPreset2  NOTIFY paramsChanged FINAL)
    Q_PROPERTY(quint8 holdPreset3 READ holdPreset3 WRITE setHoldPreset3  NOTIFY paramsChanged FINAL)
    Q_PROPERTY(quint8 holdPreset4 READ holdPreset4 WRITE setHoldPreset4  NOTIFY paramsChanged FINAL)

    Q_PROPERTY(QStringList ccNames READ ccNames CONSTANT)
public:
    explicit FswFx(quint8 num, QObject *parent = nullptr);

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

    FswMode mode() const {return m_mode;};
    void setMode(FswMode newMode);

    FswType pressType() const {return m_pressType;};
    void setPressType(FswType newPressType);

    FswType holdType() const {return m_holdType;};
    void setHoldType(FswType newHoldType);

    uint8_t controllerPressNum() const {return m_controllerPressNum;};
    void setControllerPressNum(uint8_t newControllerNum);

    uint8_t controllerHoldNum() const {return m_controllerHoldNum;};
    void setControllerHoldNum(uint8_t newControllerHoldNum);

    quint8 pressPreset1() const {return m_pressPreset[0];};
    quint8 pressPreset2() const {return m_pressPreset[1];};
    quint8 pressPreset3() const {return m_pressPreset[2];};
    quint8 pressPreset4() const {return m_pressPreset[3];};

    void setPressPreset1(quint8 presetNum);
    void setPressPreset2(quint8 presetNum);
    void setPressPreset3(quint8 presetNum);
    void setPressPreset4(quint8 presetNum);

    quint8 holdPreset1() const {return m_holdPreset[0];};
    quint8 holdPreset2() const {return m_holdPreset[1];};
    quint8 holdPreset3() const {return m_holdPreset[2];};
    quint8 holdPreset4() const {return m_holdPreset[3];};

    void setHoldPreset1(quint8 presetNum);
    void setHoldPreset2(quint8 presetNum);
    void setHoldPreset3(quint8 presetNum);
    void setHoldPreset4(quint8 presetNum);

    static QStringList ccNames();

signals:

    void paramsChanged();

private:
    quint8 m_num;

    FswMode m_mode;
    FswType m_pressType;
    FswType m_holdType;

    quint8 m_controllerPressNum;
    quint8 m_controllerHoldNum;

    quint8 m_pressPreset[4];
    quint8 m_holdPreset[4];
};

typedef FswFx::FswMode FswMode;
Q_DECLARE_METATYPE(FswMode)

typedef FswFx::FswType FswType;
Q_DECLARE_METATYPE(FswType)

#endif // FSWFX_H
