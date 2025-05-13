#ifndef DUALCABSIM_H
#define DUALCABSIM_H

#include <QQmlEngine>
#include "abstractmodule.h"

class DualCabSim : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
public:
    DualCabSim(AbstractDevice *owner);
};

#endif // DUALCABSIM_H
