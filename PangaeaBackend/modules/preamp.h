#ifndef PREAMP_H
#define PREAMP_H

#include <QObject>

#include "presetlegacy.h"

#include "abstractmodule.h"
#include "controlvalue.h"

#include <QtQml/qqmlregistration.h>

class Preamp : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* low READ low NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* mid READ mid NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* high READ high NOTIFY dataChanged FINAL)

    // FX
    Q_PROPERTY(ControlValue* gain READ gain NOTIFY dataChanged FINAL)
public:
    enum PreampType{
        Classic = 0,
        FX
    };

    explicit Preamp(AbstractDevice *owner, PreampType preampType = Classic);

    void setValues(const preset_data_cplegacy_t& prData) override;
    void setValues(const preset_data_cpmodern_t& prData) override;
    void setValues(const preset_data_fx_t &prData) override;

    ControlValue* gain() {return m_gain;};
    ControlValue* volume() {return m_volume;};
    ControlValue* low() {return m_low;};
    ControlValue* mid() {return m_mid;};
    ControlValue* high() {return m_high;};

private:
    ControlValue* m_gain;
    ControlValue* m_volume;
    ControlValue* m_low;
    ControlValue* m_mid;
    ControlValue* m_high;
};

#endif // PREAMP_H
