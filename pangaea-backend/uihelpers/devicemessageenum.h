#ifndef DEVICEMESSAGEENUM_H
#define DEVICEMESSAGEENUM_H

#include <QObject>

class DeviceMessageEnum
{
    Q_GADGET
public:
    explicit DeviceMessageEnum(){};

    enum Value
    {
        Udefined=0,
        PresetExportFinished,
        PresetNotSaved,
        NewFirmwareAvaliable,
        FimrmwareVersionError,
    };
    Q_ENUM(Value)
};
typedef DeviceMessageEnum::Value DeviceMessageType;
Q_DECLARE_METATYPE(DeviceMessageType)

#endif // DEVICEMESSAGEENUM_H
