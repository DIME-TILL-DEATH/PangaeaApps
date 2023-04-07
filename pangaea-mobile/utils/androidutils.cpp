#include "androidutils.h"

AndroidUtils::AndroidUtils()
{

}

bool AndroidUtils::checkPermission(QString strPermission)
{
    auto result = QtAndroidPrivate::checkPermission(strPermission).then([](
            QtAndroidPrivate::PermissionResult result) {
                return result;
            });

    result.waitForFinished();
    return result.result() != QtAndroidPrivate::PermissionResult::Denied;
}

bool AndroidUtils::requestPermission(QString strPermission)
{
    auto result = QtAndroidPrivate::requestPermission(strPermission);
    return result.result() != QtAndroidPrivate::PermissionResult::Denied;
}
