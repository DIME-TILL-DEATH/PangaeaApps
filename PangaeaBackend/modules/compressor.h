#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "abstractmodule.h"
#include "controlvalue.h"


class Compressor : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* sustain READ sustain NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

public:
    Compressor(AbstractDevice *owner);

    ControlValue *sustain() {return m_sustain;};
    ControlValue *volume() {return m_volume;};

    void setValues(bool enabled, quint8 sustain, quint8 volume);
    void setValues(const compressor_data_t& cmData);

private:
    ControlValue *m_sustain = nullptr;
    ControlValue *m_volume = nullptr;
};

#endif // COMPRESSOR_H
