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
    Q_PROPERTY(ControlValue* type READ type NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* time READ time NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* size READ size NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* damping READ damping NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* lpf READ lpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* detune READ detune NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* diffusion READ diffusion NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* preDelay READ preDelay NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* tail READ tail NOTIFY dataChanged FINAL)

public:
    Reverb(AbstractDevice *owner);

    void setValues(const preset_data_fx_t &rvData) override;

    ControlValue *mix() const {return m_mix;};
    ControlValue *type() const {return m_type;};
    ControlValue *time() const {return m_time;};
    ControlValue *size() const {return m_size;};
    ControlValue *damping() const {return m_damping;};
    ControlValue *hpf() const {return m_hpf;};
    ControlValue *lpf() const {return m_lpf;};
    ControlValue *detune() const {return m_detune;};
    ControlValue *diffusion() const {return m_diffusion;};
    ControlValue *preDelay() const {return m_preDelay;};
    ControlValue *tail() const {return m_tail;};

private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_type = nullptr;
    ControlValue *m_time = nullptr;
    ControlValue *m_size = nullptr;
    ControlValue *m_damping = nullptr;
    ControlValue *m_hpf = nullptr;
    ControlValue *m_lpf = nullptr;
    ControlValue *m_detune = nullptr;
    ControlValue *m_diffusion = nullptr;
    ControlValue *m_preDelay = nullptr;
    ControlValue *m_tail = nullptr;
};

#endif // REVERB_H
