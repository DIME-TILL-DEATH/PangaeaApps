#include "parser.h"

#include <QDebug>

Parser::Parser() {}

void Parser::parseNewData(const QByteArray &newData)
{
    m_buffer.append(newData);

    int lineSepPos;
    do
    {
        lineSepPos = m_buffer.indexOf("\n");


        QByteArray readedLine = m_buffer.left(lineSepPos);
        m_buffer.remove(0, lineSepPos+1);

        int commSepPos = readedLine.indexOf("\r");
        if(commSepPos == -1) continue;

        QString command = readedLine.left(commSepPos);
        QByteArray arguments = readedLine.right(readedLine.size() - commSepPos - 1);

        qDebug() << "command: " << command << "arguments: " << arguments;
        std::function<void (const QByteArray &)> callback = m_callbacks.value(command);

        if(callback) callback(arguments);
    }while(lineSepPos != -1);
}

void Parser::addCommandHandler(const QString &command,  std::function<void (const QByteArray &)> callback)
{

    m_callbacks.insert(command, callback);
}
