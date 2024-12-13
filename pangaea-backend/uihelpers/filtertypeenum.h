#ifndef FILTERTYPEENUM_H
#define FILTERTYPEENUM_H

#include <QObject>

class FilterTypeEnum
{
    Q_GADGET
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
