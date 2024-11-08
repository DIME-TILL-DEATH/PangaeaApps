#ifndef MASKEDPARSER_H
#define MASKEDPARSER_H

#include <QObject>
//#include <QRegExp>
#include <QString>

class MaskedParser : public QObject
{
    Q_OBJECT
public:
    explicit MaskedParser(QByteArray val, QByteArray mask);
    QList<QByteArray> ret;
    bool getParse(QByteArray, QList<QByteArray>*);
    void clearAll();

private:
    QByteArray val, mask;
//    QByteArray baBuffer;
    quint16 curPos;
    quint16 maskPos;
    quint16 curParam;
    quint8 prevMask; //предидущий символ маски


signals:

public slots:
};

#endif // MASKEDPARSER_H
