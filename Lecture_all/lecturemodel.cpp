#include "lecturemodel.h"
#include <QImage>

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
    if (!query.exec("SELECT * FROM subjects_and_themes ORDER BY Term")) {
        qDebug() << "Невозможно извлечь данные из subjects_and_themes";
    }
    else
    {
        QSqlRecord rec = query.record();
        int type = 0;
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
    if(role == Qt::DisplayRole) {
        switch (itemIndex->type) {
        case TERM:
        case COURSE:
        case THEME:{
            return (itemIndex->data->name);
            break;}
        default: {
            int pos;
            QString path;
            path = itemIndex->data->name;
            pos = path.lastIndexOf('/') + 1;
            return path.mid(pos, path.lastIndexOf('.') - pos);
            break;
        }
        }
    }

    if(role == Qt::DecorationRole) {
        if(itemIndex->type == IMAGE) {
            QImage image((itemIndex->data)->name);
            if(!image.isNull()) {
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

bool LectureModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
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
}

void LectureModel::insertTerm(QString strTerm)
{
    int term = strTerm.toInt();
    if(!dataBase->hasTerm(term))
    {
        int newIdSubj = dataBase->getFreeIdInS_T();
        int newSerialNumber = dataBase->getTermSerialNumber(term);
        insertRows(newSerialNumber,1,QModelIndex());
        QModelIndex updateIndex = this->index(newSerialNumber,0,QModelIndex());
        setData(updateIndex, newIdSubj, INSERT_ID_ROLE);
        setData(updateIndex, term, INSERT_NAME_ROLE);
        setData(updateIndex, term, INSERT_TERM_ROLE);
        dataBase->changeTermSerialNumber(term);
    }
    else
    {
        qDebug() << "Семестр уже имеется";
    }
}

bool LectureModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    DataWrapper* dw = static_cast<DataWrapper*>(index.internalPointer());
    QString name, tag, comment;
    int id, term, parentId;
    bool resultOfUpdate = false;
    switch(role)
    {
    case INSERT_NAME_ROLE:
        name = value.toString();
        dw->data->name = name;
        resultOfUpdate = dataBase->updateName(-1, name);
        break;
    case INSERT_ID_ROLE:
        id = value.toInt();
        dw->id = id;
        dw->number = index.row();
        dw->count = 0;
        resultOfUpdate = dataBase->updateId(-1,id);
        break;
    case INSERT_TAG_ROLE:
        tag = value.toString();
        dw->data->tags = tag;
        break;
    case INSERT_COMMENT_ROLE:
        comment = value.toString();
        dw->data->comment = comment;
        break;
    case INSERT_TERM_ROLE:
        term = value.toInt();
        resultOfUpdate = dataBase->updateTerm(-1,term);
        break;
    }
    emit dataChanged(index, index);
    return resultOfUpdate;
}

bool LectureModel::insertRows(int row, int count, const QModelIndex &parent) //вставка пустых строк
{
    DataWrapper* parentItem;
    if (!parent.isValid())
    {
        parentItem = root;
    }
    else
    {
        parentItem = static_cast<DataWrapper*>(parent.internalPointer());
    }
    beginInsertRows(parent, row, row + count - 1);
    DataWrapper* dw;
    IData* data;
    int newType = parentItem->type + 1;
    for (int i = 0; i < count; i++) {
        dw = new DataWrapper;
        data = new IData;
        dw->data = data;
        dw->parent = parentItem;
        dw->type = static_cast<h_type>(newType);
        parentItem->children.insert(row,dw);
        parentItem->count = parentItem->count + 1;
        dataBase->insertIntoSubjects_and_themes(-1,-1,newType,"-1-1-0",row,parentItem->id);
    }
    endInsertRows();
    return true;
}

Qt::ItemFlags LectureModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | defaultFlags | Qt::ItemIsEditable;
    else
        return defaultFlags;
}

/*Qt::DropAction LectureModel::supportedDropActions() const
{
    return Qt::MoveAction;
}*/

void LectureModel::insertUnit(QString unitName, int type)
{
    //indexForInsert проверяется на валидность в QML, перед вызовом setIndexFI

    int serialNumber;
    int term;
    if(type == 1)
    {
        term = unitName.toInt();
        if(dataBase->hasTerm(term))
        {
            qDebug() << "Семестр уже имеется";
            return;
        }
        if(term < 1)
        {
            qDebug() << "Семестр не подходит";
            return;
        }
        serialNumber = dataBase->getTermSerialNumber(term);
        indexForInsert = QModelIndex();
        dataBase->changeTermSerialNumber(term);
    }
    else
    {
        DataWrapper* parentItem = static_cast<DataWrapper*>(indexForInsert.internalPointer());
        serialNumber = parentItem->count;
        term = 0;
    }
    int newId = dataBase->getFreeIdInS_T();
    insertRows(serialNumber,1,indexForInsert);
    QModelIndex updateIndex = this->index(serialNumber,0,indexForInsert);
    setData(updateIndex, newId, INSERT_ID_ROLE);
    setData(updateIndex, unitName, INSERT_NAME_ROLE);
    setData(updateIndex, term, INSERT_TERM_ROLE);


}

/*void LectureModel:: saveImage(QString str) {

    QImage img = pimg.toImage();
    QImageWriter writer(str);
   // writer.setFileName(str);
    writer.setFormat("jpeg");
    writer.write(img);
    if (writer.canWrite() == false)
        qDebug() << "Файл не записался";
}*/

QVariant LectureModel::grayColor(QString str) {
    QString path = str;
    int pos = path.lastIndexOf(':') + 1;
    path = path.mid(pos);
    QImage image;
    bool ok = image.load(path);
    if (ok == false)
        qDebug()<<"Изображение не считалось";
    if(!image.isNull()) {
        QImage image_gray = image;
        image_gray = image_gray.convertToFormat(QImage::Format_Grayscale8);

        QString path_gray = path;
        int pos_gray = path_gray.lastIndexOf('.');
        path_gray= path_gray.mid(0,path_gray.lastIndexOf('.'))+"_gray"+path_gray.mid(pos_gray);
        bool ok_gray = image_gray.save(path_gray);
        if (ok_gray == false)
            qDebug()<<"Изображение не сохранилось";
        return QUrl::fromLocalFile(path_gray);
    }
    // изменить путь изображения в БД
   return QVariant();
}

bool LectureModel::showMenuItem(const QModelIndex &index, int type)
{
    DataWrapper* item = static_cast<DataWrapper*>(index.internalPointer());
    if(type < 4)
    {
        if(item->type == static_cast<h_type>(type - 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(type == 4)
    {
        if(item->type >= static_cast<h_type>(type - 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        qDebug() << "Incorrect type";
        return false;
    }
}

void LectureModel::setIndexFI(const QModelIndex &index)
{
    indexForInsert = index;
}


