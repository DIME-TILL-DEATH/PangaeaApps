#ifndef HARDWARECLASSICPRESET_H
#define HARDWARECLASSICPRESET_H

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

}preset_cplegacy_t;

//=======================New generation=======================

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t threshold;
    uint8_t decay;
    uint8_t reserved[4];
}gate_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t sustain;
    uint8_t volume;
    uint8_t reserved[4];
}compressor_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t volume;
    uint8_t low;
    uint8_t mid;
    uint8_t high;
    uint8_t reserved[2];
}preamp_cpmodern_t;

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
}pa_cpmodern_t;

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
}eq_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t rate;
    uint8_t depth;
    uint8_t type;
}tremolo_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t mix;
    uint8_t rate;
    uint8_t width;
    uint8_t hpf;
    //	uint8_t delay;
    //	uint8_t type;
    uint8_t reserved[3];
}chorus_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t mix;
    uint8_t rate;
    uint8_t center;
    uint8_t width;
    uint8_t feedback;
    uint8_t stages;
    uint8_t hpf;
}phaser_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t type;
    uint8_t volume;
    uint8_t reserved[5];
}early_cpmodern_t;

typedef struct
{
    uint8_t on;
    uint8_t mix;
    uint16_t time;
    uint8_t feedback;
    uint8_t lpf;
    uint8_t hpf;
    uint8_t reserved[2];
}delay_cpmodern_t;

#define MAX_PROCESSING_STAGES 10
typedef struct
{
    uint8_t modules_order[MAX_PROCESSING_STAGES];
    uint8_t reverb_config[2];
    uint8_t ir_send_level;
    uint8_t head_reserved; //[14-3-MAX_PROCESSING_STAGES];
    uint8_t volume;
    uint8_t cab_sim_on;

    gate_cpmodern_t 		gate;
    compressor_cpmodern_t 	compressor;
    preamp_cpmodern_t		preamp;
    pa_cpmodern_t			power_amp;

    eq_cpmodern_t eq1;
    eq_cpmodern_t eq2;

    tremolo_cpmodern_t tremolo;
    chorus_cpmodern_t chorus;
    phaser_cpmodern_t phaser;
    // flanger
    uint8_t reserved[12];

    early_cpmodern_t reverb;
    delay_cpmodern_t delay;
}preset_data_cpmodern_t;

typedef struct
{
    char name[32];
    preset_data_cpmodern_t parametersData;
}save_data_cpmodern_t;

namespace HardwarePresetCPModern
{
preset_data_cpmodern_t convertLegacyToModern(const preset_cplegacy_t& legacyData);
}

#endif // HARDWARECLASSICPRESET_H
