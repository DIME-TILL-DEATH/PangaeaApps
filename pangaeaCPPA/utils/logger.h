#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include <QStandardPaths>

class Logger
{
public:
    explicit Logger();
    ~Logger();

    static void messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void setAsMessageHandlerForApp();

private:
    QFile logFile;
    QTextStream outLog;

    static Logger* currentHandler;
    void messageOutputHandlerImplementation(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGER_H
