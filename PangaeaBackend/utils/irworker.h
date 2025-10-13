#ifndef IRWORKER_H
#define IRWORKER_H

#include <QString>
#include <QByteArray>
#include <QFile>

struct stWavHeader
{
    unsigned char chunkId[4];
    quint32 chunkSize;
    unsigned char format[4];
    unsigned char subchunk1Id[4];
    quint32 subchunk1Size;
    quint16 audioFormat;
    quint16 numChannels;
    quint32 sampleRate;
    quint32 byteRate;
    quint16 blockAlign;
    quint16 bitsPerSample;
    unsigned char subchunk2Id[4];
    quint32 subchunk2Size;
};

class IRWorker
{
public:
    IRWorker();

    static stWavHeader getFormatWav(QString filePath);

    static QString convertWav(const QString& srcPath);

    static QByteArray decode(QByteArray *data, quint8 lenSample, quint8 chanalCount);
    static QByteArray decodeSample(QByteArray data, quint8 lenSample);
    void decodeWav(QString filePath);
    const QByteArray &decodedWavIr() const;

    static QByteArray flatIr();

    static QByteArray intToBa(quint32 val);

    void saveToFile(const QByteArray &fileData);
    const QByteArray& formFileData();

private:
    QByteArray m_decodedWavIr;
    QByteArray m_fileData;
};

#endif // IRWORKER_H
