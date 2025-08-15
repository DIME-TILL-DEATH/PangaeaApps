#include <QDebug>
#include <QStandardPaths>

#include <qendian.h>

#include "presetfx.h"

#include "mockcp100fx.h"

MockCP100fx::MockCP100fx(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : AbstractMockDevice{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_CP100FX;

    initFolders();
    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP100fx::amtDevCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP100fx::amtVerCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("plist", std::bind(&MockCP100fx::plistCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("sys_settings", std::bind(&MockCP100fx::sysSettingsCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("pnum", std::bind(&MockCP100fx::pnumCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pname", std::bind(&MockCP100fx::pnameCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("pcomment", std::bind(&MockCP100fx::pcommentCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("state", std::bind(&MockCP100fx::stateCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("ir", std::bind(&MockCP100fx::irCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cd", std::bind(&MockCP100fx::cdCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("ls", std::bind(&MockCP100fx::lsCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("upload", std::bind(&MockCP100fx::uploadCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("mkdir", std::bind(&MockCP100fx::mkdirCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("rename", std::bind(&MockCP100fx::renameCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("remove", std::bind(&MockCP100fx::removeCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("cntrls", std::bind(&MockCP100fx::cntrlsCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrl_pc", std::bind(&MockCP100fx::cntrlsPcCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrl_set", std::bind(&MockCP100fx::cntrlsSetCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("pchange", std::bind(&MockCP100fx::pchangeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("psave", std::bind(&MockCP100fx::psaveCommHandler, this, _1, _2, _3));

    //--------------------------params handlers----------------------
    setParamsHandler("rf_on", &currentPresetData.modules.switches.resonance_filter);
    setParamsHandler("rf_mx", &currentPresetData.modules.resonance_filter.mix);
    setParamsHandler("rf_ft", &currentPresetData.modules.resonance_filter.f_type);
    setParamsHandler("rf_fm", &currentPresetData.modules.resonance_filter.f_mod);
    setParamsHandler("rf_lr", &currentPresetData.modules.resonance_filter.lfo_rate);
    setParamsHandler("rf_lp", &currentPresetData.modules.resonance_filter.lpf);
    setParamsHandler("rf_hp", &currentPresetData.modules.resonance_filter.hpf);
    setParamsHandler("rf_rs", &currentPresetData.modules.resonance_filter.resonance);
    setParamsHandler("rf_dt", &currentPresetData.modules.resonance_filter.dyn_threshold);
    setParamsHandler("rf_da", &currentPresetData.modules.resonance_filter.dyn_attack);
    setParamsHandler("rf_dr", &currentPresetData.modules.resonance_filter.dyn_release);
    setParamsHandler("rf_vl", &currentPresetData.modules.resonance_filter.volume);
    setParamsHandler("rf_lt", &currentPresetData.modules.resonance_filter_gen_type);

    setParamsHandler("ng_on", &currentPresetData.modules.switches.gate);
    setParamsHandler("ng_th", &currentPresetData.modules.gate.threshold);
    setParamsHandler("ng_dc", &currentPresetData.modules.gate.decay);
    setParamsHandler("ng_at", &currentPresetData.modules.gate.attack);

    setParamsHandler("cm_on", &currentPresetData.modules.switches.compressor);
    setParamsHandler("cm_th", &currentPresetData.modules.compressor.threshold);
    setParamsHandler("cm_vl", &currentPresetData.modules.compressor.volume);
    setParamsHandler("cm_rt", &currentPresetData.modules.compressor.ratio);
    setParamsHandler("cm_at", &currentPresetData.modules.compressor.attack);
    setParamsHandler("cm_kn", &currentPresetData.modules.compressor.knee);

    setParamsHandler("pr_on", &currentPresetData.modules.switches.preamp);
    setParamsHandler("pr_gn", &currentPresetData.modules.preamp.gain);
    setParamsHandler("pr_vl", &currentPresetData.modules.preamp.volume);
    setParamsHandler("pr_lo", &currentPresetData.modules.preamp.low);
    setParamsHandler("pr_mi", &currentPresetData.modules.preamp.mid);
    setParamsHandler("pr_hi", &currentPresetData.modules.preamp.high);

    setParamsHandler("pa_on", &currentPresetData.modules.switches.amp);
    setParamsHandler("pa_ms", &currentPresetData.modules.pa.master);
    setParamsHandler("pa_lv", &currentPresetData.modules.pa.level);
    setParamsHandler("pa_ps", &currentPresetData.modules.presence);
    setParamsHandler("pa_tp", &currentPresetData.modules.pa.type);

    setParamsHandler("eq_on", &currentPresetData.modules.switches.eq);
    setParamsHandler("eq_pp", &currentPresetData.modules.eq_pre_post);
    setParamsHandler("lp_f", &currentPresetData.modules.lpf);
    setParamsHandler("hp_f", &currentPresetData.modules.hpf);
    m_parser.addCommandHandler("eq_g", std::bind(&MockCP100fx::eqgCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("eq_f", std::bind(&MockCP100fx::eqfCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("eq_q", std::bind(&MockCP100fx::eqqCommHandler, this, _1, _2, _3));

    setParamsHandler("ir_on", &currentPresetData.modules.switches.cab);
    setParamsHandler("ir_v1", &currentPresetData.modules.cab1.volume);
    setParamsHandler("ir_p1", &currentPresetData.modules.cab1.pan);
    setParamsHandler("ir_d1", &currentPresetData.modules.cab1.delay);
    setParamsHandler("ir_v2", &currentPresetData.modules.cab2.volume);
    setParamsHandler("ir_p2", &currentPresetData.modules.cab2.pan);
    setParamsHandler("ir_d2", &currentPresetData.modules.cab2.delay);

    setParamsHandler("ph_on", &currentPresetData.modules.switches.phaser);
    setParamsHandler("ph_mx", &currentPresetData.modules.phaser.mix);
    setParamsHandler("ph_rt", &currentPresetData.modules.phaser.rate);
    setParamsHandler("ph_cn", &currentPresetData.modules.phaser.center);
    setParamsHandler("ph_wd", &currentPresetData.modules.phaser.width);
    setParamsHandler("ph_fb", &currentPresetData.modules.phaser.feedback);
    setParamsHandler("ph_st", &currentPresetData.modules.phaser.type);
    setParamsHandler("ph_hp", &currentPresetData.modules.hpf_phaser);
    setParamsHandler("ph_pp", &currentPresetData.modules.phaser_pre_post);

    setParamsHandler("fl_on", &currentPresetData.modules.switches.flanger);
    setParamsHandler("fl_mx", &currentPresetData.modules.flanger.mix);
    setParamsHandler("fl_lf", &currentPresetData.modules.flanger.lfo);
    setParamsHandler("fl_rt", &currentPresetData.modules.flanger.rate);
    setParamsHandler("fl_wd", &currentPresetData.modules.flanger.width);
    setParamsHandler("fl_dl", &currentPresetData.modules.flanger.delay);
    setParamsHandler("fl_fb", &currentPresetData.modules.flanger.feedback);
    setParamsHandler("fl_hp", &currentPresetData.modules.hpf_flanger);
    setParamsHandler("fl_pp", &currentPresetData.modules.flanger_pre_post);

    setParamsHandler("ch_on", &currentPresetData.modules.switches.chorus);
    setParamsHandler("ch_mx", &currentPresetData.modules.chorus.mix);
    setParamsHandler("ch_rt", &currentPresetData.modules.chorus.rate);
    setParamsHandler("ch_wd", &currentPresetData.modules.chorus.width);
    setParamsHandler("ch_dl", &currentPresetData.modules.chorus.delay);
    setParamsHandler("ch_tp", &currentPresetData.modules.chorus.type);
    setParamsHandler("ch_hp", &currentPresetData.modules.hpf_chorus);

    setParamsHandler("dl_on", &currentPresetData.modules.switches.delay);
    setParamsHandler("dl_m1", &currentPresetData.modules.delay.volumeFirst);
    setParamsHandler("dl_m2", &currentPresetData.modules.delay.volumeSecond);
    setParamsHandler("dl_p1", &currentPresetData.modules.delay.panFirst);
    setParamsHandler("dl_p2", &currentPresetData.modules.delay.panSecond);
    setParamsHandler("dl_tm", &currentPresetData.modules.delay_time); // LSB
    setParamsHandler("dl_fb", &currentPresetData.modules.delay.feedback);
    setParamsHandler("dl_hp", &currentPresetData.modules.delay.hpf);
    setParamsHandler("dl_lp", &currentPresetData.modules.delay.lpf);
    setParamsHandler("dl_os", &currentPresetData.modules.delay.offset);
    setParamsHandler("dl_md", &currentPresetData.modules.delay.modulation);
    setParamsHandler("dl_rt", &currentPresetData.modules.delay.rate);
    setParamsHandler("dl_dr", &currentPresetData.modules.delay.direction);
    setParamsHandler("dl_tp", &currentPresetData.modules.delay_tap);
    setParamsHandler("dl_tl", &currentPresetData.modules.delay_tail);

    setParamsHandler("er_on", &currentPresetData.modules.switches.early_reflections);
    setParamsHandler("er_mx", &currentPresetData.modules.early_reflections.volume);
    setParamsHandler("er_sz", &currentPresetData.modules.early_reflections.type);

    setParamsHandler("rv_on", &currentPresetData.modules.switches.reverb);
    setParamsHandler("rv_tp", &currentPresetData.modules.reverb_type);
    setParamsHandler("rv_mx", &currentPresetData.modules.reverb.mix);
    setParamsHandler("rv_tm", &currentPresetData.modules.reverb.time);
    setParamsHandler("rv_sz", &currentPresetData.modules.reverb.size);
    setParamsHandler("rv_dp", &currentPresetData.modules.reverb.damping);
    setParamsHandler("rv_hp", &currentPresetData.modules.reverb.hpf);
    setParamsHandler("rv_lp", &currentPresetData.modules.reverb.lpf);
    setParamsHandler("rv_dt", &currentPresetData.modules.reverb.detune);
    setParamsHandler("rv_df", &currentPresetData.modules.reverb_diffusion);
    setParamsHandler("rv_pd", &currentPresetData.modules.reverb_predelay);
    setParamsHandler("rv_tl", &currentPresetData.modules.reverb_tail);

    setParamsHandler("tr_on", &currentPresetData.modules.switches.tremolo);
    setParamsHandler("tr_dp", &currentPresetData.modules.tremolo.depth);
    setParamsHandler("tr_rt", &currentPresetData.modules.tremolo.rate);
    setParamsHandler("tr_lm", &currentPresetData.modules.tremolo.lfo);
    setParamsHandler("tr_fm", &currentPresetData.modules.tremolo_lfo_type);
    setParamsHandler("tr_ms", &currentPresetData.modules.tremolo.ms);
    setParamsHandler("tr_tp", &currentPresetData.modules.tremolo_tap);

    //-------------------------System params handlers---------------
    setSysParamsHandler("sys_cab_mode", &currentSystemData.cabSimDisabled);
    setSysParamsHandler("sys_cab_num", &currentSystemData.cabSimConfig);
    setSysParamsHandler("sys_midi_ch", &currentSystemData.midiChannel);
    // setSysParamsHandler("sys_expr_on", &currentSystemData.cabSimDisabled);
    //sys_expr_type

    setSysParamsHandler("sys_expr_cc", &currentSystemData.exprCC);
    setSysParamsHandler("sys_expr_slev", &currentSystemData.storeExprLevel);
    setSysParamsHandler("sys_spdif", &currentSystemData.spdifOutType);
    setSysParamsHandler("sys_tempo", &currentSystemData.tapType);

    // m_tunerControl = settings.tunerExternal & 0x80 ? 1 : 0;
    // m_tunerCC = settings.tunerExternal & 0x7F;

    setSysParamsHandler("sys_time_format", &currentSystemData.timeFormat);
    setSysParamsHandler("sys_swap_conf", &currentSystemData.swapSwitch);
    setSysParamsHandler("sys_tuner_speed", &currentSystemData.tunerSpeed);
    setSysParamsHandler("sys_fsw_speed", &currentSystemData.fswSpeed);

    setSysParamsHandler("meq_on", &currentSystemData.masterEqOn);
    setSysParamsHandler("meq_lg", &currentSystemData.masterEqLow);
    setSysParamsHandler("meq_mg", &currentSystemData.masterEqMid);
    setSysParamsHandler("meq_hg", &currentSystemData.masterEqHigh);

    // "meq_mf"
    // if(count > 0)
    // {
    //     if (count == 2)
    //     {
    //         char *end;
    //         mstEqMidFreq = kgp_sdk_libc::strtol(args[1], &end, 16);
    //     }

    //     msg_console("%s\n", args[1]);
    // }

    // sys_para[System::MASTER_EQ_FREQ_LO] = mstEqMidFreq >> 8;
    // sys_para[System::MASTER_EQ_FREQ_HI] = mstEqMidFreq & 0xFF;

    // rl->AddCommandHandler("midi_map", midi_map_command_handler);
    // m_midiPcMap.clear();

    // fsw

    // rl->AddCommandHandler("vl_at", attenuator_command_handler);
    // rl->AddCommandHandler("vl_ms", master_volume_command_handler);
    // rl->AddCommandHandler("vl_ph", phones_volume_command_handler);
    //-------------------------default preset data------------------
    memset(&defaultPresetData, 0, sizeof(modules_data_fx_t));

    defaultPresetData.modules.cab1.volume = 82;
    defaultPresetData.modules.cab1.pan = 63;
    defaultPresetData.modules.cab2.pan = 63;

    defaultPresetData.modules.eq_gain[0] = 15;
    defaultPresetData.modules.eq_gain[1] = 15;
    defaultPresetData.modules.eq_gain[2] = 15;
    defaultPresetData.modules.eq_gain[3] = 15;
    defaultPresetData.modules.eq_gain[4] = 15;

    defaultPresetData.modules.phaser.mix = 63;
    defaultPresetData.modules.phaser.rate = 49;
    defaultPresetData.modules.phaser.center = 55;

    defaultPresetData.modules.chorus.mix = 63;
    defaultPresetData.modules.chorus.rate = 31;
    defaultPresetData.modules.chorus.width = 74;
    defaultPresetData.modules.chorus.delay = 4;

    defaultPresetData.modules.delay.volumeFirst = 40;
    defaultPresetData.modules.delay.feedback = 63;
    defaultPresetData.modules.delay.panFirst = 63;
    defaultPresetData.modules.delay.volumeSecond = 63;
    defaultPresetData.modules.delay.panSecond = 63;

    defaultPresetData.modules.flanger.mix = 63;
    defaultPresetData.modules.flanger.rate = 31;
    defaultPresetData.modules.flanger.width = 74;

    defaultPresetData.modules.reverb.mix = 30;
    defaultPresetData.modules.reverb.time = 63;
    defaultPresetData.modules.reverb.size = 41;
    defaultPresetData.modules.reverb.damping = 25;

    defaultPresetData.modules.tremolo.depth = 63;
    defaultPresetData.modules.tremolo.rate = 63;

    defaultPresetData.modules.preset_volume = 127;

    defaultPresetData.modules.pa.level = 127;

    defaultPresetData.modules.early_reflections.volume = 63;
    defaultPresetData.modules.early_reflections.type = 5;

    defaultPresetData.modules.preamp.volume = 127;
    defaultPresetData.modules.preamp.low = 63;
    defaultPresetData.modules.preamp.mid = 63;
    defaultPresetData.modules.preamp.high = 63;

    defaultPresetData.modules.compressor.threshold = 50;
    defaultPresetData.modules.compressor.ratio = 127;
    defaultPresetData.modules.compressor.volume = 84;
    defaultPresetData.modules.compressor.knee = 10;

    defaultPresetData.modules.resonance_filter.mix = 127;
    defaultPresetData.modules.resonance_filter.lfo_rate = 47;
    defaultPresetData.modules.resonance_filter.lpf = 55;
    defaultPresetData.modules.resonance_filter.hpf = 52;
    defaultPresetData.modules.resonance_filter.resonance = 85;
    defaultPresetData.modules.resonance_filter.dyn_threshold = 80;
    defaultPresetData.modules.resonance_filter.dyn_attack = 11;
    defaultPresetData.modules.resonance_filter.dyn_release = 66;

    defaultPresetData.modules.bpm_delay = 120;

    //--------------------Init preset------------------
    loadPresetData(currentSystemData.lastPresetNum);
}

void MockCP100fx::writeToDevice(const QByteArray &data)
{
    m_parser.parseNewData(data);
}

void MockCP100fx::newDataRecieved()
{
    m_parser.parseNewData(*m_uartBuffer);

    QMutexLocker locker(m_mutex);
    m_uartBuffer->clear();
}

QByteArray MockCP100fx::intToBa(quint8 val)
{
    QByteArray tempBa = QString().setNum(val, 16).toUtf8();
    if(tempBa.size() == 1) tempBa.push_front("0");
    return tempBa;
}

void MockCP100fx::initFolders()
{
#ifdef Q_OS_ANDROID
    m_basePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/";
#else
    m_basePath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) + "/";
#ifndef Q_OS_IOS
    m_basePath += "AMT/pangaeaCPPA/";
#endif
#endif
    m_basePath += "virtual_CP100FX";

    if(!loadSysParameters())
    {
        memset(&currentSystemData, 0, sizeof(TSystemSettingsFx));

        currentSystemData.expressionType = 1;

        currentSystemData.phonesVolume = 127;
        currentSystemData.masterVolume = 127;
        currentSystemData.attenuator = 4;

        currentSystemData.masterEqOn = 1;

        currentSystemData.midiChannel = 1;
        currentSystemData.midiPcInd = 1;

        currentSystemData.lastPresetNum = 0;
        currentSystemData.tunerExternal = 0x81;

        quint16 defaultMidFreq = 1000;
        currentSystemData.masterEqFreq = qToBigEndian<quint16>(defaultMidFreq);
    }

    QString presetsFolderPath = m_basePath + "/FLASH" + "/PRESETS";
    if(!QDir().exists(presetsFolderPath)) QDir().mkpath(presetsFolderPath);

    QString impulsesPath = m_basePath + "/SD/IMPULSE";
    if(!QDir().exists(impulsesPath)) QDir().mkpath(impulsesPath);

    m_currentDir = impulsesPath;
}

bool MockCP100fx::loadSysParameters()
{
    QFile systemFile(m_basePath + "/FLASH" + "/system.pan");
    if(systemFile.open(QIODevice::ReadOnly))
    {
        QByteArray readedData;
        readedData = systemFile.read(sizeof(TSystemSettingsFx));
        memcpy(&currentSystemData, readedData.data(), sizeof(modules_data_fx_t));
        return true;
    }
    else return false;
}

bool MockCP100fx::saveSysParameters()
{
    QFile systemFile(m_basePath + "/FLASH" + "/system.pan");

    if(systemFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(TSystemSettingsFx)];
        memcpy(rawData, &currentSystemData, sizeof(TSystemSettingsFx));
        systemFile.write(rawData, sizeof(TSystemSettingsFx));

        systemFile.close();
        return true;
    }
    else return false;
}

void MockCP100fx::loadPresetData(quint8 presetNumber)
{
    presetNumber++;

    QString strPresetNumber = QString().setNum(presetNumber);

    if(presetNumber < 10) strPresetNumber.prepend("0");

    QString filePath = m_basePath + "/FLASH" + "/PRESETS/" + strPresetNumber + "_preset.pan";
    QFile presetFile(filePath);

    ir1Name.clear();
    ir2Name.clear();
    ir1Data.clear();
    ir2Data.clear();

    if(presetFile.open(QIODevice::ReadOnly))
    {
        QByteArray readedData;
        readedData = presetFile.read(sizeof(preset_data_fx_t));
        memcpy(&currentPresetData, readedData.data(), sizeof(preset_data_fx_t));
        readedData.clear();

        readedData = presetFile.read(2);
        if(readedData.size() >= 2)
        {
            delayTime = readedData.at(0);
            delayTime = readedData.at(1) << 8;
        }

        ir1Name = presetFile.read(64);
        ir1Data = presetFile.read(PresetFx::CabinetSize);

        if(ir1Data.size() < PresetFx::CabinetSize)
        {
            ir1Data.clear();
            ir1Data.append(0xff);
            ir1Data.append(0xff);
            ir1Data.append(0x7f);

            for(int i=0; i < PresetFx::CabinetSize - 3; i++)
            {
                char nil = 0x00;
                ir1Data.append(nil);
            }
        }

        if(currentSystemData.cabSimConfig == 2)
        {
            ir2Name = presetFile.read(64);
            ir2Data = presetFile.read(PresetFx::CabinetSize);

            if(ir2Data.size() < PresetFx::CabinetSize)
            {
                ir2Data.clear();
                ir2Data.append(0xff);
                ir2Data.append(0xff);
                ir2Data.append(0x7f);

                for(int i=0; i < PresetFx::CabinetSize - 3; i++)
                {
                    char nil = 0x00;
                    ir2Data.append(nil);
                }
            }
        }
        else
        {
            if(presetFile.seek(sizeof(preset_data_fx_t) + 2 + 64 * 2 + PresetFx::CabinetSize * 2)) //presetFile.seek(25760))
            {
                ir1Data += presetFile.read(PresetFx::CabinetSize);
            }
            else
            {
                if(ir1Data.size() < PresetFx::CabinetSize * 2)
                {
                    char nil = 0x00;
                    ir1Data.append(nil);
                }
            }
        }

        presetFile.seek(sizeof(preset_data_fx_t) + 2 + 64 * 2 + PresetFx::CabinetSize * 3); //38048);
        m_lastPath = presetFile.read(255);

        presetFile.close();
    }
    else
    {
        currentPresetData = defaultPresetData;

        memset(currentPresetData.name, 0, 15);
        memcpy(currentPresetData.name, "Preset", 6);

        memset(currentPresetData.comment, 0, 15);
        memcpy(currentPresetData.comment, "Comment", 7);


        for(int i =0; i < ControllersCount; i++)
        {
            currentPresetData.controller[i].src = 0;
            currentPresetData.controller[i].dst = 0;
            currentPresetData.controller[i].minVal = 0;
            currentPresetData.controller[i].maxVal = 127;
        }

        currentPresetData.set = presetNumber + 1;

        delayTime = 500;

        ir1Data.append(0xff);
        ir1Data.append(0xff);
        ir1Data.append(0x7f);

        // if(cab_type == 2)
        // {
        ir2Data.append(0xff);
        ir2Data.append(0xff);
        ir2Data.append(0x7f);
        // }

        for(int i=0; i < PresetFx::CabinetSize - 3; i++)
        {
            char nil = 0x00;
            ir1Data.append(nil);
            ir2Data.append(nil);
        }
    }
}

void MockCP100fx::savePresetData(quint8 presetNumber)
{
    presetNumber++;

    QString strPresetNumber = QString().setNum(presetNumber);

    if(presetNumber < 10) strPresetNumber.prepend("0");

    QString filePath = m_basePath + "/FLASH" + "/PRESETS/" + strPresetNumber + "_preset.pan";
    QFile presetFile(filePath);
    if(presetFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(preset_data_fx_t)];
        memcpy(rawData, &currentPresetData, sizeof(preset_data_fx_t));

        presetFile.write(rawData, sizeof(preset_data_fx_t));

        QByteArray baDelay;
        baDelay.append(qToBigEndian<quint16>(delayTime));
        presetFile.write(baDelay);


        presetFile.write(ir1Data.left(PresetFx::CabinetSize));
        presetFile.write(ir2Data);

        // presetFile.seek(sizeof(preset_data_fx_t) + 2 + PresetFx::CabinetSize * 2); //=25632
        presetFile.seek(25760);
        presetFile.write(ir1Data.right(PresetFx::CabinetSize));

        presetFile.seek(38048);
        presetFile.write(m_lastPath.toUtf8());

        presetFile.close();
    }
}

//===========================================================================================
//  comm handlers
//===========================================================================================
void MockCP100fx::setParamsHandler(QString commStr, quint8 *commPtr)
{
    paramsByteMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP100fx::parametersByteCommHandler, this, _1, _2, _3));
}

void MockCP100fx::setParamsHandler(QString commStr, quint16 *commPtr)
{
    paramsWordMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP100fx::parametersWordCommHandler, this, _1, _2, _3));
}

void MockCP100fx::setSysParamsHandler(QString commStr, quint8 *commPtr)
{
    sysParamsByteMap.insert(commStr, commPtr);
    using namespace std::placeholders;
    m_parser.addCommandHandler(commStr, std::bind(&MockCP100fx::sysParamsByteCommHandler, this, _1, _2, _3));
}

void MockCP100fx::parametersByteCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8* paramPtr = paramsByteMap.value(command);

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        *paramPtr = correctedArgs.toInt(nullptr, 16);
    }
    emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r\n");
}

void MockCP100fx::parametersWordCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint16* paramPtr = paramsWordMap.value(command);

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        *paramPtr = correctedArgs.toInt(nullptr, 16);
    }
    emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r\n");
}

void MockCP100fx::sysParamsByteCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8* paramPtr = sysParamsByteMap.value(command);

    QString strEdit(arguments);
    strEdit.remove('\r');
    QByteArray correctedArgs = strEdit.toUtf8();
    if(!correctedArgs.isEmpty())
    {
        *paramPtr = correctedArgs.toInt(nullptr, 16);
        saveSysParameters();
    }
    emit answerReady(command.toUtf8() + " " + QString().setNum(*paramPtr, 16).toUtf8() + "\r\n");
}

void MockCP100fx::eqgCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> splittedArgs = arguments.split(' ');
    if(splittedArgs.size() > 1)
    {
        quint8 bandNum = QString(splittedArgs.at(0)).toInt(nullptr, 16);
        quint8 value = QString(splittedArgs.at(1)).toInt(nullptr, 16);

        currentPresetData.modules.eq_gain[bandNum] = value;
        emit answerReady(command.toUtf8() + " " + QString().setNum(bandNum, 16).toUtf8() + QString().setNum(value, 16).toUtf8() + "\r\n");
    }
}

void MockCP100fx::eqfCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> splittedArgs = arguments.split(' ');
    if(splittedArgs.size() > 1)
    {
        quint8 bandNum = QString(splittedArgs.at(0)).toInt(nullptr, 16);
        quint8 value = QString(splittedArgs.at(1)).toInt(nullptr, 16);

        currentPresetData.modules.eq_freq[bandNum] = value;
        emit answerReady(command.toUtf8() + " " + QString().setNum(bandNum, 16).toUtf8() + QString().setNum(value, 16).toUtf8() + "\r\n");
    }
}

void MockCP100fx::eqqCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> splittedArgs = arguments.split(' ');
    if(splittedArgs.size() > 1)
    {
        quint8 bandNum = QString(splittedArgs.at(0)).toInt(nullptr, 16);
        quint8 value = QString(splittedArgs.at(1)).toInt(nullptr, 16);

        currentPresetData.modules.eq_q[bandNum] = value;
        emit answerReady(command.toUtf8() + " " + QString().setNum(bandNum, 16).toUtf8() + QString().setNum(value, 16).toUtf8() + "\r\n");
    }
}

void MockCP100fx::amtDevCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r5\nEND\n").toUtf8());
}

void MockCP100fx::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtver\r1.10.00\nEND\n").toUtf8());
}

void MockCP100fx::sysSettingsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray ba = PresetAbstract::dataToBa(&currentSystemData, sizeof(currentSystemData));

    emit answerReady(command.toUtf8() + "\r" + ba + "\n");
}

void MockCP100fx::stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray answer = QString("state").toUtf8();

    if(arguments == "set")
    {
        currentPresetData.modules = PresetFx::charsToPresetData(data);
        answer += " set\r\n";
    }
    else
    {   
        answer.append("\r");
        answer.append(PresetFx::presetDataToChars(currentPresetData.modules));
        answer.append("\n");
    }
    emit answerReady(answer);
}

void MockCP100fx::irCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray answer;
    answer.append("ir ");

    QList<QByteArray> splittedArgs = arguments.split(' ');

    if(splittedArgs.count() > 0)
    {
        quint8 irNumber = splittedArgs.at(0).toInt();
        answer.append(QByteArray::number(irNumber) + "\r");

        if(irNumber == 0) answer.append(ir1Name.toUtf8() + "\n");
        else answer.append(ir2Name.toUtf8() + "\n");
    }

    if(splittedArgs.count() > 2)
    {
        if(splittedArgs.at(1) == "set")
        {

            // TODo set cab and save data from WAV to preset
        }
    }

    emit answerReady(answer);
}

void MockCP100fx::lsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QFileInfoList entryInfoList = m_currentDir.entryInfoList();

    QByteArray answer;
    answer.append("ls\r");

    qDebug() << m_currentDir.absolutePath();
    qDebug() << m_basePath + "/SD/";

    foreach (QFileInfo entry, entryInfoList)
    {
        if(entry.fileName() == ".") continue;

        if(m_currentDir.absolutePath() == m_basePath + "/SD" && entry.fileName() == "..") continue;

        if(entry.isFile()) answer.append("0:");
        else answer.append("1:");

        answer.append(entry.fileName().toUtf8() + "|");
    }
    answer.append("\n");
    emit answerReady(answer);
}

void MockCP100fx::cdCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_currentDir.cd(data);

    QString correctedPath = m_currentDir.absolutePath();
    quint16 pos = correctedPath.indexOf("SD/");
    correctedPath.remove(0, pos +3);

    emit answerReady("cd\r" + correctedPath.toUtf8() + "\n");
}

void MockCP100fx::uploadCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{

}

void MockCP100fx::mkdirCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{

}

void MockCP100fx::renameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{

}

void MockCP100fx::removeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{

}

void MockCP100fx::cntrlsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray answer;
    answer.append(command.toUtf8());

    if(arguments == "set")
    {
        memcpy(&currentPresetData.controller[0].src, data.data(), sizeof(controller_fx_t) * ControllersCount);
        answer += " set\r\n";
    }
    else
    {
        QByteArray buffer;
        for(int i =0; i < ControllersCount; i++)
        {
            buffer.append(intToBa(currentPresetData.controller[i].src));
            buffer.append(intToBa(currentPresetData.controller[i].dst));
            buffer.append(intToBa(currentPresetData.controller[i].minVal));
            buffer.append(intToBa(currentPresetData.controller[i].maxVal));
        }
        answer.append("\r");
        answer.append(buffer);
        answer.append("\n");
    }
    emit answerReady(answer);
}

void MockCP100fx::cntrlsPcCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments.size() > 0)
    {
        currentPresetData.pcOut = QString(arguments.at(0)).toInt(nullptr, 16);
    }

    QString answer = command + QString("\r") + QString("0%1").arg(currentPresetData.pcOut) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP100fx::cntrlsSetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments.size() > 0)
    {
        currentPresetData.set = QString(arguments.at(0)).toInt(nullptr, 16);
    }

    QString answer = command + QString("\r") + QString("0%1").arg(currentPresetData.set) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP100fx::psaveCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    savePresetData(currentSystemData.lastPresetNum);
    emit answerReady(command.toUtf8() + "\r\n");
}

void MockCP100fx::pchangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments.size() > 0)
    {
        bool convRes;
        currentSystemData.lastPresetNum = QString(arguments).toInt(&convRes, 16);
        qDebug() << "Change preset: " << currentSystemData.lastPresetNum;
        loadPresetData(currentSystemData.lastPresetNum);
        emit answerReady(command.toUtf8() + "\r\n");

        saveSysParameters();
    }
    else
    {
        emit answerReady(command.toUtf8() + "\rINCORRECT_ARGUMENT\n");
    }
}

void MockCP100fx::plistCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString answer;
    answer.append(command.toUtf8());

    for(int i=0; i<99; i++)
    {
        answer.append("\r");

        loadPresetData(i);
        answer.append(QString::number(i) + "|");
        answer.append(QString((char*)currentPresetData.name) + "|");
        answer.append(QString((char*)currentPresetData.comment) + "|");
        answer.append(ir1Name + "|");
        answer.append(ir2Name + "|");

        uint8_t enabled[14];
        memcpy(enabled, &currentPresetData.modules.switches, 14);
        for(uint8_t i=0; i<14; i++) answer.append(QByteArray::number(enabled[i]));
    }

    answer.append("\n");
    emit answerReady(answer.toUtf8());

    loadPresetData(currentSystemData.lastPresetNum);
}

void MockCP100fx::pnumCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString answer = QString("pnum\r") + QString::number(currentSystemData.lastPresetNum, 16) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP100fx::pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "set")
    {
        // m_presetName = QString::fromUtf8(data);
        memset(currentPresetData.name, 0, 15);
        memcpy(currentPresetData.name, data.data(), data.size());
    }
    QString answer = "pname\r" + QString((char*)currentPresetData.name) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP100fx::pcommentCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments == "set")
    {
        // m_presetComment = QString::fromUtf8(data);
        memset(currentPresetData.comment, 0, 15);
        memcpy(currentPresetData.comment, data.data(), data.size());
    }
    QString answer = "pcomment\r" + QString((char*)currentPresetData.comment) + "\n";
    emit answerReady(answer.toUtf8());
}
