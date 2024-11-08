#ifndef PRESETSTATEENUM_H
#define PRESETSTATEENUM_H

#include <QObject>

class PresetStateEnum
{
    Q_GADGET
public:
    explicit PresetStateEnum(){};
    enum Value
    {
        Unknown=0,
        Idle,
        Pasting,
        Compare, // вызван оригинальный пресет
        Copying,
        Exporting,
        UploadingIr,
        SavingIr,
        Changing
    };
    Q_ENUM(Value)
};

typedef PresetStateEnum::Value PresetState;
#endif // PRESETSTATEENUM_H
