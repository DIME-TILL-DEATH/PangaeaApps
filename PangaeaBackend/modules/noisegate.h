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
    NoiseGate(AbstractDevice *owner, preset_data_cplegacy_t* ngData);
    NoiseGate(AbstractDevice *owner, preset_data_cpmodern_t* ngData);
    NoiseGate(AbstractDevice *owner, modules_data_fx_t* ngData);

    void setValues(const preset_data_cplegacy_t& ngData) override;
    void setValues(const preset_data_cpmodern_t& ngData) override;
    void setValues(const modules_data_fx_t &ngData) override;

    ControlValue *threshold() {return m_threshold;};
    ControlValue *attack() {return m_attack;};
    ControlValue *decay() {return m_decay;};

private:
    ControlValue *m_threshold = nullptr;
    ControlValue *m_attack = nullptr;
    ControlValue *m_decay = nullptr;
};

#endif // NOISEGATE_H
