#ifndef DEVICECLASSENUM_H
#define DEVICECLASSENUM_H

#include <QObject>

class DeviceClassEnum
{
    Q_GADGET
public:
    explicit DeviceClassEnum(){};

    enum Value
    {
        ABSTRACT = 0,
        CP_LEGACY,
        CP_MODERN
    };
    Q_ENUM(Value)
};

typedef DeviceClassEnum::Value DeviceClass;
Q_DECLARE_METATYPE(DeviceClass)

#endif // DEVICECLASSENUM_H
