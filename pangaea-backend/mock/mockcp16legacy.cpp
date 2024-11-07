#include "mockcp16legacy.h"

#include <QStandardPaths>
#include <QDir>

#include "preset.h"

MockCP16Legacy::MockCP16Legacy(QObject *parent)
    : AbstractMockDevice{parent}
{
    initFolders();

    using namespace std::placeholders;
    m_parser.addCommandHandler("amtdev", std::bind(&MockCP16Legacy::amtDevCommHandler, this, _1, _2));
    m_parser.addCommandHandler("amtver", std::bind(&MockCP16Legacy::amtVerCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gb", std::bind(&MockCP16Legacy::bankPresetCommHandler, this, _1, _2));
    m_parser.addCommandHandler("gm", std::bind(&MockCP16Legacy::outputModeCommHandler, this,  _1, _2));
    m_parser.addCommandHandler("gs", std::bind(&MockCP16Legacy::getStateCommHandler, this,  _1, _2));
    m_parser.addCommandHandler("rn", std::bind(&MockCP16Legacy::reqImpulseName, this, _1, _2));
}

void MockCP16Legacy::initFolders()
{
#ifdef Q_OS_ANDROID
    basePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/";
#else
    QString basePath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) + "/AMT/pangaeaCPPA/";

#endif
    if(!QDir(basePath).exists()) QDir().mkpath(basePath);

    basePath += "virtual_CP16";

    if(!QDir(basePath).exists()) QDir().mkpath(basePath);

    QFile systemFile(basePath + "/system.pan");
    if(!systemFile.exists())
    {
        systemFile.open(QIODevice::WriteOnly);

        systemFile.close();
    }

    for(int b = 0; b <4; b++)
    {
        QString bankPath = basePath + "/bank_" + QString().setNum(b);
        if(!QDir().exists(bankPath)) QDir().mkpath(bankPath);

        for(int p=0; p<4; p++)
        {
            QString presetPath = bankPath + "/preset_" + QString().setNum(p);
            if(!QDir().exists(presetPath)) QDir().mkpath(presetPath);

            QFile presetFile(presetPath + "/preset.pan");
            if(!presetFile.exists())
            {
                presetFile.open(QIODevice::WriteOnly);

                preset_data_legacy_t defaultData;
                memset(&defaultData, 0, sizeof(preset_data_legacy_t));

                defaultData.eq_band_vol[0] = 15;
                defaultData.eq_band_vol[1] = 15;
                defaultData.eq_band_vol[2] = 15;
                defaultData.eq_band_vol[3] = 15;
                defaultData.eq_band_vol[4] = 15;

                defaultData.presence_vol = 31;

                defaultData.cab_on = 1;

                defaultData.amp_volume = 16;
                defaultData.amp_slave = 16;
                defaultData.preamp_volume = 25;

                char rawData[sizeof(preset_data_legacy_t)];
                memcpy(rawData, &defaultData, sizeof(preset_data_legacy_t));

                presetFile.write(rawData, sizeof(preset_data_legacy_t));
                presetFile.close();
            }
        }
    }
}

void MockCP16Legacy::amtDevCommHandler(const QString &command, const QByteArray& arguments)
{
    emit answerReady(QString("amtdev\r3\n").toUtf8());
}

void MockCP16Legacy::amtVerCommHandler(const QString &command, const QByteArray& arguments)
{
    emit answerReady(QString("amtver\r1.04.04\n").toUtf8());
}

void MockCP16Legacy::bankPresetCommHandler(const QString &command, const QByteArray& arguments)
{
    emit answerReady(QString("gb\r0000\n").toUtf8());
}

void MockCP16Legacy::outputModeCommHandler(const QString &command, const QByteArray& arguments)
{
    quint8 outputMode = 2;
    QString answer = "gm\r0" + QString().setNum(outputMode) + "\n";
    emit answerReady(answer.toUtf8());
}

void MockCP16Legacy::getStateCommHandler(const QString &command, const QByteArray& arguments)
{
    preset_data_legacy_t legacyData;
    quint8 rawData[sizeof(preset_data_legacy_t)];

    memset(&legacyData, 0, sizeof(preset_data_legacy_t));

    legacyData.preset_volume = 20;

    legacyData.eq_on = 1;

    legacyData.eq_band_vol[0] = 15;
    legacyData.eq_band_vol[1] = 15;
    legacyData.eq_band_vol[2] = 15;
    legacyData.eq_band_vol[3] = 15;
    legacyData.eq_band_vol[4] = 15;

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

void MockCP16Legacy::reqImpulseName(const QString &command, const QByteArray& arguments)
{
    QString impulseName = "impulse.wav";
    QByteArray baData = QString("rn\r").toUtf8() + impulseName.toUtf8() + QString("\n").toUtf8();

    emit answerReady(baData);
}
