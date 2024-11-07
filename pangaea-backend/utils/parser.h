#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QByteArray>
#include <QMap>

#include <functional>

class Parser
{
public:
    Parser(QString parserName = "");

    QList<QByteArray> parseNewData(const QByteArray& newData);

    void addCommandHandler(const QString& command, std::function<void (const QString &, const QByteArray &)> callback);

    // Костыль для старого формата rns
    void enableFullEndMode() {fullEndModeEnabled=true;};
    void disableFullEndMode() {fullEndModeEnabled=false;};
private:
    QByteArray m_buffer;

    QString m_parserName;

    bool fullEndModeEnabled{true};

    QMap<QString, std::function<void(const QString& command, const QByteArray&)> > m_callbacks;
};

#endif // PARSER_H
