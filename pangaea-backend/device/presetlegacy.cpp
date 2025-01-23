#include <QDebug>
#include <QStandardPaths>

#include "presetlegacy.h"

#include "abstractdevice.h"
#include "hardwarepreset.h"

PresetLegacy::PresetLegacy(AbstractDevice *owner)
{
    m_ownerDevice = owner;
    m_rawData.append(sizeof(preset_data_legacy_t) * 2, '0');
}

PresetAbstract& PresetLegacy::operator=(const PresetAbstract& preset)
{
    // *this = preset;
    if(&preset != this)
    {
        const PresetLegacy* legacyPreset = dynamic_cast<const PresetLegacy*>(&preset);
        if(legacyPreset)
        {
            m_irName = legacyPreset->irName();
            m_bankNumber = legacyPreset->m_bankNumber;
            m_presetNumber = legacyPreset->m_presetNumber;
            m_ownerDevice = legacyPreset->m_ownerDevice;
            m_pathToExport = legacyPreset->m_pathToExport;
            m_rawData = legacyPreset->m_rawData;
            m_waveData = legacyPreset->m_waveData;
        }
    }
    return *this;
}

bool PresetLegacy::importData(QString filePath)
{
    QFile presetFile(filePath);

    if(presetFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Importing preset from:" << presetFile.fileName();

        QByteArray fileData = presetFile.readAll();
        presetFile.close();

        qint32 pos;
        pos = fileData.indexOf(QString("PANGAEA_PRESET").toUtf8());

        if(pos == -1)
        {
            qWarning() << "This is not pangaea preset file";
            return false;
        }

        pos = fileData.indexOf(presetHeaderId.versionId.toUtf8());
        quint8 importedPresetVersion = fileData.at(pos+presetHeaderId.versionId.size());
        if(importedPresetVersion == presetVersion)
        {
            m_rawData = readPresetChunck(fileData, presetHeaderId.dataId);
            m_irName = readPresetChunck(fileData, presetHeaderId.irNameId);
            m_waveData = readPresetChunck(fileData, presetHeaderId.irDataId);
            return true;
        }
        else
        {
            qWarning() << "Pangaea preset version is too high";
            return false;
        }
    }
    else
    {
        qWarning() << "Can' open file: " << presetFile.fileName();
        return false;
    }
}

QByteArray PresetLegacy::readPresetChunck(const QByteArray &fileData, QString &chunkName)
{
    qint32 pos = 0;

    QByteArray baChunkIdSize = QString(chunkName + "_SIZE").toUtf8();
    pos = fileData.indexOf(baChunkIdSize);

    if(pos == -1)
    {
        qDebug() << "Chunk size of '" << chunkName << "' not found!";
        return QByteArray();
    }
    pos += baChunkIdSize.size();

    quint32 chunkSize = fileData.mid(pos, sizeof(quint32)).toHex().toInt(nullptr, 16);

    QByteArray baChunkId = QString(chunkName + "_CHUNK").toUtf8();
    pos = fileData.indexOf(baChunkId);
    if(pos == -1)
    {
        qDebug() << "Chunk '" << chunkName << "' not found!";
        return QByteArray();
    }
    pos += baChunkId.size();
    return fileData.mid(pos, chunkSize);
}

bool PresetLegacy::exportData()
{
    QFile presetFile(m_pathToExport);
    qDebug() << "Choosen path:" << m_pathToExport;

    if(presetFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Exporting preset to:" << presetFile.fileName();

        QByteArray fileData;

        fileData.clear();
        fileData.append(presetHeaderId.header.toUtf8());
        fileData.append(presetHeaderId.versionId.toUtf8());
        fileData.append(presetVersion);

        writePresetChunk(fileData, presetHeaderId.dataId, m_rawData);
        QByteArray baIrName = m_irName.toUtf8();
        writePresetChunk(fileData, presetHeaderId.irNameId, baIrName);
        writePresetChunk(fileData, presetHeaderId.irDataId, m_waveData);

        presetFile.write(fileData);
        presetFile.close();
        return true;
    }
    else
    {
        qDebug() << "Can' open file: " << presetFile.fileName();
        return false;
    }
}

void PresetLegacy::writePresetChunk(QByteArray &fileData, QString &chunkName, const QByteArray &chunkData)
{
    QByteArray baChunkIdSize = QString(chunkName + "_SIZE").toUtf8();
    QByteArray baChunkId = QString(chunkName + "_CHUNK").toUtf8();

    fileData.append(baChunkIdSize);

    quint32 chunkSize = chunkData.size();
    QByteArray baChunkSize(reinterpret_cast<const char*>(&chunkSize), sizeof(quint32));
    std::reverse(baChunkSize.begin(), baChunkSize.end());
    fileData.append(baChunkSize);

    fileData.append(baChunkId);
    fileData.append(chunkData);
}

quint8 PresetLegacy::calcPresetFlatIndex(DeviceType deviceType, quint8 bankNumber, quint8 presetNumber)
{
    if(deviceType==DeviceType::LEGACY_CP100 || (deviceType==DeviceType::LEGACY_CP100PA))
        return bankNumber*10 + presetNumber;
    else
        return (bankNumber<<4) + presetNumber;
}

// quint8 PresetLegacy::getPresetFlatIndex() const
// {
//     // return calcPresetFlatIndex(m_ownerDevice->deviceType(), m_bankNumber, m_presetNumber);
//     return 0;
// }

quint8 PresetLegacy::getPresetFlatNumber() const
{
    quint16 maxBank;

    if(m_ownerDevice == nullptr)
    {
        qWarning() << __FUNCTION__ << "Owner device doesn't set. Using default maxBank=16";
        maxBank = 4;
    }
    else
    {
        maxBank = m_ownerDevice->maxBankCount();
    }
    return m_bankNumber*maxBank + m_presetNumber;
}

bool PresetLegacy::isIrEnabled() const
{
    int result = m_rawData.at(17)-'0';
    return result;
}

void PresetLegacy::setIsIrEnabled(bool newIsIrEnabled)
{
    char resCh = newIsIrEnabled ? '1' : '0';
    QByteArray tmpBa;
    tmpBa.append(resCh);
    m_rawData.replace(17, 1, tmpBa);
}

void PresetLegacy::clearWavData()
{
    m_waveData.clear();
    m_irName.clear();
}

void PresetLegacy::setPathToExport(const QString &newPathToExport)
{
    m_pathToExport = newPathToExport;
}

void PresetLegacy::setWaveData(const QByteArray &newWaveData)
{
    m_waveData = newWaveData;
}

void PresetLegacy::setRawData(const QByteArray &newRawData)
{
    m_rawData = newRawData;
}
