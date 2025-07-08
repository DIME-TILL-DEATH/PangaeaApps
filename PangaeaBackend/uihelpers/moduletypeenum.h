#ifndef MODULETYPEENUM_H
#define MODULETYPEENUM_H

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class ModuleTypeEnum : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ModuleType)
    QML_UNCREATABLE("")
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
        //
        NUM_MONO_MODULE_TYPES,
        //==========Only FX-ultima modules
        RF,
        RV,
        FL,
        MEQ,
        //=====================
        // Stereo moduls
        ER_MONO = 64,
        ER_STEREO,
        DELAY,
        IR_STEREO,
        //====================
        MASTER=128
    };
    Q_ENUM(Value)
};

typedef ModuleTypeEnum::Value ModuleType;
Q_DECLARE_METATYPE(ModuleType)

#endif // MODULETYPEENUM_H
