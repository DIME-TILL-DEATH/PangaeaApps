#ifndef PRESETLEGACY_H
#define PRESETLEGACY_H

#include <QFile>
#include <QString>
#include <QByteArray>

#include "presetabstract.h"
#include "devicetypeenum.h"

class AbstractDevice;

class PresetLegacy : public PresetAbstract
{
public:
    PresetLegacy(AbstractDevice* owner);

    bool importData(QString filePath);
    bool exportData();

    const QByteArray &rawData() const {return m_rawData;};
    void setRawData(const QByteArray &newRawData);

    static quint8 calcPresetFlatIndex(DeviceType deviceType, quint8 bankNumber, quint8 presetNumber);
    // quint8 getPresetFlatIndex() const;
    quint8 getPresetFlatNumber() const override;

    bool isIrEnabled() const override;
    void setIsIrEnabled(bool newIsIrEnabled) override;

    const QByteArray &waveData() const {return m_waveData;};

    const QString &pathToExport() const {return m_pathToExport;};
    void setPathToExport(const QString &newPathToExport);

    void setWaveData(const QByteArray &newWaveData);
    void clearWavData();

    quint32 wavSize() const {return m_waveData.size();};

    QString presetName() const override {return "";};
    void setPresetName(const QString &newPresetName) override {};

private:
    AbstractDevice* m_ownerDevice;

    QString m_pathToExport;

    QByteArray m_rawData;
    QByteArray m_waveData;

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

#endif // PRESETLEGACY_H
