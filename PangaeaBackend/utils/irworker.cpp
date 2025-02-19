#include <QDebug>

#include "irworker.h"

IRWorker::IRWorker()
{

}

QByteArray IRWorker::decode(QByteArray *data, quint8 lenSample, quint8 chanalCount)
{
    QByteArray ret;
    quint16 lenWav = data->length();

    for(quint16 i=0; i<lenWav; i+=(lenSample*chanalCount))
    {
        ret.append(decodeSample(data->mid(i,lenSample), lenSample));
    }
    return ret;
}

QByteArray IRWorker::decodeSample(QByteArray data, quint8 lenSample)
{
    QByteArray ret;
    if(data.length()<lenSample)
    {
        quint8 i = 0;
        for(i=0; i<data.length(); i++)
        {
            ret.append(data.at(i));
        }
        return ret;
    }

    switch (lenSample)
    {
    case 1:
        ret.append((char)0);
        ret.append((char)0);
        ret.append(data.at(0));
        break;
    case 2:
        ret.append((char)0);
        ret.append(data.at(0));
        ret.append(data.at(1));
        break;
    case 3:
        ret.append(data.at(0));
        ret.append(data.at(1));
        ret.append(data.at(2));
        break;
    case 4:
        ret.append(data.at(1));
        ret.append(data.at(2));
        ret.append(data.at(3));
        break;
    }
    return ret;
}

stWavHeader IRWorker::getFormatWav(QString filePath)
{
    QFile *file = new QFile(filePath);
    if(file->open(QIODevice::ReadOnly))
    {
        QByteArray baWav;
        baWav = file->read(44+32000);
        stWavHeader *wavHeader = reinterpret_cast<stWavHeader*>(baWav.data());
        return *wavHeader;
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Can't open file: " << filePath;
        return stWavHeader{0};
    }

}

QByteArray IRWorker::intToBa(quint32 val)
{
    QByteArray baRet;
    baRet.append((val&0xff));
    baRet.append((val&0xff00)>>8);
    baRet.append((val&0xff0000)>>16);
    baRet.append((val&0xff000000)>>24);
    return baRet;
}

const QByteArray &IRWorker::formFileData()
{
    QByteArray fileData;
    fileData = m_decodedWavIr.left(32000);

    m_fileData.clear();
    m_fileData.append(QString("RIFF").toUtf8());                        //chunkId
    m_fileData.append(QByteArray::fromRawData("\x00\x00\x00\x00", 4));  //chunkSize
    m_fileData.append(QString("WAVE").toUtf8());                        //format
    m_fileData.append(QString("fmt ").toUtf8());                        //subchunk1Id
    m_fileData.append(QByteArray::fromRawData("\x10\x00\x00\x00", 4));  //subchunk1Size
    m_fileData.append(QByteArray::fromRawData("\x01\x00", 2));          //audioFormat
    m_fileData.append(QByteArray::fromRawData("\x01\x00", 2));          //numChannels
    m_fileData.append(QByteArray::fromRawData("\x80\xBB\x00\x00", 4));  //sampleRate
    m_fileData.append(QByteArray::fromRawData("\x80\x32\x02\x00", 4));  //byteRate
    m_fileData.append(QByteArray::fromRawData("\x03\x00", 2));          //blockAlign
    m_fileData.append(QByteArray::fromRawData("\x18\x00", 2));          //bitsPerSample
    m_fileData.append(QString("data").toUtf8());                        //subchunk2Id
    m_fileData.append(IRWorker::intToBa(fileData.length()));                      //subchunk2Size

    m_fileData.append(fileData);
    m_fileData.replace(4, 4, intToBa(m_fileData.length()-8));

    return m_fileData;
}

void IRWorker::decodeWav(QString filePath)
{
    QFile *file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly)) qWarning() << Q_FUNC_INFO << "Can't open file: " << filePath;

    QByteArray baWav;

    baWav = file->read(44+32000);

    stWavHeader *wavHeader = reinterpret_cast<stWavHeader*>(baWav.data());
    quint8 bitPerSample = wavHeader->bitsPerSample;
    quint8 numChannels = wavHeader->numChannels;

    baWav.remove(0, baWav.indexOf("data")+8);

    qDebug() << "numChannels" << numChannels;
    qDebug() << "bitPerSample" << bitPerSample;

    m_decodedWavIr.clear();
    m_decodedWavIr.append(IRWorker::decode(&baWav, bitPerSample/8, numChannels));
}

const QByteArray &IRWorker::decodedWavIr() const
{
    return m_decodedWavIr;
}

QByteArray IRWorker::flatIr()
{
    QByteArray baflatIr;
    baflatIr.append(0xFF); //QString("ffff7f").toUtf8());
    baflatIr.append(0xFF);
    baflatIr.append(0x7F);
    baflatIr.append(256, 0x00);//983*3*2, 0x00);

    QByteArray fileData;

    fileData.clear();
    fileData.append(QString("RIFF").toUtf8());                        //chunkId
    fileData.append(QByteArray::fromRawData("\x00\x00\x00\x00", 4));  //chunkSize
    fileData.append(QString("WAVE").toUtf8());                        //format
    fileData.append(QString("fmt ").toUtf8());                        //subchunk1Id
    fileData.append(QByteArray::fromRawData("\x10\x00\x00\x00", 4));  //subchunk1Size
    fileData.append(QByteArray::fromRawData("\x01\x00", 2));          //audioFormat
    fileData.append(QByteArray::fromRawData("\x01\x00", 2));          //numChannels
    fileData.append(QByteArray::fromRawData("\x80\xBB\x00\x00", 4));  //sampleRate
    fileData.append(QByteArray::fromRawData("\x80\x32\x02\x00", 4));  //byteRate
    fileData.append(QByteArray::fromRawData("\x03\x00", 2));          //blockAlign
    fileData.append(QByteArray::fromRawData("\x18\x00", 2));          //bitsPerSample
    fileData.append(QString("data").toUtf8());                        //subchunk2Id
    fileData.append(IRWorker::intToBa(fileData.length()));                      //subchunk2Size

    fileData.append(baflatIr);
    fileData.replace(4, 4, intToBa(fileData.length()-8));

    return fileData;
}
