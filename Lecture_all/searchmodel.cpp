#include "searchmodel.h"

SearchModel::SearchModel(DataBaseHandler *db, QObject *parent) : QAbstractListModel(parent)
{
    dataBase = db;
}

int SearchModel::rowCount(const QModelIndex &parent) const
{
    return rootList.count();
}

QVariant SearchModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= rootList.size())
    {
        return QVariant();
    }
    switch (role) {
    case NameRole:
        return QUrl::fromLocalFile(rootList.at(index.row())->name);
        break;
    case TagRole:
        return rootList.at(index.row())->tags;
        break;
    case CommentRole:
        return rootList.at(index.row())->comment;
        break;
    }
    /*if(role == NameRole)
    {
        return QUrl::fromLocalFile(rootList.at(index.row())->name);
    }*/
    return QVariant();
}

QVariant SearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return "Поиск";
}

Qt::ItemFlags SearchModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if (index.isValid())
        return defaultFlags | Qt::ItemIsEditable ;
    else
        return defaultFlags;
}

bool SearchModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    ImageData* imData;
    for (int i = 0; i < count; ++i)
    {
        imData = new ImageData;
        imData->name = "123";
        rootList.insert(row,imData);
    }

    endInsertRows();
    return true;
}

bool SearchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        ImageData* imData = rootList.at(index.row());
        QString strName, strTag, strComment;
        switch(role)
        {
        case NameRole:
            strName = value.toString();
            imData->name = strName;
            break;
        case TagRole:
            strTag = value.toString();
            imData->tags = strTag;
            break;
        case CommentRole:
            strComment = value.toString();
            imData->comment = strComment;
            break;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


bool SearchModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for (int i = 0; i < count; ++i)
    {
        rootList.removeAt(row);
    }

    endRemoveRows();
    return true;
}

bool SearchModel::searchImage(QString strTag)
{
    QString strSearch = "SELECT * FROM pictures_info";
    QSqlQuery query;
    if(!query.exec(strSearch))
    {
        return false;
    }

    if(rootList.size() > 0)
    {
        removeRows(0,rootList.size(),QModelIndex());
    }

    QSqlRecord rec = query.record();
    int p, serialNumber;
    QString name = "",
            comment = "",
            tags = "";
    QModelIndex updateIndex;
    while(query.next())
    {
        p = 1;
        tags = query.value(rec.indexOf("Tags")).toString();
        p = p + tags.indexOf(strTag,0);
        if(p)
        {
            name = query.value(rec.indexOf("Image_path")).toString();
            comment = query.value(rec.indexOf("Comment")).toString();
            serialNumber = rootList.size();
            insertRows(serialNumber,1,QModelIndex());
            updateIndex = this->index(serialNumber,0,QModelIndex());
            setData(updateIndex,name,NameRole);
            setData(updateIndex,comment,CommentRole);
            setData(updateIndex,tags,TagRole);
        }
    }
    return true;
}

QHash<int, QByteArray> SearchModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TagRole] = "tag";
    roles[CommentRole] = "comment";
    return roles;
}
