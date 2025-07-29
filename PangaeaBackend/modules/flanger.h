#ifndef FLANGER_H
#define FLANGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

#include "hardwarefxpreset.h"

#include "abstractmodule.h"
#include "controlvalue.h"

class Flanger : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* lfo READ lfo NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* width READ width NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* delay READ delay NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* feedback READ feedback NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* position READ position NOTIFY dataChanged FINAL)
public:
    Flanger(AbstractDevice *owner);

    void setValues(const modules_data_fx_t &flData) override;
    void setValuesPointers(modules_data_fx_t* flData);

    ControlValue *mix() const {return m_mix;};
    ControlValue *lfo() const {return m_lfo;};
    ControlValue *rate() const {return m_rate;};
    ControlValue *width() const {return m_width;};
    ControlValue *delay() const {return m_delay;};
    ControlValue *feedback() const {return m_feedback;};
    ControlValue *hpf() const {return m_hpf;};
    ControlValue *position() const {return m_position;};

private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_lfo = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_width = nullptr;
    ControlValue *m_delay = nullptr;
    ControlValue *m_feedback = nullptr;
    ControlValue *m_hpf = nullptr;
    ControlValue *m_position = nullptr;
};

#endif // FLANGER_H
