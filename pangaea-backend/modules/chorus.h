#ifndef CHORUS_H
#define CHORUS_H

#include <QObject>
#include "abstractmodule.h"
#include "controlvalue.h"

class Chorus : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* width READ width NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
public:
    Chorus(AbstractDevice *owner);

    void setValues(const chorus_t& chData);

    ControlValue *mix() const;
    ControlValue *rate() const;
    ControlValue *width() const;
    ControlValue *hpf() const;

signals:
    void dataChanged();
private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_width = nullptr;
    ControlValue *m_hpf = nullptr;
};

#endif // CHORUS_H
