#include "parser.h"

#include <QDebug>

Parser::Parser(QString parserName)
    :m_parserName{parserName}
{}

QList<QByteArray> Parser::parseNewData(const QByteArray &newData)
{
    m_buffer.append(newData);

    QList<QByteArray> recievedCommands;

    int lineSepPos;
    do
    {
        QString commSeparator;
        if(fullEndModeEnabled) commSeparator = "\nEND\n";
        else commSeparator = "\n";

        lineSepPos = QString(m_buffer).indexOf(commSeparator);
        if(lineSepPos == -1) break;

        QByteArray readedLine = m_buffer.left(lineSepPos);
        m_buffer.remove(0, lineSepPos + commSeparator.size());

        int commTabSepPos = readedLine.indexOf("\r");
        int commSpaceSepPos = readedLine.indexOf(" ");
        int sepPosition;

        if(commSpaceSepPos>0 && commTabSepPos > commSpaceSepPos) sepPosition = commSpaceSepPos;
        else sepPosition = commTabSepPos;

        if(sepPosition == -1) continue;

        QString command = readedLine.left(sepPosition);
        QByteArray arguments = readedLine.right(readedLine.size() - sepPosition - 1);


        qDebug() << m_parserName << " --> command: " << command << "arguments: " << arguments;
        recievedCommands.append(command.toUtf8());
        std::function<void (const QString& command, const QByteArray &)> callback = m_callbacks.value(command);

        if(callback) callback(command, arguments);
    }while(lineSepPos != -1);

    return recievedCommands;
}

void Parser::addCommandHandler(const QString &command,  std::function<void (const QString& command, const QByteArray &)> callback)
{

    m_callbacks.insert(command, callback);
}
