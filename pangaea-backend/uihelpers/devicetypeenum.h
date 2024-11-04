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
        UnknownDev=0,
        legacyCP100,
        legacyCP16,
        legacyCP16PA,
        legacyCP100PA,

        //-------------------------------
        LEGACY_DEVICES,
        CP,
        LA3
    };
    Q_ENUM(Value)
};

typedef DeviceTypeEnum::Value DeviceType;

#endif // DEVICETYPEENUM_H
