#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QObject>
#include "abstractmodule.h"
#include "controlvalue.h"

#include "preset.h"

class Compressor : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* threshold READ threshold NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* volume READ volume NOTIFY dataChanged FINAL)

public:
    Compressor(AbstractDevice *owner);

    ControlValue *threshold() {return m_sustain;};
    ControlValue *volume() {return m_volume;};

    void setValues(bool enabled, quint8 sustain, quint8 volume);
    void setValues(const compressor_data_t& cmData);

private:
    ControlValue *m_sustain = nullptr;
    ControlValue *m_volume = nullptr;
};

#endif // COMPRESSOR_H
