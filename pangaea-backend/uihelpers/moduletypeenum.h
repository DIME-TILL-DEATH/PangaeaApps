#ifndef MODULETYPEENUM_H
#define MODULETYPEENUM_H

#include <QObject>

class ModuleTypeEnum
{
    Q_GADGET
public:
    explicit ModuleTypeEnum(){};
    enum Value
    {
        BYPASS = 0,
        CM,
        PR,
        PA,
        IR,
        HP,
        EQ,
        LP,
        NG,

        //=====================
        MASTER=128
    };
    Q_ENUM(Value)
};

typedef ModuleTypeEnum::Value ModuleType;
Q_DECLARE_METATYPE(ModuleType)

#endif // MODULETYPEENUM_H
