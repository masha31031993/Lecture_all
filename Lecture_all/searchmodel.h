#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "databasehandler.h"
//#include "lecturemodel.h"

struct ImageData
{
    QString name;
    QString comment;
    QString tags;
};



class SearchModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum AnimalRoles {
            NameRole = Qt::UserRole + 1,
            TagRole,
            CommentRole
        };

    explicit SearchModel(DataBaseHandler *db,QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    Q_INVOKABLE bool searchImage(QString strTag);

protected:

    QHash<int, QByteArray> roleNames() const;

private:

    DataBaseHandler *dataBase;
    QList<ImageData*> rootList;
    //QStringList stringList;
};


#endif // SEARCHMODEL_H
