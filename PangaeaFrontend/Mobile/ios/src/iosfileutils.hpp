#ifndef IOSFILEUTILS_HPP
#define IOSFILEUTILS_HPP

#include <QObject>
#include <QUrl>

class IosFileUtils : public QObject
{
    Q_OBJECT
public:
    explicit IosFileUtils(QObject* parent) : QObject{parent} {}

    static void copyFileToTmp(const QUrl& url, QString& pathToCopiedFile);

};

#endif // IOSFILEUTILS_HPP
