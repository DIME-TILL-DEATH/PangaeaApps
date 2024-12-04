#ifndef UIMESSAGETYPE_H
#define UIMESSAGETYPE_H

#include <QObject>

class UiMessageTypeEnum
{
    Q_GADGET
public:
    explicit UiMessageTypeEnum(){};

    enum Value
    {
        PROPOSE_IR_TRIM
    };
    Q_ENUM(Value)
};

typedef UiMessageTypeEnum::Value UiMessageType;
Q_DECLARE_METATYPE(UiMessageType)

#endif // UIMESSAGETYPE_H
