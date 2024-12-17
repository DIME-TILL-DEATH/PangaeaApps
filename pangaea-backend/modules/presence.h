#ifndef PRESENCE_H
#define PRESENCE_H

#include <QObject>
#include "abstractmodule.h"

#include "controlvalue.h"

#include "presetlegacy.h"

class Presence : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* presenceVolume READ presenceVolume NOTIFY dataChanged FINAL)
public:
    Presence(AbstractDevice *owner);

    ControlValue* presenceVolume() {return m_presenceVolume;};

    void setValues(bool enabled, quint8 value);

private:
    ControlValue* m_presenceVolume;
};

#endif // PRESENCE_H
