#ifndef EARLYREFLECTIONS_H
#define EARLYREFLECTIONS_H

#include "abstractmodule.h"

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "controlvalue.h"

#include "presetlegacy.h"

#include "moduletypeenum.h"

class EarlyReflections : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* reflectionsVolume READ reflectionsVolume NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* reflectionsType READ reflectionsType NOTIFY dataChanged FINAL)
    Q_PROPERTY(ModuleType reverbType READ reverbType WRITE setReverbType NOTIFY reverbTypeChanged FINAL)

    Q_PROPERTY(quint8 processingTimeMono READ processingTimeMono CONSTANT)
    Q_PROPERTY(quint8 processingTimeStereo READ processingTimeStereo CONSTANT)
public:
    enum EarlyType{
        Classic = 0,
        FX
    };

    EarlyReflections(AbstractDevice *owner, EarlyType earlyType = Classic);

    ControlValue *reflectionsVolume() {return m_reflectionsVolume;};
    ControlValue *reflectionsType() {return m_reflectionsType;};

    void setValues(const preset_data_cplegacy_t& erData) override;
    void setValues(const preset_data_cpmodern_t& erData) override;
    void setValues(const modules_data_fx_t &erData) override;

    quint16 processingTime() const override;
    quint8 processingTimeMono() {return m_processingTimeMono;};
    quint8 processingTimeStereo() {return m_processingTimeStereo;};

    // void sendDataToDevice(const QByteArray& baData) override;
    ModuleType reverbType() const;
    void setReverbType(const ModuleType &newReverbType);

signals:
    void reverbTypeChanged();

private:
    ControlValue *m_reflectionsVolume = nullptr;
    ControlValue *m_reflectionsType = nullptr;
    ModuleType m_reverbType;

    static constexpr quint8 m_processingTimeMono = 40;
    static constexpr quint8 m_processingTimeStereo = 80;
};

#endif // EARLYREFLECTIONS_H
