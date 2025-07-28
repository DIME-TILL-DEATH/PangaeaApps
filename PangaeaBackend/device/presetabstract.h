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

    virtual PresetAbstract& operator=(const PresetAbstract& preset);

    static quint8 calcPresetFlatIndex(DeviceType deviceType, quint8 bankNumber, quint8 presetNumber);

    quint8 bankNumber() const {return m_bankNumber;};
    quint8 presetNumber() const { return m_presetNumber;};
    void setBankPreset(quint8 newBankNumber, quint8 newPresetNumber);

    virtual QString irName() const = 0;
    virtual void setIrName(const QString &newIrName) = 0;

    virtual quint8 getPresetFlatNumber() const = 0;

    virtual bool isIrEnabled() const = 0;
    virtual void setIsIrEnabled(bool newIsIrEnabled) = 0;

    virtual QString presetName() const = 0;
    virtual void setPresetName(const QString &newPresetName) = 0;

    static QByteArray dataToBa(void* data, quint32 size);


protected:
    quint8 m_presetNumber{0};
    quint8 m_bankNumber{0};

    quint8 presetVersion{1};

    struct
    {
        QString header{"PANGAEA_PRESET"};
        QString versionId{"VERSION"};

        QString dataId{"DATA"};
        QString irNameId{"IR_NAME"};
        QString irDataId{"IR_DATA"};
    } presetHeaderId;
};

#endif // PRESETABSTRACT_H
