#ifndef PREAMP_H
#define PREAMP_H

#include <QObject>

#include "presetlegacy.h"

#include "abstractmodule.h"
#include "controlvalue.h"

#include <QtQml/qqmlregistration.h>

class Preamp : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

    Q_PROPERTY(ControlValue* low READ low NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* mid READ mid NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* high READ high NOTIFY dataChanged FINAL)
public:
    Preamp(AbstractDevice *owner);

    ControlValue* volume() {return m_volume;};
    ControlValue* low() {return m_low;};
    ControlValue* mid() {return m_mid;};
    ControlValue* high() {return m_high;};

    void setValues(bool enabled, quint8 volume, qint8 low, qint8 mid, qint8 high);
    void setValues(const preamp_data_t& paData);

private:
    ControlValue* m_volume;
    ControlValue* m_low;
    ControlValue* m_mid;
    ControlValue* m_high;
};

#endif // PREAMP_H
