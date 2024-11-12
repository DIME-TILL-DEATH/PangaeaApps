#ifndef CONTROLQLEGACY_H
#define CONTROLQLEGACY_H

#include <QObject>
#include "controlvalue.h"

class ControlQLegacy : public ControlValue
{
    Q_OBJECT
public:
    ControlQLegacy(AbstractModule *parent, QString commandStr);

    virtual void setDisplayValue(double newDisplayValue);
    virtual void setControlValue(qint32 value);
};

#endif // CONTROLQLEGACY_H
