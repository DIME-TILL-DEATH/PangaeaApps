#ifndef PRESETMODERN_H
#define PRESETMODERN_H

#include <QDebug>

#include "presetabstract.h"
#include "hardwarepreset.h"

#include "irfile.h"

class AbstractDevice;

class PresetModern : public PresetAbstract
{
public:
    PresetModern(AbstractDevice* ownerDevice);
    ~PresetModern() {};

    PresetAbstract& operator=(const PresetAbstract& preset) override;

    preset_data_t presetData;

    static preset_data_t charsToPresetData(const QByteArray& ba);
    static QByteArray presetDatatoChars(const preset_data_t& presetData);

    // TODO to static (Preset, waveData)
    bool importData(QString filePath);
    bool exportData();

    // const QString &pathToExport() const {return m_pathToExport;};
    // void setPathToExport(const QString &newPathToExport) {m_pathToExport = newPathToExport;};

    quint8 getPresetFlatNumber() const override;

    bool isIrEnabled() const override;
    void setIsIrEnabled(bool newIsIrEnabled) override;

    QString presetName() const override {return m_presetName;};
    void setPresetName(const QString &newPresetName) override {m_presetName = newPresetName;};

    IrFile irFile;

    QString irName() const override {return irFile.irName();};
    void setIrName(const QString &newIrName) override {irFile.setIrName(newIrName);};

private:
    AbstractDevice* m_ownerDevice;
    QString m_presetName;

    void writePresetChunk(QByteArray& fileData, QString& chunkName, const QByteArray &chunkData);
    QByteArray readPresetChunck(const QByteArray& fileData, QString& chunkName);


};

#endif // PRESETMODERN_H