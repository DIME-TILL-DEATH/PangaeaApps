#ifndef FILEBROWSERMODEL_H
#define FILEBROWSERMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

class FileBrowserModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString currentFolder READ currentFolder WRITE setCurrentFolder NOTIFY currentFolderChanged FINAL)
public:
    explicit FileBrowserModel(QObject *parent = nullptr);

    enum FsObjectType
    {
        File,
        Dir,
        Unknown
    };
    Q_ENUM(FsObjectType)

    typedef struct
    {
        QString name;
        FsObjectType type;
    }FsObject;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void updateFsObjectsList(const QList<FsObject>& newList);

    QString currentFolder() const;
    void setCurrentFolder(const QString &newCurrentFolder);

signals:
    void currentFolderChanged();

private:
    QList<FsObject> m_fsObjectsList;

    enum ListRoles{
        FsObjectNameRole = Qt::UserRole + 1,
        FsObjectTypeRole
    };
    QString m_currentFolder;
};
typedef FileBrowserModel::FsObjectType FsObjectType;
Q_DECLARE_METATYPE(FsObjectType)

#endif // FILEBROWSERMODEL_H
