#include "databasehandler.h"
#include <QDebug>

DataBaseHandler::DataBaseHandler(QString name)
{
    dbName = name;
    ConnectDataBase();
}

DataBaseHandler::~DataBaseHandler()
{
    dataBase.close();
}


void DataBaseHandler::ConnectDataBase()
{
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(dbName);

    if (!dataBase.open()) {
          qDebug() << "Не могу открыть БД";
    }
}

QString DataBaseHandler::getDbName()
{
    return dbName;
}

//Возвращает количество строк таблицы, имеющих заданный parentId в соотвтствующем столбце
int DataBaseHandler::getRowCountOfChild(int parentId, int type)
{
    QSqlQuery query;
    if(type < 3)
    {
        if(!query.exec(QString("SELECT count(*) FROM subjects_and_themes WHERE (Id_parent = %1)").arg(QString::number(parentId))))
        {
            return -1;
        }
    }
    else
    {
        if(!query.exec(QString("SELECT count(*) FROM pictures_info WHERE (Id_parent = %1)").arg(QString::number(parentId))))
        {
            return -1;
        }
    }
    int row_count = 0;
    if(query.next())
        row_count = query.value(0).toInt();
    return row_count;
}



void DataBaseHandler::printSubjects_and_themes()
{
    QSqlQuery queryOfSubjects_and_themes;
    if (!queryOfSubjects_and_themes.exec("SELECT * FROM subjects_and_themes")) {
        qDebug() << "Невозможно извлечь данные из subjects_and_themes";
    }
    else
    {
        QSqlRecord rec = queryOfSubjects_and_themes.record();
        int id_subj = 0,
                term = 0,
                type = 0,
                serial_number = 0,
                id_parent = 0;
        QString nameSubj = "";

        qDebug() << "Id_subj"
                 << "\tTerm"
                 << "\tType"
                 << "\tName_subj"
                 << "\tSerial_number"
                 << "\tId_parent\n";
        while (queryOfSubjects_and_themes.next()) {
            id_subj = queryOfSubjects_and_themes.value(rec.indexOf("Id_subj")).toInt();
            term = queryOfSubjects_and_themes.value(rec.indexOf("Term")).toInt();
            type = queryOfSubjects_and_themes.value(rec.indexOf("Type")).toInt();
            serial_number = queryOfSubjects_and_themes.value(rec.indexOf("Serial_number")).toInt();
            id_parent = queryOfSubjects_and_themes.value(rec.indexOf("Id_parent")).toInt();
            nameSubj = queryOfSubjects_and_themes.value(rec.indexOf("Name_subj")).toString();
            qDebug() << id_subj
                     << "\t" << term
                     << "\t" << type
                     << "\t" << nameSubj
                     << "\t" << serial_number
                     << "\t\t" << id_parent<<"\n";
        }
    }
}

void DataBaseHandler::printPictures_info()
{
    QSqlQuery queryOfPictures_info;
    if (!queryOfPictures_info.exec("SELECT * FROM pictures_info")) {
        qDebug() << "Невозможно извлечь данные из pictures_info";
    }
    else
    {
        QSqlRecord rec = queryOfPictures_info.record();
        int id_image = 0,
                number = 0,
                id_parent;
        QString tags = "",
                comment = "",
                image_path = "";

        qDebug() << "Id_image"
                 << "\tTags"
                 << "\tComment"
                 << "\tImage_path"
                 << "\tNumber\n"
                 << "\tId_parent";
        while (queryOfPictures_info.next()) {
            id_image = queryOfPictures_info.value(rec.indexOf("Id_image")).toInt();
            number = queryOfPictures_info.value(rec.indexOf("Number")).toInt();
            tags = queryOfPictures_info.value(rec.indexOf("Tags")).toString();
            comment = queryOfPictures_info.value(rec.indexOf("Comment")).toString();
            image_path = queryOfPictures_info.value(rec.indexOf("Image_path")).toString();
            id_parent = queryOfPictures_info.value(rec.indexOf("Id_parent")).toInt();
            qDebug() << id_image
                     << "\t" << tags
                     << "\t" << comment
                     << "\t" << image_path
                     << "\t\t" << number
                     <<"\t" << id_parent <<"\n";
        }
    }
}

void DataBaseHandler::insertIntoSubjects_and_themes(int idSubj,int term, int type, QString nameSubj, int newNumber, int idParent)
{
    QString str_idSubj = QString::number(idSubj),
            str_term = QString::number(term),
            str_type = QString::number(type),
            str_number = QString::number(newNumber),
            str_idParent = QString::number(idParent);

    QString strInsert, strQuery;
    strInsert = "INSERT INTO subjects_and_themes(Id_subj, Term, Type, Name_subj, Serial_number, Id_parent)"
                "VALUES (%1, %2, %3, '%4', %5, %6);";
    strQuery = strInsert.arg(str_idSubj)
                        .arg(str_term)
                        .arg(str_type)
                        .arg(nameSubj)
                        .arg(str_number)
                        .arg(str_idParent);

    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу вставить строку в subjects_and_themes\n";
    }
}

void DataBaseHandler::insertIntoPictures_info(int idImage, QString tags, QString comment, QString imagePath, int newNumber, int idParent)
{
    QString str_idImage = QString::number(idImage),
            str_number = QString::number(newNumber),
            str_idParent = QString::number(idParent);

    QString strInsert, strQuery;
    strInsert = "INSERT INTO pictures_info(Id_image, Tags, Comment, Image_path, Number, Id_parent)"
                "VALUES (%1, '%2', '%3', '%4', %5, %6);";
    strQuery = strInsert.arg(str_idImage)
                        .arg(tags)
                        .arg(comment)
                        .arg(imagePath)
                        .arg(str_number)
                        .arg(str_idParent);

    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу вставить строку в pictures_info\n";
    }
}

void DataBaseHandler::deleteFromSubjects_and_themes(int idSubj)
{
    QString str_idSubj = QString::number(idSubj);
    QString strDelete, strQuery;
    strDelete = "DELETE FROM subjects_and_themes WHERE (Id_subj = %1);";
    strQuery = strDelete.arg(str_idSubj);

    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу удалить строку из subjects_and_themes\n";
    }
}

void DataBaseHandler::deleteFromPictures_info(int idImage)
{
    QString str_idSubj = QString::number(idImage);
    QString strDelete, strQuery;
    strDelete = "DELETE FROM pictures_info WHERE (Id_image = %1);";
    strQuery = strDelete.arg(str_idSubj);

    QString strTest = "DELETE FROM subjects_and_themes WHERE (Id_subj = 100);";

    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу удалить строку из Pictures_info\n";
    }
}

void DataBaseHandler::updateParAndNumSubjects_and_themes(int idSubj, int newIdParent, int newNumber)
{
    QString str_idSubj = QString::number(idSubj),
            str_number = QString::number(newNumber),
            str_idParent = QString::number(newIdParent);

    QString strUpdate, strQuery;
    strUpdate = "UPDATE subjects_and_themes SET Id_parent = %1, Serial_number = %2 WHERE Id_subj = %3;";
    strQuery = strUpdate.arg(str_idParent)
                        .arg(str_number)
                        .arg(str_idSubj);
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу изменить строку в Subjects_and_themes\n";
    }
}

int DataBaseHandler::getFreeIdInS_T()
{
    int freeId = -1;
    QSqlQuery query;
    if(!query.exec("SELECT MAX(Id_subj) FROM Subjects_and_themes;"))
    {
        return freeId;
    }
    if(query.next())
    {
        freeId = query.value(0).toInt() + 1;
    }
    return freeId;
}

int DataBaseHandler::getTermSerialNumber(int term)
{
    if(term < 1)
    {
        return -1;
    }
    QString strSearch;
    strSearch = "SELECT Id_subj, Term, Serial_number FROM Subjects_and_themes WHERE (Type = 1) and (Term < %1) ORDER BY Term;";

    QString strQuery;
    strQuery = strSearch.arg(term);

    int serialNimber = -2;
    QSqlQuery query;
    if(query.exec(strQuery))
    {
        QSqlRecord rec = query.record();
        if(query.last())
        {
            serialNimber = query.value(rec.indexOf("Serial_number")).toInt() + 1;
        }
    }
    return serialNimber;
}

bool DataBaseHandler::hasTerm(int term)
{
    bool flag = false;
    if(term < 1)
    {
        return flag;
    }

    QString strSearch = "SELECT Term FROM Subjects_and_themes WHERE Term = %1;";
    QString strQuery = strSearch.arg(term);
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу проверить наличие семестра";
        return flag;
    }

    if(query.next())
    {
        flag = true;
    }
    return flag;
}

void DataBaseHandler::changeTermSerialNumber(int term)
{
    QString strUpdate = "UPDATE Subjects_and_themes SET Serial_number = Serial_number + 1 WHERE Term > %1;";
    QString strQuery = strUpdate.arg(term);

    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу обновить порядковый номер семестров";
    }
}

int DataBaseHandler::getIdTerm(QString term)
{
    QString strSelect = "SELECT Id_subj FROM Subjects_and_themes WHERE Term = %1";
    QString strQuery = strSelect.arg(term);

    QSqlQuery query;
    int termId = -1;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу найти id";
        return termId;
    }

    if(query.next())
    {
        termId = query.value(0).toInt();
    }

    return termId;
}

int DataBaseHandler::getSubjSerialNumber(int idParent)
{
    int serialNumber = getRowCountOfChild(idParent, 2);
    return serialNumber;
}

bool DataBaseHandler::updateTerm(int id_subj,int newTerm)
{
    QString strUpdate, strQuery;
    if(id_subj < 1)
    {
        strUpdate = "UPDATE Subjects_and_themes SET Term = %1 WHERE Term = -1;";
        strQuery = strUpdate.arg(QString::number(newTerm));
    }
    else
    {
        strUpdate = "UPDATE Subjects_and_themes SET Term = %1 WHERE Id_subj = %2;";
        strQuery = strUpdate.arg(QString::number(newTerm)).arg(QString::number(id_subj));
    }
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу обновить Term";
        return false;
    }
    return true;
}

bool DataBaseHandler::updateId(int id_subj, int newId)
{
    QString strUpdate, strQuery;
    if(id_subj < 1)
    {
        strUpdate = "UPDATE Subjects_and_themes SET Id_subj = %1 WHERE Id_subj = -1;";
        strQuery = strUpdate.arg(QString::number(newId));
    }
    else
    {
        strUpdate = "UPDATE Subjects_and_themes SET Id_subj = %1 WHERE Id_subj = %2;";
        strQuery = strUpdate.arg(QString::number(newId)).arg(QString::number(id_subj));
    }
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу обновить Id";
        return false;
    }
    return true;
}

bool DataBaseHandler::updateType(int id_subj, int newType)
{
    QString strUpdate, strQuery;
    if(id_subj < 1)
    {
        strUpdate = "UPDATE Subjects_and_themes SET Type = %1 WHERE Type = -1;";
        strQuery = strUpdate.arg(QString::number(newType));
    }
    else
    {
        strUpdate = "UPDATE Subjects_and_themes SET Type = %1 WHERE Id_subj = %2;";
        strQuery = strUpdate.arg(QString::number(newType)).arg(QString::number(id_subj));
    }
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу обновить Type";
        return false;
    }
    return true;
}

bool DataBaseHandler::updateName(int id_subj, QString newName)
{
    QString strUpdate, strQuery;
    if(id_subj < 1)
    {
        strUpdate = "UPDATE Subjects_and_themes SET Name_subj = %1 WHERE Name_subj = '-1';";
        strQuery = strUpdate.arg(newName);
    }
    else
    {
        strUpdate = "UPDATE Subjects_and_themes SET Name_subj = %1 WHERE Id_subj = %2;";
        strQuery = strUpdate.arg(newName).arg(QString::number(id_subj));
    }
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу обновить Name";
        return false;
    }
    return true;
}

bool DataBaseHandler::updateParentId(int id_subj, int newParentId)
{
    QString strUpdate, strQuery;
    if(id_subj < 1)
    {
        strUpdate = "UPDATE Subjects_and_themes SET Id_parent = %1 WHERE Id_parent = -1;";
        strQuery = strUpdate.arg(QString::number(newParentId));
    }
    else
    {
        strUpdate = "UPDATE Subjects_and_themes SET Id_parent = %1 WHERE Id_subj = %2;";
        strQuery = strUpdate.arg(QString::number(newParentId)).arg(QString::number(id_subj));
    }
    QSqlQuery query;
    if(!query.exec(strQuery))
    {
        qDebug() << "Не могу обновить ParentId";
        return false;
    }
    return true;
}
