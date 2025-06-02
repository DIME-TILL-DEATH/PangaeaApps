#include <QDebug>
#include <QStandardPaths>
#include <QDir>

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

    m_parser.addCommandHandler("state", std::bind(&MockCP100fx::stateCommHandler, this, _1, _2, _3));

    //--------------------------params handlers----------------------
    setParamsHandler("rf_on", &currentPresetData.switches.resonance_filter);
    setParamsHandler("rf_mx", &currentPresetData.resonance_filter.mix);
    setParamsHandler("rf_ft", &currentPresetData.resonance_filter.type);
    setParamsHandler("rf_fm", &currentPresetData.resonance_filter.mod);
    setParamsHandler("rf_lo", &currentPresetData.resonance_filter_gen_type);
    setParamsHandler("rf_lf", &currentPresetData.resonance_filter.lpf);
    setParamsHandler("rf_hf", &currentPresetData.resonance_filter.hpf);
    setParamsHandler("rf_rs", &currentPresetData.resonance_filter.resonance);
    setParamsHandler("rf_dt", &currentPresetData.resonance_filter.dyn_threshold);
    setParamsHandler("rf_da", &currentPresetData.resonance_filter.dyn_attack);
    setParamsHandler("rf_dr", &currentPresetData.resonance_filter.dyn_release);
    setParamsHandler("rf_vl", &currentPresetData.resonance_filter.vo);

    setParamsHandler("ng_on", &currentPresetData.switches.gate);
    setParamsHandler("ng_th", &currentPresetData.gate.threshold);
    setParamsHandler("ng_dc", &currentPresetData.gate.decay);
    setParamsHandler("ng_at", &currentPresetData.gate.attack);

    setParamsHandler("cm_on", &currentPresetData.switches.compressor);
    setParamsHandler("cm_th", &currentPresetData.compressor.threshold);
    setParamsHandler("cm_vl", &currentPresetData.compressor.volume);
    setParamsHandler("cm_rt", &currentPresetData.compressor.ratio);
    setParamsHandler("cm_at", &currentPresetData.compressor.attack);
    setParamsHandler("cm_kn", &currentPresetData.compressor.knee);

    // TODO: preamp comm names/diapasons

    setParamsHandler("pa_on", &currentPresetData.switches.amp);
    setParamsHandler("pa_ms", &currentPresetData.pa.master);
    setParamsHandler("pa_lv", &currentPresetData.pa.level);
    setParamsHandler("pa_tp", &currentPresetData.pa.type);

    // TODO: IR, EQ

    setParamsHandler("dl_on", &currentPresetData.switches.delay);
    setParamsHandler("dl_m1", &currentPresetData.delay.volumeFirst);
    setParamsHandler("dl_m2", &currentPresetData.delay.volumeSecond);
    setParamsHandler("dl_p1", &currentPresetData.delay.panFirst);
    setParamsHandler("dl_p2", &currentPresetData.delay.panSecond);
    //TODO: delay time, 16bit. MSB or LSB?
    setParamsHandler("dl_fb", &currentPresetData.delay.feedback);
    setParamsHandler("dl_hp", &currentPresetData.delay.hpf);
    setParamsHandler("dl_lp", &currentPresetData.delay.lpf);
    setParamsHandler("dl_os", &currentPresetData.delay.offset);
    setParamsHandler("dl_md", &currentPresetData.delay.modulation);
    setParamsHandler("dl_rt", &currentPresetData.delay.rate);
    setParamsHandler("dl_m2", &currentPresetData.delay.direction);
    setParamsHandler("dl_dr", &currentPresetData.delay_tap);

    setParamsHandler("ph_on", &currentPresetData.switches.phaser);
    setParamsHandler("ph_mx", &currentPresetData.phaser.mix);
    setParamsHandler("ph_rt", &currentPresetData.phaser.rate);
    setParamsHandler("ph_cn", &currentPresetData.phaser.center);
    setParamsHandler("ph_wd", &currentPresetData.phaser.width);
    setParamsHandler("ph_fb", &currentPresetData.phaser.feedback);
    setParamsHandler("ph_st", &currentPresetData.phaser.type);
    setParamsHandler("ph_hp", &currentPresetData.hpf_phaser);

    setParamsHandler("fl_on", &currentPresetData.switches.flanger);
    setParamsHandler("fl_mx", &currentPresetData.flanger.mix);
    setParamsHandler("fl_lf", &currentPresetData.flanger.lfo);
    setParamsHandler("fl_rt", &currentPresetData.flanger.rate);
    setParamsHandler("fl_wd", &currentPresetData.flanger.width);
    setParamsHandler("fl_dl", &currentPresetData.flanger.delay);
    setParamsHandler("fl_fb", &currentPresetData.flanger.feedback);
    setParamsHandler("fl_hp", &currentPresetData.hpf_flanger);

    setParamsHandler("ch_on", &currentPresetData.switches.chorus);
    setParamsHandler("ch_mx", &currentPresetData.chorus.mix);
    setParamsHandler("ch_rt", &currentPresetData.chorus.rate);
    setParamsHandler("ch_wd", &currentPresetData.chorus.width);
    setParamsHandler("ch_dl", &currentPresetData.chorus.delay);
    setParamsHandler("ch_tp", &currentPresetData.chorus.type);
    setParamsHandler("ch_hp", &currentPresetData.hpf_chorus);

    //TODO ER comm names

    setParamsHandler("rv_on", &currentPresetData.switches.reverb);
    setParamsHandler("rv_mx", &currentPresetData.reverb.mix);
    setParamsHandler("rv_tm", &currentPresetData.reverb.time);
    setParamsHandler("rv_sz", &currentPresetData.reverb.size);
    setParamsHandler("rv_dp", &currentPresetData.reverb.damping);
    setParamsHandler("rv_hp", &currentPresetData.reverb.hpf);
    setParamsHandler("rv_lp", &currentPresetData.reverb.lpf);
    setParamsHandler("rv_dt", &currentPresetData.reverb.detune);

    setParamsHandler("tr_on", &currentPresetData.switches.tremolo);
    setParamsHandler("tr_dp", &currentPresetData.tremolo.depth);
    setParamsHandler("tr_rt", &currentPresetData.tremolo.rate);
    setParamsHandler("tr_ms", &currentPresetData.tremolo.ms);
    setParamsHandler("tr_fm", &currentPresetData.tremolo_lfo_type);
    setParamsHandler("tr_tp", &currentPresetData.tremolo_tap);
    //-------------------------default preset data------------------
    memset(&defaultPresetData, 0, sizeof(preset_data_fx_t));

    defaultPresetData.cab1.volume = 82;
    defaultPresetData.cab1.pan = 63;
    defaultPresetData.cab2.pan = 63;

    defaultPresetData.eq_gain[0] = 15;
    defaultPresetData.eq_gain[1] = 15;
    defaultPresetData.eq_gain[2] = 15;
    defaultPresetData.eq_gain[3] = 15;
    defaultPresetData.eq_gain[4] = 15;

    defaultPresetData.phaser.mix = 63;
    defaultPresetData.phaser.rate = 49;
    defaultPresetData.phaser.center = 55;

    defaultPresetData.chorus.mix = 63;
    defaultPresetData.chorus.rate = 31;
    defaultPresetData.chorus.width = 74;
    defaultPresetData.chorus.delay = 4;

    defaultPresetData.delay.volumeFirst = 40;
    defaultPresetData.delay.feedback = 63;
    defaultPresetData.delay.panFirst = 63;
    defaultPresetData.delay.volumeSecond = 63;
    defaultPresetData.delay.panSecond = 63;

    defaultPresetData.flanger.mix = 63;
    defaultPresetData.flanger.rate = 31;
    defaultPresetData.flanger.width = 74;

    defaultPresetData.reverb.mix = 30;
    defaultPresetData.reverb.time = 63;
    defaultPresetData.reverb.size = 41;
    defaultPresetData.reverb.damping = 25;

    defaultPresetData.tremolo.depth = 63;
    defaultPresetData.tremolo.rate = 63;

    defaultPresetData.preset_volume = 127;

    defaultPresetData.pa.level = 127;

    defaultPresetData.early_reflections.volume = 63;
    defaultPresetData.early_reflections.type = 5;

    defaultPresetData.preamp.volume = 127;
    defaultPresetData.preamp.low = 63;
    defaultPresetData.preamp.mid = 63;
    defaultPresetData.preamp.high = 63;

    defaultPresetData.compressor.threshold = 50;
    defaultPresetData.compressor.ratio = 127;
    defaultPresetData.compressor.volume = 84;
    defaultPresetData.compressor.knee = 10;

    defaultPresetData.resonance_filter.mix = 127;
    defaultPresetData.resonance_filter.rate = 47;
    defaultPresetData.resonance_filter.lpf = 55;
    defaultPresetData.resonance_filter.hpf = 52;
    defaultPresetData.resonance_filter.resonance = 85;
    defaultPresetData.resonance_filter.dyn_threshold = 80;
    defaultPresetData.resonance_filter.dyn_attack = 11;
    defaultPresetData.resonance_filter.dyn_release = 66;
    defaultPresetData.resonance_filter.mix = 127;

    defaultPresetData.bpm_delay = 120;

    //--------------------Init preset------------------
    loadPresetData(1);
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

    QFile systemFile(m_basePath + "/system.pan");
    if(!systemFile.exists())
    {
        saveSysParameters();
    }

    QString presetsFolderPath = m_basePath + "/PRESETS";
    if(!QDir().exists(presetsFolderPath)) QDir().mkpath(presetsFolderPath);
}

bool MockCP100fx::saveSysParameters()
{
    // QFile systemFile(m_basePath + "/system.pan");

    // system_parameters_fx_t sysParameters;
    // memset(&sysParameters, 0, sizeof(system_parameters_fx_t));
    // sysParameters.output_mode = m_outputMode;
    // QString ver("1.04.04");
    // memcpy(sysParameters.firmware_version, ver.data(), ver.size());

    // if(systemFile.open(QIODevice::WriteOnly))
    // {
    //     char rawData[sizeof(system_parameters_fx_t)];
    //     memcpy(rawData, &sysParameters, sizeof(system_parameters_fx_t));
    //     systemFile.write(rawData, sizeof(system_parameters_fx_t));

    //     systemFile.close();
    //     return true;
    // }
    // else return false;
    return true;
}

void MockCP100fx::loadPresetData(quint8 presetNumber)
{
    QString strPresetNumber = QString().setNum(presetNumber);

    if(presetNumber < 10) strPresetNumber.prepend("0");

    QString filePath = m_basePath + "/PRESETS/" + strPresetNumber + "_preset.pan";
    QFile presetFile(filePath);

    if(presetFile.open(QIODevice::ReadOnly))
    {
        presetName = presetFile.read(stringDataSize);
        presetComment = presetFile.read(stringDataSize);

        QByteArray readedData;
        readedData = presetFile.read(sizeof(preset_data_fx_t));
        memcpy(&currentPresetData, readedData.data(), sizeof(preset_data_fx_t));

        readedData.clear();
        readedData = presetFile.read(512); // f_read(&file, control , 512 , &f_size);

        readedData.clear();
        readedData = presetFile.read(2);
        if(readedData.size() >= 2)
        {
            delayTime = readedData.at(0);
            delayTime = readedData.at(1) << 8;
        }

        // f_read(&file, cab_data , 12288 , &f_size);
        // f_read(&file, name_buf , 64 , &f_size);
        // if(cab_type == 2)
        // {
        //     f_read(&file, cab_data1 , 12288 , &f_size);
        //     f_read(&file, name_buf1 , 64 , &f_size);
        // }
        // else {
        //     f_lseek (&file, 25760) ;
        //     if(f_size(&file) <= 25760)memset(cab_data1,0,12288);
        //     else f_read(&file, cab_data1 , 12288 , &f_size);
        // }
        // f_lseek (&file, 38048) ;
        // memset(impulse_path,0,512);
        // char* tmp = new char [_MAX_LFN] ;
        // if ( !tmp ) throw_exeption_catcher("not enother memory");
        // f_read(&file, tmp , _MAX_LFN , &f_size);
        // if ( f_size )
        // {
        //     memcpy(impulse_path,tmp,_MAX_LFN);
        //     FSTask->Object().name = emb_string(tmp) ;
        //     f_read(&file, tmp , _MAX_LFN , &f_size);
        //     memcpy(impulse_path + 255,tmp,_MAX_LFN);
        //     FSTask->Object().startup = emb_string(tmp) ;

        //     emb_string startup = FSTask->Object().name + emb_string("/") +  FSTask->Object().startup ;
        //     if ( FR_OK == f_open(&file, startup.c_str() , FA_READ) )
        //     {
        //         f_close(&file);
        //         FSTask->SendCommand(TFsBrowser::bcStartup);
        //     }
        // }
        // delete tmp ;
        // prog_sym_cur = 0;

        presetFile.close();
    }
    else
    {
        presetName = "Preset";
        presetComment = "Name";
        memcpy(&currentPresetData, &defaultPresetData, sizeof(preset_data_fx_t));
        // memset(control,0,512);
        // for(uint8_t i = 0 ; i < 128 ; i++)control[i*4 + 3] = 127;
        delayTime = 500;

        // name_buf[0] = name_buf1[0] = 0;
        // cab_data[0] = 0xff;
        // cab_data[1] = 0xff;
        // cab_data[2] = 0x7f;
        // if(cab_type == 2)
        // {
        //     cab_data1[0] = 0xff;
        //     cab_data1[1] = 0xff;
        //     cab_data1[2] = 0x7f;
        // }
        // prog_sym_cur = 1;
    }
}

void MockCP100fx::savePresetData(quint8 presetNumber)
{
    QString strPresetNumber = QString().setNum(presetNumber);

    if(presetNumber < 10) strPresetNumber.prepend("0");

    QString filePath = m_basePath + "/PRESETS/" + strPresetNumber + "_preset.pan";
    QFile presetFile(filePath);
    if(presetFile.open(QIODevice::WriteOnly))
    {
        char chPresetName[stringDataSize];
        memset(chPresetName, 0, stringDataSize);
        memcpy(chPresetName, presetName.data(), stringDataSize);
        presetFile.write(chPresetName, stringDataSize);

        char chPresetAuthor[stringDataSize];
        memset(chPresetAuthor, 0, stringDataSize);
        memcpy(chPresetAuthor, presetName.data(), stringDataSize);
        presetFile.write(chPresetAuthor, stringDataSize);

        char rawData[sizeof(preset_data_fx_t)];
        memcpy(rawData, &currentPresetData, sizeof(preset_data_fx_t));

        presetFile.write(rawData, sizeof(preset_data_fx_t));
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


void MockCP100fx::amtDevCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r5\nEND\n").toUtf8());

}

void MockCP100fx::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtver\r1.10.00\nEND\n").toUtf8());
}

void MockCP100fx::stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    QByteArray answer = QString("state").toUtf8();

    if(arguments == "set")
    {
        currentPresetData = PresetFx::charsToPresetData(data);
        answer += " set\r\n";
    }
    else
    {
        answer.append("\r");
        answer.append(PresetFx::presetDataToChars(currentPresetData));
        answer.append("\n");
    }
    emit answerReady(answer);
}
