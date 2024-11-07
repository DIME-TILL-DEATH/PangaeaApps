#ifndef POWERAMP_H
#define POWERAMP_H

#include <QObject>

#include "preset.h"

#include "abstractmodule.h"
#include "controlvalue.h"

class PowerAmp : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* presence READ presence NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* slave READ slave NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* ampType READ ampType NOTIFY dataChanged FINAL)

public:
    explicit PowerAmp(AbstractDevice *owner);

    ControlValue* volume() {return m_volume;};
    ControlValue* presence() {return m_presence;};
    ControlValue* slave() {return m_slave;};
    ControlValue* ampType() {return m_ampType;};

    void setValues(bool enabled, quint8 volume, quint8 presence, quint8 slave, quint8 ampType);
    void setValues(const pa_data_t& paData);

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
