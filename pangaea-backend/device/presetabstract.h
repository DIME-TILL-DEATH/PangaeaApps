#ifndef PRESETABSTRACT_H
#define PRESETABSTRACT_H

#include <QFile>
#include <QString>
#include <QByteArray>

#include "devicetypeenum.h"

class PresetAbstract
{
public:
    PresetAbstract();
    virtual ~PresetAbstract() {};

    static quint8 calcPresetFlatIndex(DeviceType deviceType, quint8 bankNumber, quint8 presetNumber);

    quint8 bankNumber() const {return m_bankNumber;};
    quint8 presetNumber() const { return m_presetNumber;};
    void setBankPreset(quint8 newBankNumber, quint8 newPresetNumber);

    const QString &irName() const {return m_irName;};
    void setIrName(const QString &newIrName) {m_irName = newIrName;};

    virtual quint8 getPresetFlatNumber() const = 0;

    virtual bool isIrEnabled() const = 0;
    virtual void setIsIrEnabled(bool newIsIrEnabled) = 0;

    virtual QString presetName() const = 0;
    virtual void setPresetName(const QString &newPresetName) = 0;

protected:
    quint8 m_presetNumber{0};
    quint8 m_bankNumber{0};

    quint8 presetVersion{1};

    QString m_irName;
};

#endif // PRESETABSTRACT_H
