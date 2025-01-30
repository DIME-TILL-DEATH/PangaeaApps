#ifndef DEVICEERRORENUM_H
#define DEVICEERRORENUM_H

#include <QObject>

#include <QtQml/qqmlregistration.h>

class DeviceErrorEnum
{
    Q_GADGET
    QML_NAMED_ELEMENT(DeviceErrorEnum)
    QML_UNCREATABLE("")
public:
    explicit DeviceErrorEnum(){};

    enum Value
    {
        UnknownError=0,
        UndefinedCommand,
        IrFormatNotSupported,
        IrSaveError,
        FileNotFound,
        PresetImportUnsuccesfull,
        FimrmwareVersionInsufficient,
        FirmwareFileError,
        FormatMemoryError,
        CopyFileError,
        FileExists
    };
    Q_ENUM(Value)
};

typedef DeviceErrorEnum::Value DeviceErrorType;
Q_DECLARE_METATYPE(DeviceErrorType)

#endif // DEVICEERRORENUM_H
