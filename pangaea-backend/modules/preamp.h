#ifndef PREAMP_H
#define PREAMP_H

#include <QObject>

#include "preset.h"

#include "abstractmodule.h"
#include "controlvalue.h"

class Preamp : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* low READ low NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* mid READ mid NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* high READ high NOTIFY dataChanged FINAL)
public:
    Preamp(AbstractDevice *owner);

    ControlValue* volume() {return m_volume;};
    ControlValue* low() {return m_low;};
    ControlValue* mid() {return m_mid;};
    ControlValue* high() {return m_high;};

    void setValues(bool enabled, quint8 volume, quint8 low, quint8 mid, quint8 high);
    void setValues(const preamp_data_t& paData);

private:
    ControlValue* m_volume;
    ControlValue* m_low;
    ControlValue* m_mid;
    ControlValue* m_high;
};

#endif // PREAMP_H
