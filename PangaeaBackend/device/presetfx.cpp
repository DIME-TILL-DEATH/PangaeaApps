#include "presetfx.h"

PresetFx::PresetFx(AbstractDevice *ownerDevice)
    : m_ownerDevice{ownerDevice}
{
    presetVersion = 3;
}

preset_data_fx_t PresetFx::charsToPresetData(const QByteArray &ba)
{
    preset_data_fx_t presetData;
    quint8 rawArr[sizeof(preset_data_fx_t)];
    for(int i = 0; i < ba.size(); i += 2)
    {
        QString chByte = QString(ba.at(i)) + QString(ba.at(i+1));
        rawArr[i/2] = chByte.toInt(nullptr, 16);
    }
    memcpy(&presetData, rawArr, sizeof(preset_data_fx_t));
    return presetData;
}

QByteArray PresetFx::presetDataToChars(const preset_data_fx_t &presetData)
{
    quint8 buffer[sizeof(preset_data_fx_t)];
    memcpy(buffer, &presetData, sizeof(preset_data_fx_t));
    QByteArray baData;

    for(int i=0; i < sizeof(preset_data_fx_t);  i++)
    {
        QByteArray tempBa = QString().setNum(buffer[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baData.append(tempBa);
    }
    return baData;
}
