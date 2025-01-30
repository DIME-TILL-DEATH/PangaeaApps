#ifndef PRESETVOLUME_H
#define PRESETVOLUME_H

#include "abstractmodule.h"

#include "controlvalue.h"

class PresetVolume : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* presetVolume READ presetVolume NOTIFY dataChanged FINAL)
public:
    PresetVolume(AbstractDevice *owner);

    ControlValue* presetVolume() const {return m_master;};

    void setValue(quint8 val);


private:
    ControlValue *m_master = nullptr;
};

#endif // PRESETVOLUME_H
