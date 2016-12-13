#include "lecturemodel.h"

LectureModel::LectureModel(QString dbPath, QObject *parent)
    : QAbstractItemModel(parent)
{
    dbName = dbPath; // dbName = append(dbPath)
    dataBase = new DataBaseHandler(dbName);
    root = new DataWrapper;
    root->id = 0;
    root->count = 0;
    IData *iData = new IData;
    iData->name = "Root";
    root->data = iData;
    root->number = 0;
    root->type = ROOT;
    //root->parent = qobject_cast<DataWrapper*>(parent);
    DataWrapper* dw;
    QSqlQuery query;
    if (!query.exec("SELECT * FROM subjects_and_themes")) {
        qDebug() << "Невозможно извлечь данные из subjects_and_themes";
    }
    else
    {
        QSqlRecord rec = query.record();
        int type = 0;
        int id = 0;
        while (query.next())
        {
            dw = new DataWrapper;
            iData = new IData;
            type = query.value(rec.indexOf("Type")).toInt();
            if(type == 1)
            {
                dw->type = TERM;
                dw->id = query.value(rec.indexOf("Id_subj")).toInt();
                dw->number = query.value(rec.indexOf("Serial_number")).toInt();
                iData->name = query.value(rec.indexOf("Term")).toString();
                dw->data = iData;
                dw->parent = root;
                dw->count = dataBase->getRowCountOfChild(dw->id, dw->type);
                root->children.append(dw);
            }
        }
    }
}

LectureModel::~LectureModel()
{
    //Need implement!!!
    delete root;
}

QVariant LectureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant();
}

QModelIndex LectureModel::index(int row, int column, const QModelIndex &parent) const
{
    //createIndex()
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    DataWrapper *parentItem;

    if(!parent.isValid())
    {
        parentItem = root;
    }
    else
    {
        parentItem = static_cast<DataWrapper*>(parent.internalPointer());
    }

    DataWrapper *childItem = parentItem->children.value(row);
    if(childItem)
    {
        return createIndex(row,column,childItem);
    }
    else
    {
        return QModelIndex();
    }

    /*if(!parent.isValid()){
        return createIndex(row, column, const_cast<DataWrapper*>(root->children.value(row)));
    }
    DataWrapper* parentItem = static_cast<DataWrapper*>(parent.internalPointer());
    return createIndex(row, column, parentItem->children.value(row));*/

}

QModelIndex LectureModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    if(!index.isValid())
    {
        return QModelIndex();
    }
    DataWrapper *childItem = static_cast<DataWrapper*>(index.internalPointer());
    DataWrapper *parentItem = childItem->parent;

    if(parentItem == root)
    {
        return QModelIndex();
    }

    if(parentItem->parent)
    {
        return createIndex(parentItem->number, 0, parentItem);
    }
    else
    {
        return createIndex(0, 0, parentItem);
    }
}

int LectureModel::rowCount(const QModelIndex &parent) const
{
    DataWrapper *parentItem;
    /*if(parent.column() > 0)
    {
        return 0;
    }*/
    if (!parent.isValid())
    {
        parentItem = root;
    }
    else
    {
        parentItem = static_cast<DataWrapper*>(parent.internalPointer());
    }

    return parentItem->children.count();
}

int LectureModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant LectureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    DataWrapper* itemIndex = static_cast<DataWrapper*>(index.internalPointer());

    if(role == Qt::DisplayRole)
    {
        switch (itemIndex->type) {
        case TERM:
        case COURSE:
        case THEME:{
            return (itemIndex->data->name);
            break;}
        default:
            return (itemIndex->data->name);
            break;
        }  
    }

    if(role == Qt::DecorationRole)
    {
        if(itemIndex->type == IMAGE){
            QImage image((itemIndex->data)->name);
            if(!image.isNull())
            {
                //QPixmap pixmap = QPixmap::fromImage(image);
                //QPixmap pixmap = QPixmap::load((itemIndex->data)->name);
                //return pixmap;

                return QUrl::fromLocalFile(itemIndex->data->name);
            }
        }
    }
    return QVariant();
}

void LectureModel::fetchMore(const QModelIndex &parent)
{
    DataWrapper* parentItem = static_cast<DataWrapper*>(parent.internalPointer());

  //  if(parentItem->count == 0)
  //  {
  //      return;
  //  }

    beginInsertRows(parent,0,parentItem->count-1);
    QSqlQuery query;
    if(parentItem->type != THEME)
    {
        query.exec(QString("SELECT * FROM subjects_and_themes WHERE (Id_parent = %1)").arg(QString::number(parentItem->id)));
    }
    else
    {
        query.exec(QString("SELECT * FROM pictures_info WHERE (Id_parent = %1)").arg(QString::number(parentItem->id)));
    }
    QSqlRecord rec = query.record();
    DataWrapper* dw;
    IData *iData;
    int type = 0;
    int id = 0;
    while (query.next())
    {
        dw = new DataWrapper;
        iData = new IData;
        if(parentItem->type != THEME)
        {
            iData->name = query.value(rec.indexOf("Name_subj")).toString();
            dw->type = static_cast<h_type>(query.value(rec.indexOf("Type")).toInt());
            dw->number = query.value(rec.indexOf("Serial_number")).toInt();
            dw->id = query.value(rec.indexOf("Id_subj")).toInt();
        }
        else
        {
            iData->name = query.value(rec.indexOf("Image_path")).toString();
            iData->comment = query.value(rec.indexOf("Comment")).toString();
            iData->tags = query.value(rec.indexOf("Tags")).toString();
            dw->type = IMAGE;
            dw->number = query.value(rec.indexOf("Number")).toInt();
            dw->id = query.value(rec.indexOf("Id_image")).toInt();
        }
        dw->data = iData;
        dw->parent = parentItem;
        dw->count = dataBase->getRowCountOfChild(dw->id, dw->type);
        parentItem->children.append(dw);

    }
    endInsertRows();

}

bool LectureModel::hasChildren(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        const DataWrapper* parentItem = static_cast<DataWrapper*>(parent.internalPointer());
        Q_ASSERT(parentItem != 0);
        if(parentItem->count > 0)
            return true;
    }
    return QAbstractItemModel::hasChildren(parent);
}

bool LectureModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
            return false;
    }
    const DataWrapper* parentItem = static_cast<DataWrapper*>(parent.internalPointer());
    return (parentItem->count > parentItem->children.size());
}

/*bool LectureModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if(!sourceParent.isValid())
    {
        return false;
    }
    if(!destinationParent.isValid())
    {
        return false;
    }

    int row_count = sourceRow + count - 1;
    DataWrapper* childItem;
    DataWrapper* parentItem = static_cast<DataWrapper*>(destinationParent.internalPointer());
    beginMoveRows(sourceParent,sourceRow, row_count, destinationParent, destinationChild);
    for(int i = 0; i < count; i++)
    {
        childItem = static_cast<DataWrapper*>((sourceParent.child(sourceRow + i,0)).internalPointer());
        childItem->number = destinationChild + i;
        childItem->parent = parentItem;
        //childItem->type = parentItem->type + 1;
        parentItem->children.append(childItem);
        dataBase->updateParAndNumSubjects_and_themes(childItem->id,(childItem->parent)->id,childItem->number);
    }
    endMoveRows();
}*/

void LectureModel::insertTerm(int term)
{
    if(!dataBase->hasTerm(term))
    {
        int newIdSubj = dataBase->getFreeIdInS_T();
        int newSerialNumber = dataBase->getTermSerialNumber(term);
        dataBase->insertIntoSubjects_and_themes(newIdSubj,term,1,"0",newSerialNumber,0);
        dataBase->changeTermSerialNumber(term);
    }
    else
    {
        qDebug() << "Семестр уже имеется";
    }
}





