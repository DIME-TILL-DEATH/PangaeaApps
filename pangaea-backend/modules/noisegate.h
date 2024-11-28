#ifndef NOISEGATE_H
#define NOISEGATE_H

#include "abstractmodule.h"

#include "controlvalue.h"
#include "presetlegacy.h"

class NoiseGate : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* threshold READ threshold NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* decay READ decay NOTIFY dataChanged FINAL)

public:
    NoiseGate(AbstractDevice *owner);

    ControlValue *threshold() {return m_threshold;};
    ControlValue *decay() {return m_decay;};

    void setValues(bool enabled, quint8 threshold, quint8 decay);
    void setValues(const gate_data_t& ngData);

private:
    ControlValue *m_threshold = nullptr;
    ControlValue *m_decay = nullptr;
};

#endif // NOISEGATE_H
