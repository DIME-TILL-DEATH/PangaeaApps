#ifndef PRESET_H
#define PRESET_H

#include <QFile>
#include <QString>
#include <QByteArray>

#include "device.h"

class Preset
{
public:
    Preset(Device* owner);

    quint8 bankNumber() const;
    quint8 presetNumber() const;
    void setBankPreset(quint8 newBankNumber, quint8 newPresetNumber);

    bool importData(QString filePath);
    bool exportData();

    const QByteArray &rawData() const;
    void setRawData(const QByteArray &newRawData);

    const QString &impulseName() const;
    void setImpulseName(const QString &newImpulseName);

    quint8 getPresetFlatIndex();
    quint8 getPresetFlatNumber();

    bool isIrEnabled() const;
    void setIsIrEnabled(bool newIsIrEnabled);

    const QByteArray &waveData() const;

    const QString &pathToExport() const;
    void setPathToExport(const QString &newPathToExport);

    void setWaveData(const QByteArray &newWaveData);
    void clearWavData();

    quint32 wavSize() const;

    Device *ownerDevice() const;
    void setOwnerDevice(Device *newOwnerDevice);

private:
    Device* m_ownerDevice;

    quint8 m_presetNumber{0};
    quint8 m_bankNumber{0};

    quint8 presetVersion{1};

    // TODO: заменить DeviceControls? Все параемтры?
    bool m_isIrEnabled{false};
    QString m_impulseName{"none"};

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

#endif // PRESET_H
