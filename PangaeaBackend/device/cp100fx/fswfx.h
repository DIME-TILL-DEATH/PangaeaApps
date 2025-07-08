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

    // Q_PROPERTY(uint8_t presetMapNum[] READ presetMapNum WRITE setPresetMapNum NOTIFY paramsChanged FINAL)

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

    FswMode mode() const;
    void setMode(FswMode newMode);

    FswType pressType() const;
    void setPressType(FswType newPressType);

    FswType holdType() const;
    void setHoldType(FswType newHoldType);

    uint8_t controllerPressNum() const;
    void setControllerPressNum(uint8_t newControllerNum);

    // uint8_t presetMapNum() const;
    // void setPresetMapNum(uint8_t (&newPresetMapNum)[]);

    uint8_t controllerHoldNum() const;
    void setControllerHoldNum(uint8_t newControllerHoldNum);

    QStringList ccNames();

signals:

    void paramsChanged();

private:
    quint8 m_num;

    FswMode m_mode;
    FswType m_pressType;
    FswType m_holdType;

    uint8_t m_controllerPressNum;
    uint8_t m_controllerHoldNum;
    uint8_t m_presetMapNum[4];
};

typedef FswFx::FswMode FswMode;
Q_DECLARE_METATYPE(FswMode)

typedef FswFx::FswType FswType;
Q_DECLARE_METATYPE(FswType)

#endif // FSWFX_H
