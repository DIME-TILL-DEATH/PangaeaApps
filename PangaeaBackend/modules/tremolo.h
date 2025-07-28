#ifndef TREMOLO_H
#define TREMOLO_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
#include "controlvalue.h"

class Tremolo : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* depth READ depth NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* rate READ rate NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* form READ form NOTIFY dataChanged FINAL)

    // FX
    Q_PROPERTY(ControlValue* lfoMod READ lfoMod NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* ms READ ms NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* tap READ tap NOTIFY dataChanged FINAL)
public:
    Tremolo(AbstractDevice *owner);

    void setValues(const preset_data_cpmodern_t& trData) override;
    void setValues(const modules_data_fx_t &trData) override;

    ControlValue *depth() const;
    ControlValue *rate() const;
    ControlValue *form() const;
    ControlValue *lfoMod() const;
    ControlValue *ms() const;
    ControlValue *tap() const;

private:
    ControlValue *m_depth = nullptr;
    ControlValue *m_rate = nullptr;
    ControlValue *m_form = nullptr;

    ControlValue *m_lfoMod = nullptr;
    ControlValue *m_ms = nullptr;
    ControlValue *m_tap = nullptr;
};

#endif // TREMOLO_H
