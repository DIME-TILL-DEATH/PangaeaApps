#ifndef MASTEREQ_H
#define MASTEREQ_H

#include <QObject>
#include <QQmlEngine>
#include "abstractmodule.h"

#include "controlvalue.h"

#include "systemsettingsfx.h"

class MasterEq : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* lowGain READ lowGain NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* midGain READ midGain NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* midFrequency READ midFrequency NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* highGain READ highGain NOTIFY dataChanged FINAL)
public:
    MasterEq(AbstractDevice *owner);
    ~MasterEq();

    void setValues(const TSystemSettingsFx &sysSettings);

    ControlValue *lowGain() const;
    ControlValue *midGain() const;
    ControlValue *midFrequency() const;
    ControlValue *highGain() const;

private:
    ControlValue *m_lowGain = nullptr;
    ControlValue *m_midGain = nullptr;
    ControlValue *m_midFrequency = nullptr;
    ControlValue *m_highGain = nullptr;
};

#endif // MASTEREQ_H
