#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "abstractmodule.h"

#include "controlvalue.h"

#include "presetlegacy.h"

class LowPassFilter : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* lpf READ lpf NOTIFY dataChanged FINAL)
public:
    LowPassFilter(AbstractDevice *owner, preset_data_cplegacy_t* lpData);

    ControlValue* lpf() {return m_lpf;};

    void setValues(const preset_data_cplegacy_t& lpData) override;

private:
    ControlValue* m_lpf;
};

#endif // LOWPASSFILTER_H
