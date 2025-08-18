#include <QDebug>

#include "presetabstract.h"

PresetAbstract::PresetAbstract() {}

PresetAbstract &PresetAbstract::operator=(const PresetAbstract &preset)
{
    m_bankNumber = preset.m_bankNumber;
    m_presetNumber = preset.m_presetNumber;
    return *this;
}

void PresetAbstract::setBankPreset(quint8 newBankNumber, quint8 newPresetNumber)
{
    m_bankNumber = newBankNumber;
    m_presetNumber = newPresetNumber;
}

QByteArray PresetAbstract::dataToBa(void* data_ptr, const quint32 size)
{
    quint8* data = new quint8[size];
    memcpy(data, data_ptr, size);

    QByteArray baData;
    for(int i=0; i < size;  i++)
    {
        QByteArray tempBa = QString().setNum(data[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baData.append(tempBa);
    }

    delete[] data;

    return baData;
}

