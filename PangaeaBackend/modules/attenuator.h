#ifndef ATTENUATOR_H
#define ATTENUATOR_H

#include "abstractmodule.h"

#include "controlvalue.h"

class Attenuator : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* value READ value NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* source READ source NOTIFY dataChanged FINAL)
public:
    Attenuator(AbstractDevice *owner);

    ControlValue* value() const {return m_value;};
    ControlValue* source() const {return m_source;};

    void setGlobalValue(quint8 val);
    void setPresetValue(quint8 val);
    void setSource(quint8 src);

private:
    ControlValue *m_value = nullptr;
    ControlValue *m_source = nullptr;

    void srcDisplaySetter(double value);
    void srcControlSetter(qint32 value);
    void valControlSetter(qint32 value);

    qint32 m_presetAttValue{0};
    qint32 m_globalAttValue{0};
};

#endif // ATTENUATOR_H
