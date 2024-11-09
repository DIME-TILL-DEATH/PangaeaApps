#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <QObject>
#include "abstractmodule.h"

#include "controlvalue.h"

#include "preset.h"

class LowPassFilter : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* lpf READ lpf NOTIFY dataChanged FINAL)
public:
    LowPassFilter(AbstractDevice *owner);

    ControlValue* lpf() {return m_lpf;};

    void setValues(bool enabled, quint8 value);
    void setValues(const eq_t& eq);

private:
    ControlValue* m_lpf;
};

#endif // LOWPASSFILTER_H
