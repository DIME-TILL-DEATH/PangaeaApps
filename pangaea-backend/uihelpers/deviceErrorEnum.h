#ifndef DEVICEERRORENUM_H
#define DEVICEERRORENUM_H

#include <QObject>

class DeviceErrorEnum
{
    Q_GADGET
public:
    explicit DeviceErrorEnum(){};

    enum Value
    {
        UnknownError=0,
        UndefinedCommand,
        IrFormatNotSupported,
        IrSaveError,


    };
    Q_ENUM(Value)
};

typedef DeviceErrorEnum::Value DeviceError;

#endif // DEVICEERRORENUM_H
