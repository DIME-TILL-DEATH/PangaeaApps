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

    void setValues(const preset_data_fx_t &csData) override;
};

#endif // DUALCABSIM_H
