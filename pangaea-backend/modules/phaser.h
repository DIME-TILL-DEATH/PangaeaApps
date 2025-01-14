#ifndef PHASER_H
#define PHASER_H

#include <QObject>
#include "abstractmodule.h"
#include "controlvalue.h"

class Phaser : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* mix READ mix NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* center READ center NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* width READ width NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* feedback READ feedback NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* stages READ stages NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* hpf READ hpf NOTIFY dataChanged FINAL)
public:
    Phaser(AbstractDevice *owner);

    void setValues(const phaser_t& phData);

    ControlValue *mix() const;
    ControlValue *rate() const;
    ControlValue *center() const;
    ControlValue *width() const;
    ControlValue *feedback() const;
    ControlValue *stages() const;
    ControlValue *hpf() const;

signals:
    void dataChanged();
private:
    ControlValue *m_mix = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_center = nullptr;
    ControlValue *m_width = nullptr;
    ControlValue *m_feedback = nullptr;
    ControlValue *m_stages = nullptr;
    ControlValue *m_hpf = nullptr;
};

#endif // PHASER_H
