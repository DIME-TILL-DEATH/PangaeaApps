#ifndef REVERB_H
#define REVERB_H

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

#include "hardwarefxpreset.h"

#include "abstractmodule.h"
#include "controlvalue.h"

class Reverb : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* time READ time NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* size READ size NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* damping READ damping NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* lpf READ lpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* detune READ detune NOTIFY dataChanged FINAL)

public:
    Reverb(AbstractDevice *owner);

    void setValues(uint8_t enabled, const reverb_fx_t& rvData);

    ControlValue *mix() const {return m_mix;};
    ControlValue *time() const {return m_time;};
    ControlValue *size() const {return m_size;};
    ControlValue *damping() const {return m_damping;};
    ControlValue *hpf() const {return m_hpf;};
    ControlValue *lpf() const {return m_lpf;};
    ControlValue *detune() const {return m_detune;};

private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_time = nullptr;
    ControlValue *m_size = nullptr;
    ControlValue *m_damping = nullptr;
    ControlValue *m_hpf = nullptr;
    ControlValue *m_lpf = nullptr;
    ControlValue *m_detune = nullptr;
};

#endif // REVERB_H
