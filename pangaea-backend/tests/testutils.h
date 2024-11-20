#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QString>

namespace TestUtils
{
void copyDirectoryNested(QString from, QString to)
{

    QDirIterator it(from, QDirIterator::Subdirectories);

    while (it.hasNext()) {

        QString file_in = it.next();

        QFileInfo file_info = QFileInfo(file_in);

        QString file_out = file_in;
        file_out.replace(from,to);

        if(file_info.isFile())
        {
            //is file copy
            QFile::copy(file_in, file_out);
            QFile copiedFile(file_out);
            copiedFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
        }

        if(file_info.isDir())
        {
            //dir mkdir
            QDir dir(file_out);
            if (!dir.exists())
                dir.mkpath(".");
        }
    }
}

}

#endif // TESTUTILS_H
