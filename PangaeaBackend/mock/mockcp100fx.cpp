#include <QDebug>

#include "mockcp100fx.h"

MockCP100fx::MockCP100fx(QMutex *mutex, QByteArray *uartBuffer, QObject *parent)
    : AbstractMockDevice{mutex, uartBuffer, parent}
{
    m_mockDeviceType = MockDeviceType::Mock_CP100FX;

    initFolders();
    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP100fx::amtDevCommHandler, this, _1, _2, _3));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP100fx::amtVerCommHandler, this, _1, _2, _3));

    //--------------------------params handler----------------------

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

}
//===========================================================================================
//  comm handlers
//===========================================================================================

void MockCP100fx::amtDevCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtdev\r5\nEND\n").toUtf8());

}

void MockCP100fx::amtVerCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data)
{
    emit answerReady(QString("amtver\r1.10.00\nEND\n").toUtf8());

}
