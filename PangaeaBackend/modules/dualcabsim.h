#ifndef DUALCABSIM_H
#define DUALCABSIM_H

#include <QQmlEngine>
#include "abstractmodule.h"
#include "controlvalue.h"

class DualCabSim : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* firstCabVolume READ firstCabVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* firstCabPan READ firstCabPan NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* firstCabDelay READ firstCabDelay NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* secondCabVolume READ secondCabVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* secondCabPan READ secondCabPan NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* secondCabDelay READ secondCabDelay NOTIFY dataChanged FINAL)

public:
    DualCabSim(AbstractDevice *owner);

    void setValues(const preset_data_fx_t &csData) override;

    ControlValue* firstCabVolume() {return m_firstCabVolume;};
    ControlValue* firstCabPan() {return m_firstCabPan;};
    ControlValue* firstCabDelay() {return m_firstCabDelay;};
    ControlValue* secondCabVolume() {return m_secondCabVolume;};
    ControlValue* secondCabPan() {return m_secondCabPan;};
    ControlValue* secondCabDelay() {return m_secondCabDelay;};

private:
    ControlValue* m_firstCabVolume;
    ControlValue* m_firstCabPan;
    ControlValue* m_firstCabDelay;
    ControlValue* m_secondCabVolume;
    ControlValue* m_secondCabPan;
    ControlValue* m_secondCabDelay;
};

#endif // DUALCABSIM_H
