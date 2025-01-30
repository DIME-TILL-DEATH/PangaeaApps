#ifndef UIMESSAGETYPE_H
#define UIMESSAGETYPE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

class UiMessageTypeEnum
{
    Q_GADGET
    QML_NAMED_ELEMENT(UiMessageType)
    QML_UNCREATABLE("")
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
