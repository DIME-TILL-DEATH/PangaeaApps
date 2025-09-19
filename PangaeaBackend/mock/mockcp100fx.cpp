#include <QDebug>
#include <QStandardPaths>

#include <qendian.h>
#include <qthread.h>

#include "irworker.h"
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
    m_parser.addCommandHandler("copy_to", std::bind(&MockCP100fx::copyToCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("erase_preset", std::bind(&MockCP100fx::erasePresetCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("cntrl", std::bind(&MockCP100fx::cntrlCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrls", std::bind(&MockCP100fx::cntrlsCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrl_pc", std::bind(&MockCP100fx::cntrlsPcCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("cntrl_set", std::bind(&MockCP100fx::cntrlsSetCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("sys_tuner_ctrl", std::bind(&MockCP100fx::sysTuner, this, _1, _2, _3));
    m_parser.addCommandHandler("sys_tuner_cc", std::bind(&MockCP100fx::sysTuner, this, _1, _2, _3));

    m_parser.addCommandHandler("sys_expr_on", std::bind(&MockCP100fx::sysExpr, this, _1, _2, _3));
    m_parser.addCommandHandler("sys_expr_type", std::bind(&MockCP100fx::sysExpr, this, _1, _2, _3));

    m_parser.addCommandHandler("pchange", std::bind(&MockCP100fx::pchangeCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("psave", std::bind(&MockCP100fx::psaveCommHandler, this, _1, _2, _3));

    m_parser.addCommandHandler("midi_map", std::bind(&MockCP100fx::midiMap, this, _1, _2, _3));
    m_parser.addCommandHandler("fsw", std::bind(&MockCP100fx::fsw, this, _1, _2, _3));

    //--------------------------params handlers----------------------
    setParamsHandler("vl_pr", &currentPresetData.modules.preset_volume);
    setParamsHandler("vl_pr_control", &currentPresetData.modules.volume_control);

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


    setSysParamsHandler("sys_expr_cc", &currentSystemData.exprCC);
    setSysParamsHandler("sys_expr_slev", &currentSystemData.storeExprLevel);
    setSysParamsHandler("sys_spdif", &currentSystemData.spdifOutType);
    setSysParamsHandler("sys_tempo", &currentSystemData.tapType);

    // setSysParamsHandler("sys_expr_type", &currentSystemData.expressionType);


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
    m_parser.addCommandHandler("meq_mf", std::bind(&MockCP100fx::meqMf, this, _1, _2, _3));

    setSysParamsHandler("vl_ms", &currentSystemData.masterVolume);
    setSysParamsHandler("vl_ph", &currentSystemData.phonesVolume);
    setSysParamsHandler("vl_at", &currentSystemData.attenuator);

    //-------------------------default preset data------------------
    memset(&defaultPresetData, 0, sizeof(preset_data_fx_t));

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
    loadPresetData(currentSystemData.lastPresetNum, currentPresetData);
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

void MockCP100fx::loadPresetData(quint8 presetNumber, preset_data_fx_t &presetData)
{
    presetNumber++;

    QString strPresetNumber = QString().setNum(presetNumber);

    if(presetNumber < 10) strPresetNumber.prepend("0");

    QString filePath = m_basePath + "/FLASH" + "/PRESETS/" + strPresetNumber + "_preset.pan";
    QFile presetFile(filePath);

    if(presetFile.open(QIODevice::ReadOnly))
    {
        memset(&presetData, 0, sizeof(preset_data_fx_t));
        // If ir data in file empty (file size < CabSize * 3)
        presetData.ir1Data[0] = 0xff;
        presetData.ir1Data[1] = 0xff;
        presetData.ir1Data[2] = 0x7f;

        presetData.ir1Data[0] = 0xff;
        presetData.ir1Data[1] = 0xff;
        presetData.ir1Data[2] = 0x7f;

        QByteArray readedData;
        readedData = presetFile.read(sizeof(preset_data_fx_t));
        memcpy(&presetData, readedData.data(), sizeof(preset_data_fx_t));

        presetFile.close();
    }
    else
    {
        setDefaultPresetData(presetNumber);
    }
}

void MockCP100fx::savePresetData(quint8 presetNumber, const preset_data_fx_t& presetData)
{
    presetNumber++;

    QString strPresetNumber = QString().setNum(presetNumber);

    if(presetNumber < 10) strPresetNumber.prepend("0");

    QString filePath = m_basePath + "/FLASH" + "/PRESETS/" + strPresetNumber + "_preset.pan";
    QFile presetFile(filePath);
    if(presetFile.open(QIODevice::WriteOnly))
    {
        char rawData[sizeof(preset_data_fx_t)];
        memcpy(rawData, &presetData, sizeof(preset_data_fx_t));

        presetFile.write(rawData, sizeof(preset_data_fx_t));

        presetFile.close();
    }
}

void MockCP100fx::setDefaultPresetData(quint8 presetNumber)
{
    memset(&currentPresetData, 0, sizeof(preset_data_fx_t));

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

    currentPresetData.modules.delay_time = 500;

    currentPresetData.ir1Data[0] = 0xff;
    currentPresetData.ir1Data[1] = 0xff;
    currentPresetData.ir1Data[2] = 0x7f;


    currentPresetData.ir2Data[0] = 0xff;
    currentPresetData.ir2Data[1] = 0xff;
    currentPresetData.ir2Data[2] = 0x7f;
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
        emit answerReady(command.toUtf8() + " " + QString().setNum(bandNum, 16).toUtf8() + " " + QString().setNum(value, 16).toUtf8() + "\r\n");
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
        emit answerReady(command.toUtf8() + " " + QString().setNum(bandNum, 16).toUtf8() + " " + QString().setNum(value, 16).toUtf8() + "\r\n");
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
        emit answerReady(command.toUtf8() + " " + QString().setNum(bandNum, 16).toUtf8() + " " + QString().setNum(value, 16).toUtf8() + "\r\n");
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
    answer.append(command.toUtf8() + " ");

    QList<QByteArray> splittedArgs = arguments.split(' ');
    QByteArray irNameBa;

    if(splittedArgs.count() == 1)
    {
        quint8 irNumber = splittedArgs.at(0).toInt(nullptr, 16);
        answer.append(QByteArray::number(irNumber) + "\r");

        if(irNumber == 0) irNameBa.append(currentPresetData.ir1Name, 63);
        else irNameBa.append(currentPresetData.ir2Name, 63);

    }

    if(splittedArgs.count() == 2)
    {
        if(splittedArgs.at(1) == "set")
        {
            quint8 irNumber = splittedArgs.at(0).toInt(nullptr, 16);
            answer.append(QByteArray::number(irNumber) + " set");

            QString fileName = data;
            fileName.remove("\r");

            QString srcFilePath = m_currentDir.absolutePath() + "/" + fileName;

            stWavHeader wavHead = IRWorker::getFormatWav(srcFilePath);

            if((wavHead.sampleRate == 48000) && (wavHead.bitsPerSample == 24) && (wavHead.numChannels == 1))
            {

                QFile file(srcFilePath);
                QByteArray fileData;
                if(file.open(QIODevice::ReadOnly))
                {
                    fileData = file.read(4096 * 3 + 44);
                    fileData = fileData.mid(44, 4096 * 3);

                    file.close();
                }

                irNameBa = fileName.left(63).toUtf8();

                if(irNumber == 0)
                {
                    memcpy(currentPresetData.ir1Name, irNameBa.data(), irNameBa.size());
                    currentPresetData.ir1NameLength = irNameBa.size();
                    memcpy(currentPresetData.ir1Data, fileData.data(), fileData.size());
                }
                else
                {
                    memcpy(currentPresetData.ir2Name, irNameBa.data(), irNameBa.size());
                    currentPresetData.ir2NameLength = irNameBa.size();
                    memcpy(currentPresetData.ir2Data, fileData.data(), fileData.size());
                }
            }
        }
    }

    answer.append("\r" + irNameBa + "\n");
    emit answerReady(answer);
}

void MockCP100fx::lsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QFileInfoList entryInfoList = m_currentDir.entryInfoList();

    QByteArray answer;
    answer.append("ls\r");

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
    if(arguments == "start")
    {
        QByteArray answer;

        QString fullPath = m_currentDir.path() + "/" + data;
        uploadingIrFile.setFileName(fullPath);

        if(uploadingIrFile.open(QIODevice::WriteOnly))
        {
            uploadingIrFile.close();
            answer = "upload request_part\r\n";
        }
        else
        {
            answer = "upload error\rCANNOT_CREATE_FILE\n";
        }

        emit answerReady(answer);
    }

    QStringList argsList = QString(arguments).split(" ");
    if(argsList.size() > 1)
    {
        if(argsList.at(0) == "part")
        {
            quint16 partSize;
            if(argsList.size() > 1)
                partSize = argsList.at(1).toInt();

            while(m_uartBuffer->size() < partSize)
            {
                QThread::sleep(100);
            }
            QMutexLocker locker(m_mutex);

            QByteArray recievedPart = m_uartBuffer->right(partSize+1);
            recievedPart.chop(1); // remove \n
            m_uartBuffer->clear();

            if(uploadingIrFile.open(QIODevice::Append))
            {
                uploadingIrFile.write(recievedPart);
                uploadingIrFile.close();
                emit answerReady("upload request_part\r\n");
            }
        }
    }
}

void MockCP100fx::mkdirCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    m_currentDir.mkdir(data);
    QThread::yieldCurrentThread();
    emit answerReady(command.toUtf8() + "\r" + data + "\n");
}

void MockCP100fx::renameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> splittedArgs = data.split('\r');

    if(splittedArgs.count() == 2)
    {
        QString srcObjPath = m_currentDir.absolutePath() + "/" + splittedArgs.at(0);

        QFile file(srcObjPath);

        file.rename(splittedArgs.at(1));

        emit answerReady(command.toUtf8() + "\r" + splittedArgs.at(0) + "\r" +  splittedArgs.at(1) + "\n");
    }
    else
    {
        emit answerReady(command.toUtf8() + "\rINCORRECT_ARGUMENTS\n");
    }
}

void MockCP100fx::removeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QString objName = data;
    objName.remove("\r");

    QString objPath = m_currentDir.absolutePath() + "/" + objName;
    QFileInfo fileInfo(objPath);

    if(fileInfo.isFile())
    {
        QFile file(objPath);
        file.remove(objPath);
    }
    else if(fileInfo.isDir())
    {
        QDir dir(objPath);
        dir.removeRecursively();
    }
    emit answerReady(command.toUtf8() + "\r" + data + "\n");
}

void MockCP100fx::copyToCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 targetPresetNum = arguments.toInt(nullptr, 16);

    qDebug() << data.size();

    if(data.size() < 18) return;

    preset_data_fx_t targetPreset;

    if(data.at(0) == '1') memcpy(targetPreset.name, currentPresetData.name, 15);
    if(data.at(1) == '1') memcpy(targetPreset.comment, currentPresetData.comment, 15);
    if(data.at(2) == '1') memcpy(targetPreset.controller, currentPresetData.controller, 32);

    if(data.at(3) == '1')
    {
        targetPreset.modules.switches.resonance_filter = currentPresetData.modules.switches.resonance_filter;
        targetPreset.modules.resonance_filter_gen_type = currentPresetData.modules.resonance_filter_gen_type;
        memcpy(&targetPreset.modules.resonance_filter, &currentPresetData.modules.resonance_filter, sizeof(resonance_filter_fx_t));
    }

    if(data.at(4) == '1')
    {
        targetPreset.modules.switches.gate = currentPresetData.modules.switches.gate;
        memcpy(&targetPreset.modules.gate, &currentPresetData.modules.gate, sizeof(gate_fx_t));
    }

    if(data.at(5) == '1')
    {
        targetPreset.modules.switches.compressor = currentPresetData.modules.switches.compressor;
        memcpy(&targetPreset.modules.compressor, &currentPresetData.modules.compressor, sizeof(compressor_fx_t));
    }

    if(data.at(6) == '1')
    {
        targetPreset.modules.switches.preamp = currentPresetData.modules.switches.preamp;
        memcpy(&targetPreset.modules.preamp, &currentPresetData.modules.preamp, sizeof(preamp_fx_t));
    }

    if(data.at(7) == '1')
    {
        targetPreset.modules.switches.amp = currentPresetData.modules.switches.amp;
        targetPreset.modules.presence = currentPresetData.modules.presence;
        memcpy(&targetPreset.modules.pa, &currentPresetData.modules.pa, sizeof(pa_fx_t));
    }

    if(data.at(8) == '1')
    {
        targetPreset.modules.switches.cab = currentPresetData.modules.switches.cab;
        memcpy(&targetPreset.modules.cab1, &currentPresetData.modules.cab1, sizeof(cab_fx_t));
        memcpy(&targetPreset.modules.cab2, &currentPresetData.modules.cab2, sizeof(cab_fx_t));

        memcpy(&targetPreset.ir1Data, currentPresetData.ir1Data, 4096*3);
        memcpy(&targetPreset.ir2Data, currentPresetData.ir2Data, 4096*3);
        memcpy(&targetPreset.irAuxData, currentPresetData.irAuxData, 4096*3);

        targetPreset.ir1NameLength = currentPresetData.ir1NameLength;
        memcpy(&targetPreset.ir1Name, currentPresetData.ir1Name, 63);

        targetPreset.ir2NameLength = currentPresetData.ir2NameLength;
        memcpy(&targetPreset.ir2Name, currentPresetData.ir2Name, 63);

        targetPreset.ir2NameLength = currentPresetData.ir2NameLength;
    }

    if(data.at(9) == '1')
    {
        targetPreset.modules.switches.eq = currentPresetData.modules.switches.eq;
        targetPreset.modules.eq_pre_post = currentPresetData.modules.eq_pre_post;
        memcpy(&targetPreset.modules.eq_freq, &currentPresetData.modules.eq_freq, sizeof(5));
        memcpy(&targetPreset.modules.eq_gain, &currentPresetData.modules.eq_gain, sizeof(5));
        memcpy(&targetPreset.modules.eq_q, &currentPresetData.modules.eq_q, sizeof(5));
    }

    if(data.at(10) == '1')
    {
        targetPreset.modules.switches.flanger = currentPresetData.modules.switches.flanger;
        targetPreset.modules.flanger_pre_post = currentPresetData.modules.flanger_pre_post;
        targetPreset.modules.hpf_flanger = currentPresetData.modules.hpf_flanger;
        memcpy(&targetPreset.modules.flanger, &currentPresetData.modules.flanger, sizeof(flanger_fx_t));
    }

    if(data.at(11) == '1')
    {
        targetPreset.modules.switches.phaser = currentPresetData.modules.switches.phaser;
        targetPreset.modules.phaser_pre_post = currentPresetData.modules.phaser_pre_post;
        targetPreset.modules.hpf_phaser = currentPresetData.modules.hpf_phaser;
        memcpy(&targetPreset.modules.phaser, &currentPresetData.modules.phaser, sizeof(phaser_fx_t));
    }

    if(data.at(12) == '1')
    {
        targetPreset.modules.switches.chorus = currentPresetData.modules.switches.chorus;
        targetPreset.modules.hpf_chorus = currentPresetData.modules.hpf_chorus;
        memcpy(&targetPreset.modules.chorus, &currentPresetData.modules.chorus, sizeof(chorus_fx_t));
    }

    if(data.at(13) == '1')
    {
        targetPreset.modules.switches.delay = currentPresetData.modules.switches.delay;
        targetPreset.modules.delay_time = currentPresetData.modules.delay_time;
        targetPreset.modules.delay_tap = currentPresetData.modules.delay_tap;
        targetPreset.modules.delay_tail = currentPresetData.modules.delay_tail;
        memcpy(&targetPreset.modules.delay, &currentPresetData.modules.delay, sizeof(delay_fx_t));
    }

    if(data.at(14) == '1')
    {
        targetPreset.modules.switches.early_reflections = currentPresetData.modules.switches.early_reflections;
        memcpy(&targetPreset.modules.early_reflections, &currentPresetData.modules.early_reflections, sizeof(early_fx_t));
    }

    if(data.at(15) == '1')
    {
        targetPreset.modules.switches.reverb = currentPresetData.modules.switches.reverb;
        targetPreset.modules.reverb_diffusion = currentPresetData.modules.reverb_diffusion;
        targetPreset.modules.reverb_predelay = currentPresetData.modules.reverb_predelay;
        targetPreset.modules.reverb_tail = currentPresetData.modules.reverb_tail;
        targetPreset.modules.reverb_type = currentPresetData.modules.reverb_type;
        memcpy(&targetPreset.modules.reverb, &currentPresetData.modules.reverb, sizeof(reverb_fx_t));
    }

    if(data.at(16) == '1')
    {
        targetPreset.modules.switches.tremolo = currentPresetData.modules.switches.tremolo;
        targetPreset.modules.tremolo_lfo_type = currentPresetData.modules.tremolo_lfo_type;
        targetPreset.modules.tremolo_tap = currentPresetData.modules.tremolo_tap;
        memcpy(&targetPreset.modules.tremolo, &currentPresetData.modules.tremolo, sizeof(tremolo_fx_t));
    }

    if(data.at(17) == '1') targetPreset.modules.preset_volume = currentPresetData.modules.preset_volume;

    savePresetData(targetPresetNum, targetPreset);

    emit answerReady(command.toUtf8() + "\r\n");
}

void MockCP100fx::erasePresetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    setDefaultPresetData(currentSystemData.lastPresetNum);
    savePresetData(currentSystemData.lastPresetNum, currentPresetData);
    emit answerReady(command.toUtf8() + "\r\n");
}

void MockCP100fx::cntrlCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> separatedArgs = arguments.split(' ');
    QString answer = command + "\r";

    if(separatedArgs.count() > 2)
    {
        quint8 cntrlNum = separatedArgs.at(0).toInt(nullptr, 16);
        QString param = separatedArgs.at(1);
        quint8 value = separatedArgs.at(2).toInt(nullptr, 16);

        if(param == "dst")
        {
            currentPresetData.controller[cntrlNum].dst = value;
        }

        if(param == "src")
        {
            currentPresetData.controller[cntrlNum].src = value;
        }

        if(param == "min")
        {
            currentPresetData.controller[cntrlNum].minVal = value;
        }

        if(param == "max")
        {
            currentPresetData.controller[cntrlNum].maxVal = value;
        }

        answer.append(QByteArray::number(cntrlNum, 16) + "\r" + param.toUtf8() + "\r" + QByteArray::number(value, 16));
    }
    else
    {
        answer.append("INCORRECT_ARGS");
    }
    answer.append("\n");
    emit answerReady(answer.toUtf8());
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

void MockCP100fx::sysExpr(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 val = arguments.toInt(nullptr, 16);

    if(command == "sys_expr_on")
    {
        if(!val) currentSystemData.expressionType &= 0x7f;
        else currentSystemData.expressionType |= 0x80;
    }

    if(command == "sys_expr_type")
    {
        currentSystemData.expressionType = (val & 0x7F) | (currentSystemData.expressionType & 0x80);
    }

    saveSysParameters();
    emit answerReady(command.toUtf8() + " " + arguments + "\r\n");
}

void MockCP100fx::sysTuner(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint8 val = arguments.toInt(nullptr, 16);

    if(command == "sys_tuner_ctrl")
    {
        if(!val) currentSystemData.tunerExternal &= 0x7f;
        else currentSystemData.tunerExternal |= 0x80;
    }

    if(command == "sys_tuner_cc")
    {
        currentSystemData.tunerExternal = (val & 0x7F) | (currentSystemData.tunerExternal & 0x80);
    }

    saveSysParameters();
    emit answerReady(command.toUtf8() + " " + arguments + "\r\n");
}

void MockCP100fx::psaveCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    savePresetData(currentSystemData.lastPresetNum, currentPresetData);
    emit answerReady(command.toUtf8() + "\r\n");
}

void MockCP100fx::pchangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    if(arguments.size() > 0)
    {
        bool convRes;
        currentSystemData.lastPresetNum = QString(arguments).toInt(&convRes, 16);
        qDebug() << "Change preset: " << currentSystemData.lastPresetNum;
        loadPresetData(currentSystemData.lastPresetNum, currentPresetData);
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

        preset_data_fx_t presetData;

        loadPresetData(i, presetData);
        answer.append(QString::number(i) + "|");

        QString name((char*)presetData.name);
        name.remove("//r//n|");
        answer.append(name + "|");

        QString comment((char*)presetData.comment);
        comment.remove("//r//n|");
        answer.append(comment + "|");

        QString ir1NameStr(presetData.ir1Name);
        QString ir2NameStr(presetData.ir2Name);

        ir1NameStr.remove("//r//n|");
        ir2NameStr.remove("//r//n|");
        answer.append(ir1NameStr.toUtf8() + "|");
        answer.append(ir2NameStr.toUtf8() + "|");
        // answer.append("||");

        uint8_t enabled[14];
        memcpy(enabled, &currentPresetData.modules.switches, 14);
        for(uint8_t i=0; i<14; i++) answer.append(QByteArray::number(enabled[i]));
    }

    answer.append("\n");
    emit answerReady(answer.toUtf8());

    loadPresetData(currentSystemData.lastPresetNum, currentPresetData);
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

void MockCP100fx::meqMf(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    quint16 val = arguments.toInt(nullptr, 16);

    currentSystemData.masterEqFreq = qToBigEndian(val);
    saveSysParameters();

    emit answerReady(command.toUtf8() + " " + arguments + "\r\n");

}

void MockCP100fx::midiMap(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> separatedArgs = arguments.split(' ');

    if(separatedArgs.count() > 1)
    {
        quint8 num = separatedArgs.at(0).toInt(nullptr, 16);
        quint8 val = separatedArgs.at(1).toInt(nullptr, 16);

        currentSystemData.midiMap[num] = val;

        saveSysParameters();

        emit answerReady((QString("midi_map\r%1\r%2\n").arg(num, 2, 16, QChar('0')).arg(val, 2, 16, QChar('0'))).toUtf8());
    }
}

void MockCP100fx::fsw(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QList<QByteArray> separatedArgs = arguments.split(' ');
    QString answer = command + "\r";

    if(separatedArgs.count() > 2)
    {
        quint8 fswNum = separatedArgs.at(0).toInt(nullptr, 16);
        QString param = separatedArgs.at(1);
        quint8 value = separatedArgs.at(2).toInt(nullptr, 16);

        answer.append(QByteArray::number(fswNum, 16) + "\r" + param.toUtf8() + "\r" + QByteArray::number(value, 16));

        if(param == "mode")
        {
            currentSystemData.fswMode[fswNum] = value;
        }

        if(param == "ptype")
        {
            currentSystemData.fswPressType[fswNum] = value;
        }

        if(param == "htype")
        {
            currentSystemData.fswHoldType[fswNum] = value;
        }

        if(param == "cpressnum")
        {
            currentSystemData.fswControlPressCc[fswNum] = value;
        }

        if(param == "choldnum")
        {
            currentSystemData.fswControlHoldCc[fswNum] = value;
        }

        if(param == "ppressnum")
        {
            if(separatedArgs.count() > 3)
            {
                quint8 presetNum = separatedArgs.at(3).toInt(nullptr, 16);
                currentSystemData.fswPressPreset[fswNum][value] = presetNum;

                answer.append("\r" + QByteArray::number(presetNum, 16));
            }
        }

        if(param == "pholdnum")
        {
            if(separatedArgs.count() > 3)
            {
                quint8 presetNum = separatedArgs.at(3).toInt(nullptr, 16);
                currentSystemData.fswHoldPreset[fswNum][value] = presetNum;

                answer.append("\r" + QByteArray::number(presetNum, 16));
            }
        }

        saveSysParameters();
    }
    else
    {
        answer.append("INCORRECT_ARGS");
    }
    answer.append("\n");
    emit answerReady(answer.toUtf8());
}
