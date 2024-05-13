#ifndef ACTIVITYRESULTMANAGER_H
#define ACTIVITYRESULTMANAGER_H

#include <qglobal.h>

#ifdef Q_OS_ANDROID

#include <QtCore/private/qandroidextras_p.h>
//#include <QAndroidActivityResultReceiver>

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
//public:
//    ActivityResultManager();
//    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data) override;

//    static QString getFileNameFromUri(QString uri);
//    static void takeReadUriPermission(QAndroidJniObject uriObject);
//    static void takeWriteUriPermission(QAndroidJniObject uriObject);
//private:
//    QString m_fileName;
//    QString m_filePath;

//    void processUri(QAndroidJniObject uriObject);

public:
    ActivityResultManager();
    void handleActivityResult(int receiverRequestCode, int resultCode, const QJniObject &data) override;

    static QString getFileNameFromUri(QString uri);
    static void takeReadUriPermission(QJniObject uriObject);
    static void takeWriteUriPermission(QJniObject uriObject);
private:
    QString m_fileName;
    QString m_filePath;

    void processUri(QJniObject uriObject);

signals:
    void sgIrFilePicked(QString fullFilePath, QString fileName);
    void sgPresetFilePicked(QString fullFilePath, QString fileName);
    void sgFirmwareFilePicked(QString fullFilePath, QString fileName);
    void sgPresetFileCreated(QString fullFilePath, QString fileName);
};

#endif // ACTIVITYRESULTMANAGER_H
#endif
