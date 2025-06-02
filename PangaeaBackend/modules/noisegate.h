#ifndef NOISEGATE_H
#define NOISEGATE_H

#include "abstractmodule.h"

#include <QtQml/qqmlregistration.h>

#include "controlvalue.h"
#include "presetlegacy.h"

class NoiseGate : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* threshold READ threshold NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* decay READ decay NOTIFY dataChanged FINAL)

    // FX and Ultima
    Q_PROPERTY(ControlValue* attack READ attack NOTIFY dataChanged FINAL)

public:
    enum GateType{
        Classic = 0,
        FX
    };

    explicit NoiseGate(AbstractDevice *owner, GateType gateType = Classic);

    ControlValue *threshold() {return m_threshold;};
    ControlValue *attack() {return m_attack;};
    ControlValue *decay() {return m_decay;};

    void setValues(bool enabled, quint8 threshold, quint8 decay);
    void setValues(const gate_cpmodern_t& ngData);
    void setValues(uint8_t enabled, const gate_fx_t& ngData);

private:
    ControlValue *m_threshold = nullptr;
    ControlValue *m_attack = nullptr;
    ControlValue *m_decay = nullptr;
};

#endif // NOISEGATE_H
