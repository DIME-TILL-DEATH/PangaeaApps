#ifndef PRESET_H
#define PRESET_H

#include <QFile>
#include <QString>
#include <QByteArray>

#include "device.h"

#define EQ_BANDS_COUNT 5

typedef struct
{
    uint8_t eq_band_vol[EQ_BANDS_COUNT];

    uint8_t early_volume;
    uint8_t early_type;
    uint8_t preset_volume;
    uint8_t cab_on;
    uint8_t eq_on;
    uint8_t early_on;

    uint8_t amp_on;
    uint8_t amp_volume;
    uint8_t amp_slave;
    uint8_t amp_type;

    uint8_t preamp_on;
    uint8_t preamp_volume;
    uint8_t preamp_low;
    uint8_t preamp_mid;
    uint8_t preamp_high;

    uint8_t gate_on;
    uint8_t gate_threshold;
    uint8_t gate_decay;

    uint8_t compressor_on;
    uint8_t compressor_sustain;
    uint8_t compressor_volume;

    uint8_t eq_freq[EQ_BANDS_COUNT];
    uint8_t eq_Q[EQ_BANDS_COUNT];

    uint8_t lp_freq;
    uint8_t hp_freq;

    uint8_t hp_on;
    uint8_t lp_on;

    uint8_t presence_on;
    uint8_t presence_vol;

    uint8_t eq_pre;

}preset_data_legacy_t;

//=======================New generation=======================

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t threshold;
    uint8_t decay;
    uint8_t reserved[4];
}gate_data_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t sustain;
    uint8_t volume;
    uint8_t reserved[4];
}compressor_data_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t volume;
    uint8_t low;
    uint8_t mid;
    uint8_t high;
    uint8_t reserved[2];
}preamp_data_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t volume;
    uint8_t slave;
    uint8_t presence_on;
    uint8_t presence_vol;
    uint8_t reserved[2];
}pa_data_t;

typedef struct
{
    uint8_t parametric_on;
    uint8_t hp_on;
    uint8_t lp_on;

    uint8_t lp_freq;
    uint8_t hp_freq;

    uint8_t band_vol[EQ_BANDS_COUNT];
    uint8_t freq[EQ_BANDS_COUNT];
    uint8_t Q[EQ_BANDS_COUNT];
    uint8_t band_type[EQ_BANDS_COUNT];

    uint8_t reserved[7];
}eq_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t volume;
    uint8_t reserved[5];
}reverb_data_t;

typedef struct
{
    uint8_t name[16];
    uint8_t modules_order[16];
    uint8_t volume;

    uint8_t reserved_pre[63];

    gate_data_t 		gate;
    compressor_data_t 	compressor;
    preamp_data_t		preamp;
    pa_data_t			power_amp;

    eq_t eq1;
    eq_t eq2;

    reverb_data_t reverb;

    // IR data
    uint8_t cab_sim_on;
    uint8_t reserved_post[3];
}preset_data_t;

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

    static quint8 calcPresetFlatIndex(DeviceType deviceType, quint8 bankNumber, quint8 presetNumber);
    quint8 getPresetFlatIndex() const;
    quint8 getPresetFlatNumber() const;

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

    // TODO: заменить на rawBA или соответствующую структуру
    bool m_isIrEnabled{false};
    QString m_impulseName{""};

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


    preset_data_legacy_t legacyDataStruct;
};

#endif // PRESET_H
