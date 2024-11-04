#ifndef LEGACYPARSER_H
#define LEGACYPARSER_H

#include <QObject>
//#include <QRegExp>
#include <QString>

class LegacyParser : public QObject
{
    Q_OBJECT
public:
    explicit LegacyParser(QByteArray val, QByteArray mask);
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

#endif // LEGACYPARSER_H
