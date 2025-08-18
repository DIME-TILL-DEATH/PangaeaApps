#ifndef DELAY_H
#define DELAY_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
#include "controlvalue.h"

class Delay : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* time READ time NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* feedback READ feedback NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* lpf READ lpf NOTIFY dataChanged FINAL)
    // FX
    Q_PROPERTY(ControlValue* mixFirst READ mixFirst NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* mixSecond READ mixSecond NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* panFirst READ panFirst NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* panSecond READ panSecond NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* offset READ offset NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* modulation READ modulation NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* direction READ direction NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* tap READ tap NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* tail READ tail NOTIFY dataChanged FINAL)
public:

    Delay(AbstractDevice *owner, preset_data_cpmodern_t* data);
    Delay(AbstractDevice *owner, modules_data_fx_t* data);

    void setValues(const preset_data_cpmodern_t& dlData) override;
    void setValues(const modules_data_fx_t &dlData) override;

    ControlValue *mix() const {return m_mixFirst;};
    ControlValue *time() const {return m_time;};
    ControlValue *feedback() const {return m_feedback;};
    ControlValue *hpf() const {return m_hpf;};
    ControlValue *lpf() const {return m_lpf;};

    ControlValue *mixFirst() const {return m_mixFirst;};
    ControlValue *mixSecond() const {return m_mixSecond;};
    ControlValue *panFirst() const {return m_panFirst;};
    ControlValue *panSecond() const {return m_panSecond;};
    ControlValue *offset() const {return m_offset;};
    ControlValue *modulation() const {return m_modulation;};
    ControlValue *rate() const {return m_rate;};
    ControlValue *direction() const {return m_direction;};

    ControlValue *tap() const {return m_tap;};
    ControlValue *tail() const {return m_tail;};

private:
    ControlValue *m_mixFirst = nullptr;
    ControlValue *m_mixSecond = nullptr;
    ControlValue *m_panFirst = nullptr;
    ControlValue *m_panSecond = nullptr;
    ControlValue *m_time = nullptr;
    ControlValue *m_feedback = nullptr;
    ControlValue *m_hpf = nullptr;
    ControlValue *m_lpf = nullptr;
    ControlValue *m_offset = nullptr;
    ControlValue *m_modulation = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_direction = nullptr;
    ControlValue *m_tap = nullptr;
    ControlValue *m_tail = nullptr;
};

#endif // DELAY_H
