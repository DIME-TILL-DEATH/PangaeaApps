#ifndef EARLYREFLECTIONS_H
#define EARLYREFLECTIONS_H

#include "abstractmodule.h"

#include <QObject>
#include "controlvalue.h"

#include "presetlegacy.h"

class EarlyReflections : public AbstractModule
{
    Q_OBJECT

    Q_PROPERTY(ControlValue* reflectionsVolume READ reflectionsVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* reflectionsType READ reflectionsType NOTIFY dataChanged FINAL)
public:
    EarlyReflections(AbstractDevice *owner);

    ControlValue *reflectionsVolume() {return m_reflectionsVolume;};
    ControlValue *reflectionsType() {return m_reflectionsType;};

    void setValues(bool enabled, quint8 volume, quint8 type);
    void setValues(const reverb_data_t& rvData);

    // void sendDataToDevice(const QByteArray& baData) override;
private:
    ControlValue *m_reflectionsVolume = nullptr;
    ControlValue *m_reflectionsType = nullptr;
};

#endif // EARLYREFLECTIONS_H
