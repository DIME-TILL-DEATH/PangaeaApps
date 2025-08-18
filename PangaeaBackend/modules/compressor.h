#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
#include "controlvalue.h"


class Compressor : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* sustain READ sustain NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

    // FX and ultima
    Q_PROPERTY(ControlValue* attack READ attack NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* threshold READ threshold NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* ratio READ ratio NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* knee READ knee NOTIFY dataChanged FINAL)

public:
    Compressor(AbstractDevice *owner, preset_data_cplegacy_t* cmData);
    Compressor(AbstractDevice *owner, preset_data_cpmodern_t* cmData);
    Compressor(AbstractDevice *owner, modules_data_fx_t* cmData);

    void setValues(const preset_data_cplegacy_t& cmData) override;
    void setValues(const preset_data_cpmodern_t &cmData) override;
    void setValues(const modules_data_fx_t &cmData) override;

    ControlValue *sustain() {return m_sustain;};
    ControlValue *volume() {return m_volume;};    
    ControlValue *attack() {return m_attack;};
    ControlValue *threshold() {return m_threshold;};
    ControlValue *ratio() {return m_ratio;};
    ControlValue *knee() {return m_knee;};

private:
    ControlValue *m_sustain = nullptr;
    ControlValue *m_volume = nullptr;

    ControlValue *m_attack = nullptr;
    ControlValue *m_threshold = nullptr;
    ControlValue *m_ratio = nullptr;
    ControlValue *m_knee = nullptr;
};

#endif // COMPRESSOR_H
