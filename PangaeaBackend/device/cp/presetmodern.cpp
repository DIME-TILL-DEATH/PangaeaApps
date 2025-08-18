#include "presetmodern.h"

#include <QDebug>
#include <QStandardPaths>

#include "abstractdevice.h"

PresetModern::PresetModern(AbstractDevice *owner)
    : m_ownerDevice{owner}
{
    presetVersion = 2;
}

PresetModern::~PresetModern()
{

}

PresetAbstract& PresetModern::operator=(const PresetAbstract& preset)
{
    // *this = preset;
    if(&preset != this)
    {
        const PresetModern* modernPreset = dynamic_cast<const PresetModern*>(&preset);
        if(modernPreset)
        {
            m_bankNumber = modernPreset->m_bankNumber;
            m_presetNumber = modernPreset->m_presetNumber;
            presetData = modernPreset->presetData;
            irFile = modernPreset->irFile;
            m_ownerDevice = modernPreset->m_ownerDevice;
            m_presetName = modernPreset->m_presetName;
        }
    }
    return *this;
}

preset_data_cpmodern_t PresetModern::charsToPresetData(const QByteArray &ba)
{
    preset_data_cpmodern_t presetData;
    quint8 rawArr[sizeof(preset_data_cpmodern_t)];
    for(int i = 0; i < ba.size(); i += 2)
    {
        QString chByte = QString(ba.at(i)) + QString(ba.at(i+1));
        rawArr[i/2] = chByte.toInt(nullptr, 16);
    }
    memcpy(&presetData, rawArr, sizeof(preset_data_cpmodern_t));
    return presetData;
}

QByteArray PresetModern::presetDataToChars(const preset_data_cpmodern_t &presetData)
{
    quint8 buffer[sizeof(preset_data_cpmodern_t)];
    memcpy(buffer, &presetData, sizeof(preset_data_cpmodern_t));
    QByteArray baData;

    for(int i=0; i < sizeof(preset_data_cpmodern_t);  i++)
    {
        QByteArray tempBa = QString().setNum(buffer[i], 16).toUtf8();

        if(tempBa.size() == 1) tempBa.push_front("0");
        baData.append(tempBa);
    }
    return baData;
}

bool PresetModern::exportData(const QString& pathToExport, const QByteArray& wavData)
{
    QFile presetFile(pathToExport);
    qInfo() << "Choosen path:" << pathToExport;

    if(presetFile.open(QIODevice::WriteOnly))
    {
        QByteArray fileData;

        fileData.append(presetHeaderId.header.toUtf8());
        fileData.append(presetHeaderId.versionId.toUtf8());
        fileData.append(presetVersion);

        save_data_cpmodern_t saveData{0};
        QByteArray baPresetName = m_presetName.left(32).toUtf8();
        memcpy(&saveData.name, baPresetName.data(), baPresetName.size());
        memcpy(&saveData.parametersData, &presetData, sizeof(preset_data_cpmodern_t));

        QByteArray rawData;
        char buffer[sizeof(save_data_cpmodern_t)];
        memcpy(buffer, &saveData, sizeof(save_data_cpmodern_t));
        rawData.append(buffer, sizeof(save_data_cpmodern_t));

        writePresetChunk(fileData, presetHeaderId.dataId, rawData);
        writePresetChunk(fileData, presetHeaderId.irNameId, irFile.irName().toUtf8());
        writePresetChunk(fileData, presetHeaderId.irDataId, wavData);

        presetFile.write(fileData);
        presetFile.close();
        return true;
    }
    else
    {
        qDebug() << "Can' open file: " << presetFile.fileName();
        return false;
    }
    return false;
}

bool PresetModern::importData(const QString& filePath, QByteArray& loadedWavData)
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

        pos = fileData.indexOf(presetHeaderId.versionId.toUtf8());
        quint8 importedPresetVersion = fileData.at(pos+presetHeaderId.versionId.size());
        QByteArray rawData = readPresetChunck(fileData, presetHeaderId.dataId);
        if(importedPresetVersion == presetVersion)
        {
            save_data_cpmodern_t imporetedSaveData;
            memcpy(&imporetedSaveData, rawData.data(), sizeof(save_data_cpmodern_t));
            m_presetName = imporetedSaveData.name;
            memcpy(&presetData, &imporetedSaveData.parametersData, sizeof(preset_data_cpmodern_t));
        }
        else
        {
            m_presetName.clear();

            quint8 rawArr[sizeof(preset_data_cplegacy_t)];
            for(int i = 0; i < rawData.size(); i += 2)
            {
                QString chByte = QString(rawData.at(i)) + QString(rawData.at(i+1));
                rawArr[i/2] = chByte.toInt(nullptr, 16);
            }
            preset_data_cplegacy_t imporetedPresetData;
            memcpy(&imporetedPresetData, rawArr, sizeof(preset_data_cplegacy_t));
            presetData = HardwarePresetCPModern::convertLegacyToModern(imporetedPresetData);

        }
        irFile.setIrName(readPresetChunck(fileData, presetHeaderId.irNameId));
        irFile.setIrLinkPath("ir_library/");
        loadedWavData = readPresetChunck(fileData, presetHeaderId.irDataId);
        return true;
    }
    else
    {
        qDebug() << "Can' open file: " << presetFile.fileName();
        return false;
    }
    return false;
}

void PresetModern::writePresetChunk(QByteArray &fileData, QString &chunkName, const QByteArray &chunkData)
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

QByteArray PresetModern::readPresetChunck(const QByteArray &fileData, QString &chunkName)
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

quint8 PresetModern::getPresetFlatNumber() const
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

bool PresetModern::isIrEnabled() const
{
    return presetData.cab_sim_on;
}

void PresetModern::setIsIrEnabled(bool newIsIrEnabled)
{
    presetData.cab_sim_on = newIsIrEnabled;
}
