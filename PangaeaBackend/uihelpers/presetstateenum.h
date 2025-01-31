#ifndef PRESETSTATEENUM_H
#define PRESETSTATEENUM_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

class PresetStateEnum : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PresetState)
    QML_UNCREATABLE("")
public:
    explicit PresetStateEnum(){};
    enum Value
    {
        Unknown=0,
        Idle,
        // Pasting,
        SetCompare,
        Compare, // вызван оригинальный пресет
        Copying,
        Importing,
        Exporting,
        UploadingIr,
        SavingIr,
        Changing
    };
    Q_ENUM(Value)
};
typedef PresetStateEnum::Value PresetState;
Q_DECLARE_METATYPE(PresetState)

#endif // PRESETSTATEENUM_H
