#include <QTime>

#include <QDebug>
#include <iostream>

#ifdef Q_OS_ANDROID
//#include <QtAndroidExtras>
#include <QtCore/private/qandroidextras_p.h>

#include <android/log.h>
#include "androidutils.h"
#endif

#include "logger.h"

Logger::Logger(QObject* parent)
{
    Q_UNUSED(parent)


#ifdef Q_OS_ANDROID
    QString folderPath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(0)+"/AMT/pangaea_mobile/";

    if(!QDir(folderPath).exists())
    {
        QDir().mkpath(folderPath);
    }

    logFile.setFileName(folderPath+"log.txt");

    bool result = AndroidUtils::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(!result)
    {
        qDebug() << "WRITE_EXTERNAL_STORAGE permission denied, trying to request";
         AndroidUtils::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    }

    result = AndroidUtils::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    if(!result)
    {
        qDebug() << "READ_EXTERNAL_STORAGE permission denied, trying to request";
         AndroidUtils::requestPermission("android.permission.READ_EXTERNAL_STORAGE");
    }

#else

#ifdef Q_OS_MACOS
    QString folderPath = QDir::homePath() + "/Documents/";
#else
    QString folderPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) + "/";
#endif

#ifndef Q_OS_IOS
    folderPath += "AMT/Pangaea-desktop/";
#endif

    if(!QDir(folderPath).exists())
    {
        QDir().mkpath(folderPath);
    }

    logFile.setFileName(folderPath+"log.txt");
#endif

    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Can't open log file";
    }
    qDebug() << logFile.fileName();

    logFile.resize(0);
    outLog.setDevice(&logFile);
}

Logger::~Logger()
{
    qDebug() << "Logger destructor";
    logFile.close();
}

void Logger::messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(currentHandler)
        currentHandler->messageOutputHandlerImplementation(type, context, msg);
}

void Logger::setAsMessageHandlerForApp()
{
    currentHandler = this;
    qInstallMessageHandler(messageOutputHandler);
}

void Logger::messageOutputHandlerImplementation(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    switch (type)
    {
        case QtDebugMsg:
        #ifdef ANDROID
            __android_log_print(ANDROID_LOG_DEBUG, "pangaea_mobile debug", "%s", qPrintable(localMsg.constData()));
        #endif
            std::cout << localMsg.constData() << std::endl << std::flush;

            outLog << QTime::currentTime().toString() << " debug: "
                   << localMsg.constData() << "\n("
                   << file << ":" << context.line << "," << function << ")\n\n";
            outLog.flush();
            break;

        case QtInfoMsg:
        #ifdef ANDROID
            __android_log_print(ANDROID_LOG_INFO, "pangaea_mobile info", "%s", qPrintable(localMsg.constData()));
        #endif
            std::cout << localMsg.constData() << std::endl << std::flush;

            // outLog << QTime::currentTime().toString() << " info: "
            //        << localMsg.constData() << "\n("
            //        << file << ":" << context.line << "," << function << ")\n\n";
            outLog << QTime::currentTime().toString() << ": "
                   << localMsg.constData() << "\n\n";
            outLog.flush();
            break;

        case QtWarningMsg:
        #ifdef ANDROID
            __android_log_print(ANDROID_LOG_WARN, "pangaea_mobile warning", "%s", qPrintable(localMsg.constData()));
        #endif
            std::cout << "Error: " << localMsg.constData() << std::endl << std::flush;
            outLog << QTime::currentTime().toString() << " warning: "
                   << localMsg.constData() << "\n("
                   << file << ":" << context.line << "," << function << ")\n\n";
            outLog.flush();
            break;

        case QtCriticalMsg:
        #ifdef ANDROID
            __android_log_print(ANDROID_LOG_ERROR, "pangaea_mobile error", "%s", qPrintable(localMsg.constData()));
        #endif
            fprintf(stdout, "Critical: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            std::cout << std::endl << std::flush;

            outLog << QTime::currentTime().toString() << " critical: "
                   << localMsg.constData() << "\n("
                   << file << ":" << context.line << "," << function << ")\n\n";
            outLog.flush();
            break;

        case QtFatalMsg:
        #ifdef ANDROID
            __android_log_print(ANDROID_LOG_FATAL, "pangaea_mobile fatal", "%s", qPrintable(localMsg.constData()));
        #endif
            fprintf(stdout, "Fatal: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            std::cout << std::endl << std::flush;

            outLog << QTime::currentTime().toString() << " fatal: "
                   << localMsg.constData() << "\n("
                   << file << ":" << context.line << "," << function << ")\n\n";
            outLog.flush();

            logFile.close();
            break;
    }
}
