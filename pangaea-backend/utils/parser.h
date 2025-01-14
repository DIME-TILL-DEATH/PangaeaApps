#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QByteArray>
#include <QMap>

#include <functional>

#include "maskedparser.h"

class Parser
{
public:
    Parser(QString parserName = "");

    QList<QByteArray> parseNewData(const QByteArray& newData);

    void addCommandHandler(const QString& command, std::function<void (const QString &, const QByteArray &, const QByteArray&)> callback);
    void addCureParser(QString comm, MaskedParser* parser);

    // Костыль для старого формата rns
    void enableFullEndMode() {fullEndModeEnabled=true;};
    void disableFullEndMode() {fullEndModeEnabled=false;};

    QByteArray getBufferAndFlush();
private:
    QByteArray m_buffer;

    QString m_parserName;

    bool fullEndModeEnabled{false};

    QMap<QString, std::function<void(const QString& command, const QByteArray&, const QByteArray&)> > m_callbacks;

    // cure for non-\n commands
    QList<QPair<QString, MaskedParser*> > m_mskParserList;
};

#endif // PARSER_H
