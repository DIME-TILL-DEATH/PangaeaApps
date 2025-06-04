#ifndef PRESENCE_H
#define PRESENCE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"

#include "controlvalue.h"

#include "presetlegacy.h"

class Presence : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* presenceVolume READ presenceVolume NOTIFY dataChanged FINAL)
public:
    Presence(AbstractDevice *owner);

    ControlValue* presenceVolume() {return m_presenceVolume;};

    void setValues(const preset_data_cplegacy_t& presetData) override;

private:
    ControlValue* m_presenceVolume;
};

#endif // PRESENCE_H
