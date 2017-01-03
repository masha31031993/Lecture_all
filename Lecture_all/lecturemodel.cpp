#include "lecturemodel.h"
#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QPainter>

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
            dw->count = dataBase->getRowCountOfChild(dw->id, dw->type);
        }
        else
        {
            iData->name = query.value(rec.indexOf("Image_path")).toString();
            iData->comment = query.value(rec.indexOf("Comment")).toString();
            iData->tags = query.value(rec.indexOf("Tags")).toString();
            dw->type = IMAGE;
            dw->number = query.value(rec.indexOf("Number")).toInt();
            dw->id = query.value(rec.indexOf("Id_image")).toInt();
            dw->count = 0;
        }
        dw->data = iData;
        dw->parent = parentItem;
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

bool LectureModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    DataWrapper* dw = static_cast<DataWrapper*>(index.internalPointer());
    QString name, tag, comment;
    int id, term;
    bool resultOfUpdate = false;
    switch(role)
    {
    case INSERT_NAME_ROLE:
        name = value.toString();
        dw->data->name = name;
        resultOfUpdate = dataBase->updateName(dw->id, name, dw->type);
        break;
    case INSERT_ID_ROLE:
        id = value.toInt();
        dw->id = id;
        dw->number = index.row();
        dw->count = 0;
        resultOfUpdate = dataBase->updateId(-1,id,dw->type);
        break;
    case INSERT_TAG_ROLE:
        tag = value.toString();
        dw->data->tags = tag;
        resultOfUpdate = dataBase->updateTagP_I(dw->id,tag);
        break;
    case INSERT_COMMENT_ROLE:
        comment = value.toString();
        dw->data->comment = comment;
        resultOfUpdate = dataBase->updateCommentP_I(dw->id, comment);
        break;
    case INSERT_TERM_ROLE:
        term = value.toInt();
        resultOfUpdate = dataBase->updateTerm(dw->id,term);
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
        if(newType < 4)
        {
            dataBase->insertIntoSubjects_and_themes(-1,-1,newType,"-1-1-0",row,parentItem->id);
        }
        else
        {
            dataBase->insertIntoPictures_info(-1,"Пусто","Пусто","-1-1-0",row,parentItem->id);
        }
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
    //selectedIndex проверяется на валидность в QML, перед вызовом setSelIndex

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
        selectedIndex = QModelIndex();
        dataBase->changeTermSerialNumber(term);
    }
    else
    {
        DataWrapper* parentItem = static_cast<DataWrapper*>(selectedIndex.internalPointer());
        serialNumber = parentItem->count;
        term = 0;
    }
    int newId = dataBase->getFreeIdInS_T();
    insertRows(serialNumber,1,selectedIndex);
    QModelIndex updateIndex = this->index(serialNumber,0,selectedIndex);
    setData(updateIndex, newId, INSERT_ID_ROLE);
    setData(updateIndex, unitName, INSERT_NAME_ROLE);
    setData(updateIndex, term, INSERT_TERM_ROLE);
}

void LectureModel::insertImage(QString path, QString comment, QString tags)
{
    QImage image;
    int pos = path.lastIndexOf(':') + 1;
    //path = path.mid(pos);
    if(image.load(path))
    {
        QString folderPath = "../Pictures/";
        pos = path.lastIndexOf('/') + 1;
        path = folderPath.append(path.mid(pos));
        image.save(path);
        DataWrapper* selectedItem = static_cast<DataWrapper*>(selectedIndex.internalPointer());
        if(selectedItem->type == IMAGE)
        {
            selectedIndex = selectedIndex.parent();
            selectedItem = static_cast<DataWrapper*>(selectedIndex.internalPointer());
        }
        int serialNumber = selectedItem->count;
        int newId = dataBase->getFreeIdPic_Inf();
        insertRows(serialNumber,1,selectedIndex);
        QModelIndex updateIndex = this->index(serialNumber,0,selectedIndex);
        setData(updateIndex, newId, INSERT_ID_ROLE);
        setData(updateIndex, path, INSERT_NAME_ROLE);
        setData(updateIndex, tags, INSERT_TAG_ROLE);
        setData(updateIndex, comment, INSERT_COMMENT_ROLE);
    }


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

void LectureModel::setSelIndex(const QModelIndex &index)
{
    selectedIndex = index;
}

bool LectureModel::removeRows(int row, int count, const QModelIndex &parent)
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
    beginRemoveRows(parent, row, row + count - 1);
    DataWrapper* dw;
    for(int i = 0; i<count; i++)
    {
        dw = parentItem->children.at(row);
        if(dw->type != IMAGE)
        {
            dataBase->deleteFromSubjects_and_themes(dw->id);
        }
        else
        {
            dataBase->deleteFromPictures_info(dw->id);
        }
        parentItem->children.removeAt(row);
        parentItem->count--;
        foreach (DataWrapper* child, parentItem->children) {
            if(child->number > row)
            {
                child->number--;
            }
        }
        dataBase->decrimentSerialNimber(parentItem->id,row,parentItem->type);
    }
    endRemoveRows();
    return true;
}

void LectureModel::removeUnit()
{
    DataWrapper* item = static_cast<DataWrapper*>(selectedIndex.internalPointer());
    int serialNumber = item->number;
    QModelIndex parentIndex;
    if(item->type == TERM)
    {
        parentIndex = QModelIndex();
    }
    else
    {
        parentIndex = selectedIndex.parent();
    }

    //удаление всех "детей" элемента
    QModelIndex indexLevel1, indexLevel2;
    if(item->count > 0)
    {
        foreach (DataWrapper* dwLevel1, item->children) {
            if(dwLevel1->count > 0)
            {
                indexLevel1 = index(dwLevel1->number,0,selectedIndex);
                foreach (DataWrapper* dwLevel2, dwLevel1->children) {
                    if(dwLevel2->count > 0)
                    {
                        indexLevel2 = index(dwLevel2->number,0,indexLevel1);
                        removeRows(0,dwLevel2->count,indexLevel2);
                    }
                }
                removeRows(0, dwLevel1->count,indexLevel1);
            }
        }
        removeRows(0,item->count,selectedIndex);
    }
    //*******************************

    removeRows(serialNumber,1,parentIndex);
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


/*QVariant*/ void LectureModel::grayColor(QUrl url) {
    QImage image;
    bool ok = image.load(url.toLocalFile());
    if (ok == false)
        qDebug()<<"Изображение не считалось";
    if(!image.isNull()) {
        QImage image_gray = image;
        image_gray = image_gray.convertToFormat(QImage::Format_Grayscale8);

        QString path_gray = path(url.toLocalFile(),"_gray");
        /*int pos_gray = path_gray.lastIndexOf('.');
        path_gray= path_gray.mid(0,path_gray.lastIndexOf('.'))+"_gray"+path_gray.mid(pos_gray);*/
        bool ok_gray = image_gray.save(path_gray);
        if (ok_gray == false)
            qDebug()<<"Изображение не сохранилось";
        //return QUrl::fromLocalFile(path_gray);
    }
    // изменить путь изображения в БД
   //return QVariant();
}

QString LectureModel::cutPath(QUrl/*QString*/ url) {
    //int pos = url.indexOf('/') + 2;
    //url = url.mid(pos);
    //return url;
    return url.toLocalFile();
}

/*QVariant*/ void LectureModel::gauss(QUrl url) {
    QImage image;
    int k = 15;
    bool ok = image.load(path(url.toLocalFile(),"_gray"));
    if (ok == false)
        qDebug()<<"Изображение не считалось";
    if(!image.isNull()) {
        int h = image.height();
        int w = image.width();
        QColor p_r,p,p_l;
        int red, green, blue;
        // по горизонтали
        while (k!=0) {
        for (int i=1; i<w-1; i++)
            for (int j=0; j<h; j++)
            {
                p_l = image.pixelColor(i-1,j);
                p = image.pixelColor(i,j);
                p_r = image.pixelColor(i+1,j);

                red = (p_l.red() + p.red() + p_r.red()) / 3;
                green = (p_l.green() + p.green() + p_r.green()) / 3;
                blue = (p_l.blue() + p.blue() + p_r.blue()) / 3;

                QColor p_new(red,green,blue);
                image.setPixelColor(i,j,p_new);
            }
        // по вертикали
        for (int i=0; i<w; i++)
            for (int j=1; j<h-1; j++)
            {
                p_l = image.pixelColor(i,j-1);
                p = image.pixelColor(i,j);
                p_r = image.pixelColor(i,j+1);

                red = (p_l.red() + p.red() + p_r.red()) / 3;
                green = (p_l.green() + p.green() + p_r.green()) / 3;
                blue = (p_l.blue() + p.blue() + p_r.blue()) / 3;

                QColor p_new(red,green,blue);
                image.setPixelColor(i,j,p_new);
            }
        k--;
        }
        QString path_gauss = path(url.toLocalFile(),"_gauss");
        bool ok_gauss = image.save(path_gauss);
        if (ok_gauss == false)
        qDebug()<<"Изображение не сохранилось";
        //return QUrl::fromLocalFile(path_gauss);
    }
   //return QVariant();
}

void LectureModel::printImage(QUrl url)

{
    QPixmap pix;
    pix.load(url.toLocalFile());
    QPrinter printer;
    /*QPrintPreviewDialog *dlg = new QPrintPreviewDialog(&printer,0); // ????
    if(dlg->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        painter.drawPixmap(QPoint(0, 0), pix);
        painter.end();
    }*/
    QPrintDialog *dialog = new QPrintDialog(&printer,0);
    if(dialog->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        painter.drawPixmap(QPoint(0, 0), pix);
        painter.end();
    }
}

QString LectureModel::path(QString p, QString str) {
    QString s = str;
    if (str != "")
    {
        int pos = p.lastIndexOf('.');
        p= p.mid(0,p.lastIndexOf('.'))+str+p.mid(pos);
        return p;
    }
    else
    {
        int pos = p.lastIndexOf('.');
        p= p.mid(0,p.lastIndexOf('.'))+p.mid(pos);
        return p;
    }
}

QVariant LectureModel::division(QUrl url) {
    QImage image_gray, image_gauss, image;
    image.load(url.toLocalFile());
    bool ok_gray = image_gray.load(path(url.toLocalFile(),"_gray"));
    if (ok_gray == false)
        qDebug()<<"Изображение в серых тонах не считалось";
    bool ok_gauss= image_gauss.load(path(url.toLocalFile(),"_gauss"));
    if (ok_gauss == false)
        qDebug()<<"Изображение в серых тонах не считалось";
    if(!image_gray.isNull() && !image_gauss.isNull()) {
        int h = image.height();
        int w = image.width();
        QColor p_gray, p_gauss;
        int red, green, blue;
       for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {
                p_gray = image_gray.pixelColor(i,j);
                p_gauss = image_gauss.pixelColor(i,j);

                if (p_gray.red() == 0)
                    p_gray.setRed(1);
                if (p_gauss.red() == 0)
                    p_gauss.setRed(1);
                if (p_gray.green() == 0)
                    p_gray.setGreen(1);
                if (p_gauss.green() == 0)
                    p_gauss.setGreen(1);
                if (p_gray.blue() == 0)
                    p_gray.setBlue(1);
                if (p_gauss.blue() == 0)
                    p_gauss.setBlue(1);

                red = (p_gray.red() / p_gauss.red() * 255);
                green =  (p_gray.green() / p_gauss.green() * 255);
                blue = (p_gray.blue() / p_gauss.blue() * 255);

                QColor p_new(red,green,blue);
                image.setPixelColor(i,j,p_new);
            }
       QString path_image = path(url.toLocalFile(),"_d");
        bool ok = image.save(path_image);
        if (ok == false)
        qDebug()<<"Изображение не сохранилось";
        return QUrl::fromLocalFile(path_image);
    }
   return QVariant();
}
