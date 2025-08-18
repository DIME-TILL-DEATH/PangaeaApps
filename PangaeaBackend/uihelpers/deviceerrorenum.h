#ifndef DEVICEERRORENUM_H
#define DEVICEERRORENUM_H

#include <QObject>

#include <QtQml/qqmlregistration.h>

class DeviceErrorEnum : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DeviceErrorType)
    QML_UNCREATABLE("")
public:
    explicit DeviceErrorEnum(){};

    enum Value
    {
        UnknownError=0,
        UndefinedCommand,
        NotIrFile,
        IrFormatNotSupported,
        IrSaveError,
        FileNotFound,
        PresetImportUnsuccesfull,
        FimrmwareVersionInsufficient,
        FirmwareFileError,
        FormatMemoryError,
        CopyFileError,
        FileExists,
        PresetNotSaved
    };
    Q_ENUM(Value)
};

typedef DeviceErrorEnum::Value DeviceErrorType;
Q_DECLARE_METATYPE(DeviceErrorType)

#endif // DEVICEERRORENUM_H
