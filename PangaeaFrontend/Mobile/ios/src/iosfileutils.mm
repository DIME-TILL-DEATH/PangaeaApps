#import "iosfileutils.hpp"

#import <QFile>
#import <QFileInfo>
#import <QStandardPaths>

#import <Foundation/Foundation.h>

void IosFileUtils::copyFileToTmp(const QUrl &url, QString& pathToCopiedFile)
{
  NSURL *iosURL = url.toNSURL();

  if([iosURL startAccessingSecurityScopedResource])
    qDebug() << "access to resource granted";

  QString localPath = url.toLocalFile();

  QFile file(localPath);
  QFileInfo fileInfo(localPath);
  QString fileName, copiedFilePath;

  if(fileInfo.isFile())
  {
    fileName = fileInfo.fileName();
    copiedFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + fileName;
      
      if(QFile(copiedFilePath).exists())
      {
          QFile(copiedFilePath).remove();
      }
      
    if(!file.copy(copiedFilePath))
    {
      qWarning() << "Can't copy file to temporally directory";
    }
    else
    {
      pathToCopiedFile = copiedFilePath;
    }
  }
  else
  {
    qWarning() << "Can't open file" << localPath;
  }

  [iosURL stopAccessingSecurityScopedResource];
}
