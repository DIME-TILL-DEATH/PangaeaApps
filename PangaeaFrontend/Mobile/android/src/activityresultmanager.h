#ifndef ACTIVITYRESULTMANAGER_H
#define ACTIVITYRESULTMANAGER_H

#include <qglobal.h>

#ifdef Q_OS_ANDROID

#include <QUrl>

#include <QtCore/private/qandroidextras_p.h>
//#include <QAndroidActivityResultReceiver>

Q_DECLARE_JNI_CLASS(JClipData, "android/content/ClipData")
Q_DECLARE_JNI_CLASS(JItem, "android/content/ClipData$Item")
Q_DECLARE_JNI_CLASS(JUri, "android/net/Uri")
Q_DECLARE_JNI_CLASS(JFile, "com.amtelectronics.utils/JavaFile")

enum ActivityType
{
    DEFAULT = 0,
    PICK_IR,
    PICK_PRESET,
    PICK_FIRMWARE,
    CREATE_PRESET
};

class ActivityResultManager : public QObject, public QAndroidActivityResultReceiver
{
    Q_OBJECT
public:
    ActivityResultManager();
    void handleActivityResult(int receiverRequestCode, int resultCode, const QJniObject &data) override;

    static QString getFileNameFromUri(QString uri);
    static void takeReadUriPermission(QtJniTypes::JUri uriObject);
    static void takeWriteUriPermission(QtJniTypes::JUri uriObject);
private:
    QString m_fileName;
    QString m_filePath;

    void processUri(QtJniTypes::JUri uriObject);

signals:
    void sgIrFilePicked(QString fullFilePath, QString fileName);
    void sgIrFileListPicked(QList<QUrl> fileList, QUrl dsrPath = QUrl());
    void sgPresetFilePicked(QString fullFilePath, QString fileName);
    void sgFirmwareFilePicked(QString fullFilePath, QString fileName);
    void sgPresetFileCreated(QString fullFilePath, QString fileName);
};

#endif // ACTIVITYRESULTMANAGER_H
#endif
