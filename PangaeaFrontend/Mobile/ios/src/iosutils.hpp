#ifndef IOSUTILS_H
#define IOSUTILS_H

#include <QObject>
#include <QUrl>

class IosUtils : public QObject
{
    Q_OBJECT
public:
    explicit IosUtils(QObject* parent) : QObject{parent} {}

    static void copyFileToTmp(const QUrl& url, QString& pathToCopiedFile);
    static void wakeLockDisable();

};

#endif // IOSUTILS_H
