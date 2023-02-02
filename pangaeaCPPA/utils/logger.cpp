#include <QTime>

#include <QDebug>
#include "logger.h"

Logger::Logger(QObject* parent)
{
    Q_UNUSED(parent)

    QString folderPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)+"/AMT/pangaeaCPPA/";

    if(!QDir(folderPath).exists())
    {
        QDir().mkpath(folderPath);
    }

    logFile.setFileName(folderPath+"log.txt");

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
    qDebug() << "Logger destructor. Closing Log file.";
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
    {
        QByteArray localMsg = msg.toLocal8Bit();
        const char *file = context.file ? context.file : "";
        const char *function = context.function ? context.function : "";

        switch (type)
        {
            case QtDebugMsg:
                std::cout << localMsg.constData() << std::endl << std::flush;

                outLog << QTime::currentTime().toString() << " debug: "
                       << localMsg.constData() << "\n("
                       << file << ":" << context.line << "," << function << ")\n\n";
                outLog.flush();
                break;

            case QtInfoMsg:
                std::cout << localMsg.constData() << std::endl << std::flush;

                outLog << QTime::currentTime().toString() << " info: "
                       << localMsg.constData() << "\n("
                       << file << ":" << context.line << "," << function << ")\n\n";
                outLog.flush();
                break;

            case QtWarningMsg:
                std::cout << "Error: " << localMsg.constData() << std::endl << std::flush;

                outLog << QTime::currentTime().toString() << " warning: "
                       << localMsg.constData() << "\n("
                       << file << ":" << context.line << "," << function << ")\n\n";
                outLog.flush();
                break;

            case QtCriticalMsg:
                fprintf(stdout, "Critical: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::endl << std::flush;

                outLog << QTime::currentTime().toString() << " critical: "
                       << localMsg.constData() << "\n("
                       << file << ":" << context.line << "," << function << ")\n\n";
                outLog.flush();
                break;

            case QtFatalMsg:
                fprintf(stdout, "Fatal: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::endl << std::flush;

                outLog << QTime::currentTime().toString() << " fatal: "
                       << localMsg.constData() << "\n("
                       << file << ":" << context.line << "," << function << ")\n\n";
                outLog.flush();
                break;
        }
    }
}
