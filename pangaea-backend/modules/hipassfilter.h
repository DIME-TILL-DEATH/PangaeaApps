#ifndef HIPASSFILTER_H
#define HIPASSFILTER_H

#include <QObject>
#include "abstractmodule.h"

#include "controlvalue.h"

#include "preset.h"

class HiPassFilter : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
public:
    HiPassFilter(AbstractDevice *owner);

    ControlValue* hpf() {return m_hpf;};

    void setValues(bool enabled, quint8 value);
    void setValues(const eq_t& eq);

private:
    ControlValue* m_hpf;
};

#endif // HIPASSFILTER_H