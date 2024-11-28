#ifndef PRESETMODERN_H
#define PRESETMODERN_H

#include "presetabstract.h"
#include "hardwarepreset.h"
#include <QDebug>

class AbstractDevice;

class PresetModern : public PresetAbstract
{
public:
    PresetModern(AbstractDevice* ownerDevice);
    ~PresetModern() {};

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

private:
    AbstractDevice* m_ownerDevice;
    QString m_presetName;


    QString m_irDir;

    // QString m_pathToExport;

    void writePresetChunk(QByteArray& fileData, QString& chunkName, const QByteArray &chunkData);
    QByteArray readPresetChunck(const QByteArray& fileData, QString& chunkName);

    struct
    {
        QString header{"PANGAEA_PRESET"};
        QString versionId{"VERSION"};

        QString dataId{"DATA"};
        QString irNameId{"IR_NAME"};
        QString irDataId{"IR_DATA"};
    } presetHeaderId;
};

#endif // PRESETMODERN_H
