#include <qdebug.h>
#include <QFile>
#include <QStandardPaths>

#include <QCoreApplication>

#ifdef Q_OS_ANDROID
#include <jni.h>
//#include <QtAndroidExtras/QtAndroid>
//#include <QtAndroidExtras/qandroidjnienvironment.h>

#include <QtCore/private/qandroidextras_p.h> //
#include "androidutils.h"

#include "activityresultmanager.h"


ActivityResultManager::ActivityResultManager()
{

}

void ActivityResultManager::handleActivityResult(int receiverRequestCode, int resultCode, const QJniObject &data)
{
    jint RESULT_OK = QJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");

    if(resultCode == RESULT_OK)
    {
        switch(receiverRequestCode)
        {
            case ActivityType::PICK_IR:
            {
                QJniObject uriObject = data.callObjectMethod("getData", "()Landroid/net/Uri;");
                takeReadUriPermission(uriObject);
                processUri(uriObject);

                qDebug() << "IR file path: " << m_filePath;
                qDebug() << "IR file name: " << m_fileName;

                emit sgIrFilePicked(m_filePath, m_fileName);
                break;
            }

            case ActivityType::PICK_PRESET:
            {
                QJniObject uriObject = data.callObjectMethod("getData", "()Landroid/net/Uri;");
                takeReadUriPermission(uriObject);
                processUri(uriObject);

                qDebug() << "Preset file path: " << m_filePath;
                qDebug() << "Preset file name: " << m_fileName;

                emit sgPresetFilePicked(m_filePath, m_fileName);
                break;
            }

            case ActivityType::PICK_FIRMWARE:
            {
                QJniObject uriObject = data.callObjectMethod("getData", "()Landroid/net/Uri;");
                takeReadUriPermission(uriObject);
                processUri(uriObject);

                qDebug() << "Firmware file path: " << m_filePath;
                qDebug() << "Firmware file name: " << m_fileName;

                emit sgFirmwareFilePicked(m_filePath, m_fileName);
                break;
            }

            case ActivityType::CREATE_PRESET:
            {
                QJniObject uriObject = data.callObjectMethod("getData", "()Landroid/net/Uri;");
                takeReadUriPermission(uriObject);
                takeWriteUriPermission(uriObject);
                processUri(uriObject);


                qDebug() << "Preset file created: " << m_fileName << " path: " << m_filePath;

                emit sgPresetFileCreated(m_filePath, m_fileName);
                break;
            }

            default:
            {
                qDebug() << __FUNCTION__ << "Unknown activity result";
            }
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << "Activity result not OK. Code: " << resultCode;
    }
}

void ActivityResultManager::processUri(QJniObject uriObject)
{

    m_filePath = uriObject.toString();
//    m_filePath.replace("%2F", "/");
//    m_filePath.replace("%3A", ":");

    QtJniTypes::Context androidContext = QNativeInterface::QAndroidApplication::context();
    m_fileName= QJniObject::callStaticObjectMethod(
            "com.amtelectronics.utils/JavaFile", "getFileName",
            "(Landroid/net/Uri;Landroid/content/Context;)Ljava/lang/String;",
            uriObject.object(),
            androidContext.object()).toString();
}

void ActivityResultManager::takeReadUriPermission(QJniObject uriObject)
{
    bool result = AndroidUtils::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    if(!result)
    {
        qDebug() << "READ_EXTERNAL_STORAGE permission denied, trying to request";
         AndroidUtils::requestPermission("android.permission.READ_EXTERNAL_STORAGE");
    }

    QJniObject::callStaticMethod<void>(
            "com.amtelectronics.utils/JavaFile", "takeReadUriPermission",
            "(Landroid/net/Uri;Landroid/content/Context;)V",
            uriObject.object(),
            QNativeInterface::QAndroidApplication::context());

    QJniEnvironment env;
    if (env->ExceptionCheck()) {
        qDebug() << "Java exception";
        env->ExceptionClear();
    }

    qDebug() << "Read URI permission taken";
}

void ActivityResultManager::takeWriteUriPermission(QJniObject uriObject)
{
    bool result = AndroidUtils::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(!result)
    {
        qDebug() << "WRITE_EXTERNAL_STORAGE permission denied, trying to request";
         AndroidUtils::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    }

    QJniObject::callStaticMethod<void>(
            "com.amtelectronics.utils/JavaFile", "takeWriteUriPermission",
            "(Landroid/net/Uri;Landroid/content/Context;)V",
            uriObject.object(),
            QNativeInterface::QAndroidApplication::context());

    QJniEnvironment env;
    if (env->ExceptionCheck()) {
        qDebug() << "Java exception";
        env->ExceptionClear();
    }

    qDebug() << "Write URI permission taken";
}

//TODO: move to AndroidUtils
QString ActivityResultManager::getFileNameFromUri(QString uri)
{
        QJniObject uriJni = QJniObject::callStaticObjectMethod(
            "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;",
            QJniObject::fromString(uri).object<jstring>());

        QtJniTypes::Context androidContext = QNativeInterface::QAndroidApplication::context();
        QString fileName = QJniObject::callStaticObjectMethod(
                               "com.amtelectronics.utils/JavaFile", "getFileName",
                               "(Landroid/net/Uri;Landroid/content/Context;)Ljava/lang/String;",
                               uriJni.object(), androidContext.object()).toString();
        return fileName;
}
#endif
