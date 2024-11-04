#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QByteArray>
#include <QMap>

#include <functional>

class Parser
{
public:
    Parser();

    void parseNewData(const QByteArray& newData);

    void addCommandHandler(const QString& command, std::function<void(const QByteArray&)> callback);
private:
    QByteArray m_buffer;

    QMap<QString, std::function<void(const QByteArray&)> > m_callbacks;
};

#endif // PARSER_H
