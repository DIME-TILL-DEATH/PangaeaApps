#ifndef IRFILE_H
#define IRFILE_H

#include <QObject>

class IrFile
{
    Q_GADGET

    Q_PROPERTY(QString irName READ irName FINAL)
    Q_PROPERTY(QString irLinkPath READ irLinkPath FINAL)

public:
    IrFile();
    IrFile(QString irName, QString irLinkPath);
    QString irName() const;
    QString irLinkPath() const;

    void setIrName(QString irName);
    void setIrLinkPath(QString irLinkPath);

    void clear();
    bool isLinked();

private:
    QString m_irName;
    QString m_irLinkPath;
};

#endif // IRFILE_H
