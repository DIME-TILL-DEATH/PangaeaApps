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
        EQ1,
        LP,
        NG,
        PS,
        //=====================
        // New moduls
        TR,
        CH,
        PH,
        EQ2,
        //---------
        NUM_MONO_MODULE_TYPES,
        //=====================
        // Stereo moduls
        ER_MONO = 64,
        ER_STEREO,
        DELAY,
        //====================
        MASTER=128
    };
    Q_ENUM(Value)
};

typedef ModuleTypeEnum::Value ModuleType;
Q_DECLARE_METATYPE(ModuleType)

#endif // MODULETYPEENUM_H
