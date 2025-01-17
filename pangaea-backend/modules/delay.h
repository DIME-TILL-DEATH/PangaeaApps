#ifndef DELAY_H
#define DELAY_H

#include <QObject>
#include "abstractmodule.h"
#include "controlvalue.h"

class Delay : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* time READ time NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* feedback READ feedback NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* lpf READ lpf NOTIFY dataChanged FINAL)
public:
    Delay(AbstractDevice *owner);

    void setValues(const delay_t& dlData);

    ControlValue *mix() const;
    ControlValue *time() const;
    ControlValue *feedback() const;
    ControlValue *hpf() const;
    ControlValue *lpf() const;

// signals:
//     void dataChanged();

private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_time = nullptr;
    ControlValue *m_feedback = nullptr;
    ControlValue *m_hpf = nullptr;
    ControlValue *m_lpf = nullptr;
};

#endif // DELAY_H
