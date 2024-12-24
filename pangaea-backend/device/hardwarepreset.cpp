#include "hardwarepreset.h"

#include "moduletypeenum.h"

preset_data_t HardwarePreset::convertLegacyToModern(const preset_data_legacy_t& legacyData)
{
    qint16 defaultFrequencies[] = {120, 360, 800, 2000, 6000};
    preset_data_t modernData{0};

    memset(&modernData, 0, sizeof(preset_data_t));

    // secondary eq default
    for(int i=0; i<5; i++)
    {
        modernData.eq2.gain[i] = 0;
        modernData.eq2.freq[i] = defaultFrequencies[i];
    }

    if(legacyData.eq_pre)
    {
        modernData.modules_order[0] = ModuleType::CM;
        modernData.modules_order[1] = ModuleType::EQ1;
        modernData.modules_order[2] = ModuleType::PR;
        modernData.modules_order[3] = ModuleType::PA;
        modernData.modules_order[4] = ModuleType::IR;
        modernData.modules_order[5] = ModuleType::NG;
    }
    else
    {
        modernData.modules_order[0] = ModuleType::CM;
        modernData.modules_order[1] = ModuleType::PR;
        modernData.modules_order[2] = ModuleType::PA;
        modernData.modules_order[3] = ModuleType::IR;
        modernData.modules_order[4] = ModuleType::EQ1;
        modernData.modules_order[5] = ModuleType::NG;
    }

    modernData.volume = legacyData.preset_volume;

    modernData.gate.on = legacyData.gate_on;
    modernData.gate.threshold = legacyData.gate_threshold;
    modernData.gate.decay = legacyData.gate_decay;

    modernData.compressor.on = legacyData.compressor_on;
    modernData.compressor.sustain = legacyData.compressor_sustain;
    modernData.compressor.volume = legacyData.compressor_volume;

    modernData.preamp.on = legacyData.preamp_on;
    modernData.preamp.volume = legacyData.preamp_volume;
    modernData.preamp.low = legacyData.preamp_low;
    modernData.preamp.mid = legacyData.preamp_mid;
    modernData.preamp.high = legacyData.preamp_high;

    modernData.power_amp.on = legacyData.amp_on;
    modernData.power_amp.volume = legacyData.amp_volume;
    modernData.power_amp.slave = legacyData.amp_slave;
    modernData.power_amp.type = legacyData.amp_type;
    modernData.power_amp.presence_on = legacyData.presence_on;
    modernData.power_amp.presence_vol = legacyData.presence_vol;

    modernData.eq1.parametric_on = legacyData.eq_on;
    for(int i=0; i<5; i++)
    {
        modernData.eq1.gain[i] = legacyData.eq_band_vol[i] - 15;
        modernData.eq1.freq[i] = defaultFrequencies[i] + legacyData.eq_freq[i];
        modernData.eq1.Q[i] = legacyData.eq_Q[i];
    }
    modernData.eq1.hp_on = legacyData.hp_on;
    modernData.eq1.hp_freq = legacyData.hp_freq * (980.0f / 255.0f) + 20.0f;
    modernData.eq1.lp_on = legacyData.lp_on;
    modernData.eq1.lp_freq = powf(195 - legacyData.lp_freq, 2.0f) * (19000.0f / powf(195.0f, 2.0f)) + 1000.0f;

    modernData.cab_sim_on = legacyData.cab_on;

    modernData.reverb.on = legacyData.early_on;
    modernData.reverb.type = legacyData.early_type;
    modernData.reverb.volume = legacyData.early_volume;

    return modernData;
}
