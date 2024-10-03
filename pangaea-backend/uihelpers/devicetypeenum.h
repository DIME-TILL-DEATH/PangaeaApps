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
        CP100,
        CP16,
        CP16PA,
        CP100PA,
        LA3RV,
        LA3PA
    };
    Q_ENUM(Value)
};

typedef DeviceTypeEnum::Value DeviceType;

#endif // DEVICETYPEENUM_H
