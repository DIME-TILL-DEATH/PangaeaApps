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

        int commTabSepPos = readedLine.indexOf("\r");
        int commSpaceSepPos = readedLine.indexOf(" ");
        int commSepPosition;

        if(commSpaceSepPos>0 && commTabSepPos > commSpaceSepPos) commSepPosition = commSpaceSepPos;
        else commSepPosition = commTabSepPos;

        // if(sepPosition == -1) continue;

        QString command;
        QByteArray arguments;
        if(commSepPosition>-1)
        {
            command = readedLine.left(commSepPosition);
            arguments = readedLine.right(readedLine.size() - commSepPosition - 1);
        }
        else
        {
            command = readedLine;
        }

        // qDebug() << m_parserName << " --> command: " << command << "arguments: " << arguments;
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
