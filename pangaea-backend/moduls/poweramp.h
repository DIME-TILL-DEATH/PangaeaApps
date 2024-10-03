#ifndef POWERAMP_H
#define POWERAMP_H

#include <QObject>

#include "abstractmodule.h"
#include "controlvalue.h"

class PowerAmp : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* volume READ volume CONSTANT)
    Q_PROPERTY(ControlValue* presence READ presence CONSTANT)
    Q_PROPERTY(ControlValue* slave READ slave CONSTANT)
public:
    explicit PowerAmp(Core *core, QObject *parent = nullptr);

    ControlValue *volume() const {return m_volume;};
    ControlValue *presence() const {return m_presence;};
    ControlValue *slave() const {return m_slave;};

private:
    ControlValue *m_volume = nullptr;
    ControlValue *m_presence = nullptr;
    ControlValue *m_slave = nullptr;

private slots:
    void slPaEnabledChanged();
};

#endif // POWERAMP_H
