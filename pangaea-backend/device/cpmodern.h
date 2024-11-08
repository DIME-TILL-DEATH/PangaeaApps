#ifndef CPMODERN_H
#define CPMODERN_H

#include <QObject>
#include "abstractdevice.h"

class CPModern : public AbstractDevice
{
public:
    CPModern(Core *parent);
};

#endif // CPMODERN_H
