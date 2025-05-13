#ifndef HARDWAREFXPRESET_H
#define HARDWAREFXPRESET_H

#include <QObject>

typedef struct
{
    uint8_t amp;
    uint8_t cab;
    uint8_t eq;
    uint8_t delay;
    uint8_t phaser;
    uint8_t flanger;
    uint8_t chorus;
    uint8_t early_reflections;
    uint8_t reverb;
    uint8_t tremolo;
    uint8_t preamp;
    uint8_t gate;
    uint8_t compressor;
    uint8_t resonance_filter;
}switch_fx_t;

typedef struct
{
    uint8_t volume;
    uint8_t pan;
    uint8_t delay;
}cab_fx_t;

typedef struct
{
    uint8_t mix;
    uint8_t rate;
    uint8_t width;
    uint8_t center;
    uint8_t feedback;
    uint8_t type;
}phaser_fx_t;

typedef struct
{
    uint8_t mix;    // volume
    uint8_t rate;
    uint8_t width;
    uint8_t type;
    uint8_t delay;
}chorus_fx_t;

typedef struct
{
    uint8_t volumeFirst;
    uint8_t feedback;
    uint8_t lpf;
    uint8_t hpf;
    uint8_t panFirst;
    uint8_t volumeSecond;
    uint8_t panSecond;
    uint8_t offset; // d - offset?
    uint8_t modulation;
    uint8_t rate;
    uint8_t direction;
}delay_fx_t;

typedef struct
{
    uint8_t mix;
    uint8_t lfo;
    uint8_t rate;
    uint8_t width;
    uint8_t delay;
    uint8_t feedback;
}flanger_fx_t;

typedef struct
{
    uint8_t mix;
    uint8_t time;
    uint8_t size;
    uint8_t damping;
    uint8_t lpf;
    uint8_t hpf;
    uint8_t detune;
}reverb_fx_t;

typedef struct
{
    uint8_t depth;  // volume
    uint8_t rate;
    uint8_t lfo;
    uint8_t ms;
}tremolo_fx_t;

typedef struct
{
    uint8_t master;
    uint8_t type;
    uint8_t level;
}pa_fx_t;

typedef struct
{
    uint8_t volume;
    uint8_t type;
}early_fx_t;

typedef struct
{
    uint8_t gain;
    uint8_t volume;
    uint8_t pre_0;
    uint8_t low;
    uint8_t mid;
    uint8_t high;
}preamp_fx_t;

typedef struct
{
    uint8_t threshold;
    uint8_t attack;
    uint8_t decay;
}gate_fx_t;

typedef struct
{
    uint8_t threshold;
    uint8_t ratio;
    uint8_t volume;
    uint8_t attack;
    uint8_t knee;
}compressor_fx_t;

typedef struct
{
    uint8_t mix;
    uint8_t type;
    uint8_t mod;
    uint8_t rate;
    uint8_t lpf;
    uint8_t hpf;
    uint8_t resonance;
    uint8_t dyn_threshold;
    uint8_t dyn_attack;
    uint8_t dyn_release;
    uint8_t vo;
    uint8_t ext;
}resonance_filter_fx_t;

typedef struct
{
    switch_fx_t switches;       //+

    uint8_t dummy1[27];

    uint8_t cab_mono;
    cab_fx_t cab1;
    cab_fx_t cab2;

    uint8_t eq_gain[5];

    uint8_t hpf;
    uint8_t lpf;
    uint8_t presence;

    phaser_fx_t phaser;             //+
    chorus_fx_t chorus;             //+
    delay_fx_t delay;               //+
    flanger_fx_t flanger;           //+
    reverb_fx_t reverb;
    tremolo_fx_t tremolo;           //+

    uint8_t preset_volume;          //+

    pa_fx_t pa;                     //+
    early_fx_t early_reflections;   //+

    uint8_t eq_freq[5];
    uint8_t eq_q[5];

    preamp_fx_t preamp;         //+
    gate_fx_t gate;             //+
    compressor_fx_t compressor; //+

    uint8_t hpf_phaser;         //+
    uint8_t hpf_flanger;        //+
    uint8_t hpf_chorus;         //+

    uint8_t delay_tap;          //+
    uint8_t tremolo_tap;        //+

    resonance_filter_fx_t resonance_filter; //+

    uint8_t reverb_type;        //??????
    uint8_t reverb_diffusion;   //??????

    uint8_t resonance_filter_gen_type;  //+

    uint8_t volume_control;

    uint8_t eq_pre_post;

    uint8_t delay_time_hi;  //+ BYTE
    uint8_t delay_time_lo;  //+ BYTE

    uint8_t phaser_pre_post;
    uint8_t flanger_pre_post;

    uint8_t reverb_predelay;    //??????

    uint8_t foot_ind1[3]; // /*foot ind*/fo1,fo2,fo3
    uint8_t foot_ind2[3]; // /*foot ind*/fo11,fo12,fo13

    uint8_t tremolo_lfo_type;   //+ ????

    uint8_t delay_tail;         //+
    uint8_t reverb_tail;        //????

    uint8_t dummy2[350];

    uint8_t bpm_delay;
}preset_data_fx_t;

#endif // HARDWAREFXPRESET_H
