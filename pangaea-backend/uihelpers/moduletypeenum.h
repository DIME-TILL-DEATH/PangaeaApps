#ifndef MODULETYPEENUM_H
#define MODULETYPEENUM_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class ModuleTypeEnum
{
    Q_GADGET
    QML_ELEMENT
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
        PS,
        //=====================
        // Stereo moduls
        ER = 64,
        //====================
        MASTER=128
    };
    Q_ENUM(Value)
};

typedef ModuleTypeEnum::Value ModuleType;
Q_DECLARE_METATYPE(ModuleType)

#endif // MODULETYPEENUM_H
