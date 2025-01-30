#ifndef TREMOLO_H
#define TREMOLO_H

#include <QObject>
#include "abstractmodule.h"
#include "controlvalue.h"

class Tremolo : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* depth READ depth NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* form READ form NOTIFY dataChanged FINAL)
public:
    Tremolo(AbstractDevice *owner);

    ControlValue *depth() const;
    ControlValue *rate() const;
    ControlValue *form() const;

    void setValues(const tremolo_t& trData);

private:
    ControlValue *m_depth = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_form = nullptr;
};

#endif // TREMOLO_H
