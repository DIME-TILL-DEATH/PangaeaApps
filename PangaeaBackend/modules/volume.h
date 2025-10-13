#ifndef VOLUME_H
#define VOLUME_H

#include "abstractmodule.h"

#include <QtQml/qqmlregistration.h>

#include "controlvalue.h"

class Volume : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* source READ source NOTIFY dataChanged FINAL)
public:
    enum VolumeType{
        PresetClassic = 0,
        PresetFx,
        MasterFx,
        PhonesFx,
        AttenuatorFx
    };

    Volume(AbstractDevice *owner, VolumeType volumeType = VolumeType::PresetClassic, void* pointer = nullptr);

    ControlValue* volume() const {return m_volume;};
    ControlValue* source() const {return m_source;};

    void setValue(quint8 val, quint8 src = 0);

private:
    ControlValue *m_volume = nullptr;
    ControlValue *m_source = nullptr;
};

#endif // VOLUME_H
