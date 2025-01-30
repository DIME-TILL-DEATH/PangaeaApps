#include "parser.h"

#include <QDebug>
#include <qforeach.h>

Parser::Parser(QString parserName)
    :m_parserName{parserName}
{}

QList<QByteArray> Parser::parseNewData(const QByteArray &newData)
{
    m_buffer.append(newData);

    // cure for non \n commands
    for(int i=0; i < m_mskParserList.size(); i++)
    {
        QString comm = m_mskParserList.at(i).first;
        MaskedParser* curParser = m_mskParserList.at(i).second;

        QList<QByteArray> argsList;
        while(curParser->getParse(m_buffer, &argsList))
        {
            QByteArray dataForRemove = comm.toUtf8();
            if(argsList.size() > 0)
            {
                dataForRemove += " ";
                dataForRemove += argsList.at(0);
            }

            if(argsList.size() == 2)
            {
                dataForRemove += " ";
                dataForRemove += argsList.at(1);
            }
            dataForRemove += "\r";
            while(m_buffer.indexOf(dataForRemove) != -1)
            {
                m_buffer.remove(m_buffer.indexOf(dataForRemove), dataForRemove.size());
                // qDebug () << "Data for remove from buffer: " << dataForRemove << "Result buffer: " << m_buffer;
            }
        }
    }
    //--------------------------------------------------------


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

        int commSepPosition = readedLine.indexOf("\r");

        QString command;
        QString commandAndArgs;
        QByteArray arguments;
        QByteArray data;
        if(commSepPosition>-1)
        {
            commandAndArgs = readedLine.left(commSepPosition);
            data = readedLine.right(readedLine.size() - commSepPosition - 1);

            int argSepPos = commandAndArgs.indexOf(" ");
            if(argSepPos>-1)
            {
                command = readedLine.left(argSepPos);
                arguments = readedLine.mid(argSepPos+1, readedLine.size() - data.size() - command.size() - 2);
            }
            else
            {
                command = readedLine.left(commSepPosition);
            }
        }
        else
        {
            int argSepPos = readedLine.indexOf(" ");
            if(argSepPos>-1)
            {
                command = readedLine.left(argSepPos);
                arguments = readedLine.right(readedLine.size() - argSepPos - 1);
            }
            else
            {
                command = readedLine;
            }
        }
        // qDebug() << m_parserName << " --> command: " << command << "arguments: " << arguments << "data: " << data;

        recievedCommands.append(command.toUtf8());
        std::function<void (const QString& command, const QByteArray &, const QByteArray&)> callback = m_callbacks.value(command);

        if(callback) callback(command, arguments, data);
    }while(lineSepPos != -1);

    return recievedCommands;
}

void Parser::addCommandHandler(const QString &command,  std::function<void (const QString &, const QByteArray &, const QByteArray &)> callback)
{

    m_callbacks.insert(command, callback);
}

void Parser::addCureParser(QString comm, MaskedParser *parser)
{
    m_mskParserList.append({comm, parser});
}

QByteArray Parser::getBufferAndFlush()
{
    QByteArray answer = m_buffer;
    m_buffer.clear();
    return answer;
}
