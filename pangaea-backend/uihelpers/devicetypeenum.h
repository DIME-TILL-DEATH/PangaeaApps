#ifndef DEVICETYPEENUM_H
#define DEVICETYPEENUM_H

#include <QObject>

class DeviceTypeEnum
{
        Q_GADGET
public:
    explicit DeviceTypeEnum(){};
    enum Value
    {
        UNKNOWN_DEVICE=0,
        LEGACY_CP100,
        LEGACY_CP16,
        LEGACY_CP16PA,
        LEGACY_CP100PA,

        LEGACY_DEVICES = 15,
        //-------------------------------
        MODERN_CP,
        LA3
    };
    Q_ENUM(Value)
};

typedef DeviceTypeEnum::Value DeviceType;
Q_DECLARE_METATYPE(DeviceType)

#endif // DEVICETYPEENUM_H
