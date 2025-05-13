#ifndef CHORUS_H
#define CHORUS_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
#include "controlvalue.h"

class Chorus : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* width READ width NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)

    // FX and ultima params
    Q_PROPERTY(ControlValue* delay READ delay NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* type READ type NOTIFY dataChanged FINAL)
public:
    enum ChorusType{
        Classic = 0,
        FX
    };

    explicit Chorus(AbstractDevice *owner, ChorusType chorusType = ChorusType::Classic);

    void setValues(const chorus_cpmodern_t& chData);
    void setValues(uint8_t enabled, const chorus_fx_t& chData, uint8_t hpfValue);

    ControlValue *mix() const;
    ControlValue *rate() const;
    ControlValue *width() const;
    ControlValue *hpf() const;

    ControlValue *delay() const;
    ControlValue *type() const;

private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_width = nullptr;
    ControlValue *m_hpf = nullptr;

    ControlValue *m_delay = nullptr;
    ControlValue *m_type = nullptr;
};

#endif // CHORUS_H
