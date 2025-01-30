#include "irfile.h"

IrFile::IrFile()
{

}

IrFile::IrFile(QString irName, QString irLinkPath)
    : m_irName{irName},
    m_irLinkPath{irLinkPath}
{

}

QString IrFile::irName() const
{
    return m_irName;
}

QString IrFile::irLinkPath() const
{
    return m_irLinkPath;
}

void IrFile::setIrName(QString irName)
{
    m_irName = irName;
}

void IrFile::setIrLinkPath(QString irLinkPath)
{
    m_irLinkPath = irLinkPath;
}

void IrFile::clear()
{
    m_irName = "";
    m_irLinkPath = "";
}

bool IrFile::isLinked()
{
    return !m_irLinkPath.isEmpty();
}
