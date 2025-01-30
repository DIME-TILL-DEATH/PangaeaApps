#ifndef FILTERTYPEENUM_H
#define FILTERTYPEENUM_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

class FilterTypeEnum
{
    Q_GADGET
    QML_NAMED_ELEMENT(FilterType)
    QML_UNCREATABLE("")
public:
    explicit FilterTypeEnum(){};

    enum Value
    {
        PEAKING = 0,
        LOW_SHELF,
        HIGH_SHELF,
        HIGH_CUT,
        LOW_CUT,
        BYPASS
    };
    Q_ENUM(Value)
};

typedef FilterTypeEnum::Value FilterType;
Q_DECLARE_METATYPE(FilterType)

#endif // FILTERTYPEENUM_H
