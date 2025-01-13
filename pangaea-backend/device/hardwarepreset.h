#ifndef HARDWAREPRESET_H
#define HARDWAREPRESET_H

#include <QObject>

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
    uint8_t depth;
    uint8_t reserved[1];
}pa_data_t;

typedef struct
{
    uint8_t parametric_on;
    uint8_t hp_on;
    uint8_t lp_on;

    uint16_t lp_freq;
    uint16_t hp_freq;

    int8_t gain[EQ_BANDS_COUNT];
    uint16_t freq[EQ_BANDS_COUNT];
    int8_t Q[EQ_BANDS_COUNT];
    uint8_t band_type[EQ_BANDS_COUNT];

    uint8_t reserved[4];
}eq_t;

typedef struct
{
    uint8_t on;
    uint8_t rate;
    uint8_t depth;
    uint8_t type;
}tremolo_t;

typedef struct
{
    uint8_t on;
    uint8_t mix;
    uint8_t rate;
    uint8_t width;
    uint8_t delay;
    uint8_t type;
    uint8_t hpf;
    uint8_t reserved;
}chorus_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t volume;
    uint8_t reserved[5];
}reverb_data_t;

#define MAX_PROCESSING_STAGES 10
typedef struct
{
    uint8_t modules_order[MAX_PROCESSING_STAGES];
    uint8_t head_reserved[14-MAX_PROCESSING_STAGES];
    uint8_t volume;
    uint8_t cab_sim_on;

    gate_data_t 		gate;
    compressor_data_t 	compressor;
    preamp_data_t		preamp;
    pa_data_t			power_amp;

    eq_t eq1;
    eq_t eq2;

    tremolo_t tremolo;
    chorus_t chorus;
    // phaser, flanger
    uint8_t reserved[12];

    reverb_data_t reverb;
}preset_data_t;

typedef struct
{
    char name[32];
    preset_data_t parametersData;
}save_data_t;

namespace HardwarePreset
{
preset_data_t convertLegacyToModern(const preset_data_legacy_t& legacyData);
}

#endif // HARDWAREPRESET_H
