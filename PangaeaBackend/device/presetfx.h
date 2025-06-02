#ifndef PRESETFX_H
#define PRESETFX_H

#include <QDebug>

#include "presetabstract.h"
#include "hardwarefxpreset.h"

#include "abstractdevice.h"

class PresetFx : public PresetAbstract
{
public:
    explicit PresetFx(AbstractDevice* ownerDevice);

    preset_data_fx_t presetData;

    static preset_data_fx_t charsToPresetData(const QByteArray& ba);
    static QByteArray presetDataToChars(const preset_data_fx_t& presetData);

    quint8 getPresetFlatNumber() const override {return 0;};

    bool isIrEnabled() const override {return false;};
    void setIsIrEnabled(bool newIsIrEnabled) override {};

    QString presetName() const override {return m_presetName;};
    void setPresetName(const QString &newPresetName) override {m_presetName = newPresetName;};

    QString presetComment() const {return m_presetComment;};
    void setPresetComment(const QString &newPresetComment) {m_presetComment = newPresetComment;};

    QString irName() const override {return "";};
    void setIrName(const QString &newIrName) override {};

private:
    AbstractDevice* m_ownerDevice;

    QString m_presetName;
    QString m_presetComment;
};

#endif // PRESETFX_H
