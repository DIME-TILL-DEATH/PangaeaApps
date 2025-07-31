#ifndef PRESETFX_H
#define PRESETFX_H

#include <QDebug>

#include "controllerfx.h"
#include "presetabstract.h"
#include "hardwarefxpreset.h"

#include "abstractdevice.h"

class PresetFx : public PresetAbstract
{
public:
    explicit PresetFx(AbstractDevice* ownerDevice);

    PresetAbstract& operator=(const PresetAbstract& preset) override;

    static modules_data_fx_t charsToPresetData(const QByteArray& ba);
    static QByteArray presetDataToChars(const modules_data_fx_t& presetData);

    quint8 getPresetFlatNumber() const override {return m_presetNumber;};

    bool isIrEnabled() const override {return false;};
    void setIsIrEnabled(bool newIsIrEnabled) override {};

    QString presetName() const override {return m_presetName;};
    void setPresetName(const QString &newPresetName) override {m_presetName = newPresetName;};

    QString presetComment() const {return m_presetComment;};
    void setPresetComment(const QString &newPresetComment) {m_presetComment = newPresetComment;};

    QString irName() const override {return m_ir1Name;};
    void setIrName(const QString &newIrName) override {m_ir1Name = newIrName;};

    QString ir1Name() const {return m_ir1Name;};
    void setIr1Name(const QString &newIrName) {m_ir1Name = newIrName;};

    QString ir2Name() const {return m_ir2Name;};
    void setIr2Name(const QString &newIrName) {m_ir2Name = newIrName;};

    void setActiveModules(const QByteArray& ba);
    // QList<ModuleType> activeModules() const {return m_activeModules;};
    QStringList strActiveModules() const;

    modules_data_fx_t presetData;
    void setPresetData(const modules_data_fx_t &newPresetData);

    controller_fx_t controller[ControllersCount];

    quint8 cntrlPcOut() const {return m_cntrlPcOut;};
    void setCntrlPcOut(quint8 newCntrlPcOut);

    quint8 cntrlSet() const {return m_cntrlSet;};
    void setCntrlSet(quint8 newCntrlSet);

    static constexpr uint16_t CabinetSize = 4096 * 3;

private:
    AbstractDevice* m_ownerDevice;

    // QList<ModuleType> m_activeModules;

    QString m_presetName;
    QString m_presetComment;

    QString m_ir1Name;
    QString m_ir2Name;

    quint8 m_cntrlPcOut;
    quint8 m_cntrlSet; 
};

#endif // PRESETFX_H
