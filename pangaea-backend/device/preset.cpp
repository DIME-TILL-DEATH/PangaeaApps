#include <QDebug>
#include <QStandardPaths>

#include "preset.h"

Preset::Preset()
{
    // m_ownerDevice = owner;
    m_rawData.append(86, '0');
}

quint8 Preset::presetNumber() const
{
    return m_presetNumber;
}

void Preset::setBankPreset(quint8 newBankNumber, quint8 newPresetNumber)
{
    m_bankNumber = newBankNumber;
    m_presetNumber = newPresetNumber;
}

bool Preset::importData(QString filePath)
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
            qDebug() << "This is not pangaea preset file";
            return false;
        }

        m_rawData = readPresetChunck(fileData, presetHeaderId.dataId);
        m_impulseName = readPresetChunck(fileData, presetHeaderId.irNameId);
        m_waveData = readPresetChunck(fileData, presetHeaderId.irDataId);
        return true;
    }
    else
    {
        qDebug() << "Can' open file: " << presetFile.fileName();
        return false;
    }
}


QByteArray Preset::readPresetChunck(const QByteArray &fileData, QString &chunkName)
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

bool Preset::exportData()
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
        QByteArray baIrName = m_impulseName.toUtf8();
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

void Preset::writePresetChunk(QByteArray &fileData, QString &chunkName, const QByteArray &chunkData)
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

quint8 Preset::bankNumber() const
{
    return m_bankNumber;
}

const QByteArray &Preset::rawData() const
{
    return m_rawData;
}

void Preset::setRawData(const QByteArray &newRawData)
{
    m_rawData = newRawData;
}

const QString &Preset::impulseName() const
{
    return m_impulseName;
}

void Preset::setImpulseName(const QString &newImpulseName)
{
    m_impulseName = newImpulseName;
}

quint8 Preset::calcPresetFlatIndex(DeviceType deviceType, quint8 bankNumber, quint8 presetNumber)
{
    if(deviceType==DeviceType::legacyCP100 || (deviceType==DeviceType::legacyCP100PA))
        return bankNumber*10 + presetNumber;
    else
        return (bankNumber<<4) + presetNumber;
}

quint8 Preset::getPresetFlatIndex() const
{
    // return calcPresetFlatIndex(m_ownerDevice->deviceType(), m_bankNumber, m_presetNumber);
    return 0;
}

quint8 Preset::getPresetFlatNumber() const
{
    quint16 maxBank;

    // if(m_ownerDevice == nullptr)
    // {
    //     qWarning() << __FUNCTION__ << "Owner device doesn't set. Using default maxBank=16";
    //     maxBank = 4;
    // }
    // else
    // {
    //     maxBank = m_ownerDevice->maxBankCount();
    // }
    return m_bankNumber*maxBank + m_presetNumber;
}

bool Preset::isIrEnabled() const
{
    //TODO переделать на запрос и установку любого параметра из/в rawData
    int result = m_rawData.at(17)-'0';
    return result;
}

void Preset::setIsIrEnabled(bool newIsIrEnabled)
{
    char resCh = newIsIrEnabled ? '1' : '0';
    QByteArray tmpBa;
    tmpBa.append(resCh);

    m_rawData.replace(17, 1, tmpBa);
}

const QByteArray &Preset::waveData() const
{
    return m_waveData;
}

const QString &Preset::pathToExport() const
{
    return m_pathToExport;
}

void Preset::setPathToExport(const QString &newPathToExport)
{
    m_pathToExport = newPathToExport;
}

void Preset::setWaveData(const QByteArray &newWaveData)
{
    m_waveData = newWaveData;
}

void Preset::clearWavData()
{
    m_waveData.clear();
    m_impulseName.clear();
}

quint32 Preset::wavSize() const
{
    return m_waveData.size();
}

// Device *Preset::ownerDevice() const
// {
//     return m_ownerDevice;
// }

// void Preset::setOwnerDevice(Device *newOwnerDevice)
// {
//     m_ownerDevice = newOwnerDevice;
// }

