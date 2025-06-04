#ifndef POWERAMP_H
#define POWERAMP_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "presetlegacy.h"

#include "abstractmodule.h"
#include "controlvalue.h"

class PowerAmp : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* presence READ presence NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* slave READ slave NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* ampType READ ampType NOTIFY dataChanged FINAL)

public:
    enum PaType{
        Classic = 0,
        FX
    };

    explicit PowerAmp(AbstractDevice *owner, PaType paType = PaType::Classic);

    void setValues(const preset_data_cplegacy_t& paData) override;
    void setValues(const preset_data_cpmodern_t& paData) override;
    void setValues(const preset_data_fx_t &paData) override;

    ControlValue* volume() {return m_volume;};
    ControlValue* presence() {return m_presence;};
    ControlValue* slave() {return m_slave;};
    ControlValue* ampType() {return m_ampType;};


private:
    ControlValue *m_volume = nullptr;
    ControlValue *m_presence = nullptr;
    ControlValue *m_slave = nullptr;

    ControlValue *m_ampType = nullptr;

private slots:
    void slPaEnabledChanged();
};
// Q_DECLARE_METATYPE(PowerAmp)

#endif // POWERAMP_H
