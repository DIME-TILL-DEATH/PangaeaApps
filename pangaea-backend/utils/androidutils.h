#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H

#include <qglobal.h>

#ifdef Q_OS_ANDROID
//#include <QtAndroidExtras>
#include <QtCore/private/qandroidextras_p.h>
#endif

#include <QString>

class AndroidUtils
{
public:
    AndroidUtils();

    static bool checkPermission(QString strPermission);
    static bool requestPermission(QString strPermission);
};

#endif // ANDROIDUTILS_H
