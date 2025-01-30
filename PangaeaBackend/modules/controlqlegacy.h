#ifndef CONTROLQLEGACY_H
#define CONTROLQLEGACY_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "controlvalue.h"

class ControlQLegacy : public ControlValue
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
public:
    ControlQLegacy(AbstractModule *parent, QString commandStr);

    virtual void setDisplayValue(double newDisplayValue);
    virtual void setControlValue(qint32 value);
};

#endif // CONTROLQLEGACY_H
