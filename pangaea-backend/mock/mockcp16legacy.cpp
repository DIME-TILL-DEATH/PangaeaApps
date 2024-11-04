#include "mockcp16legacy.h"

#include "preset.h"

MockCP16Legacy::MockCP16Legacy(QObject *parent)
    : AbstractMockDevice{parent}
{
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP16Legacy::amtDevCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP16Legacy::amtVerCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("gb", std::bind(&MockCP16Legacy::bankPresetCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("gm", std::bind(&MockCP16Legacy::outputModeCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("gs", std::bind(&MockCP16Legacy::getStateCommHandler, this, std::placeholders::_1));
    m_parser.addCommandHandler("rn", std::bind(&MockCP16Legacy::reqImpulseName, this, std::placeholders::_1));
}

void MockCP16Legacy::amtDevCommHandler(const QByteArray &arguments)
{
    emit answerReady(QString("amtdev\r3\n").toUtf8());
}

void MockCP16Legacy::amtVerCommHandler(const QByteArray &arguments)
{
    emit answerReady(QString("amtver\r1.04.04\n").toUtf8());
}

void MockCP16Legacy::bankPresetCommHandler(const QByteArray &arguments)
{
    emit answerReady(QString("gb\r0000\n").toUtf8());
}

void MockCP16Legacy::outputModeCommHandler(const QByteArray &arguments)
{
    quint8 outputMode = 2;
    QString answer = "gm\r0" + QString().setNum(outputMode) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Legacy::getStateCommHandler(const QByteArray &arguments)
{
    preset_data_legacy_t legacyData;
    quint8 rawData[sizeof(preset_data_legacy_t)];

    legacyData.preset_volume = 20;

    legacyData.amp_on = 1;
    legacyData.amp_slave = 10;
    legacyData.amp_volume = 5;
    legacyData.amp_type = 3;
    legacyData.presence_vol = 15;

    memcpy(rawData, &legacyData, sizeof(preset_data_legacy_t));

    QByteArray baData = QString("gs\r").toUtf8();

    for(int i=0; i < sizeof(preset_data_legacy_t);  i++)
    {
        QByteArray tempBa = QString().setNum(rawData[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baData.append(tempBa);
    }
    baData.append("\n");

    emit answerReady(baData);
}

void MockCP16Legacy::reqImpulseName(const QByteArray &arguments)
{
    QString impulseName = "impulse.wav";
    QByteArray baData = QString("rn\r").toUtf8() + impulseName.toUtf8() + QString("\n").toUtf8();

    emit answerReady(baData);
}
