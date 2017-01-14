#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QtSql>
#include <QFile>
#include <QDebug>

class DataBaseHandler
{
private:
    QSqlDatabase    dataBase;
    QString         dbName;

    void        ConnectDataBase();      //подключение базы данных

public:
    DataBaseHandler(QString name);
    ~DataBaseHandler();

    //Получение имени таблицы
    QString     getDbName();

    // Возвращает количество строк таблицы,
    // имеющих заданный parentId в соотвтствующем столбце
    int         getRowCountOfChild(int parentId, int type);

    //Печать таблиц
    void        printSubjects_and_themes();
    void        printPictures_info();

    //добавление данных в таблицы
    void        insertIntoSubjects_and_themes(int idSubj, int term,  int type, QString nameSubj, int number, int idParent);
    void        insertIntoPictures_info(int idImage, QString tags, QString comment, QString imagePath, int newNumber, int idParent);

    //удаление данных из таблиц
    void        deleteFromSubjects_and_themes(int idSubj);
    void        deleteFromPictures_info(int idImage);

    //удаление "детей" по Id_parent
    void        deleteChildsInSubj_and_themes(int parentId);
    void        deleteChildsInPicture_info(int parentId);

    //удаление всех "детей" по Id_parent
    void        deleteAllChilds(int parentId);

    //обновление данных в таблицах
    void        updateParAndNumSubjects_and_themes(int idSubj, int newIdParent, int newNumber);
    bool        updateTerm(int idSubj, int newTerm);
    bool        updateId(int id, int newId, int type);
    bool        updateType(int idSubj, int newType);
    bool        updateName(int id, QString newName, int type);
    bool        updateParentId(int id, int newParentId, int type);
    bool        updateTagP_I(int idImage, QString newTag);
    bool        updateCommentP_I(int idImage, QString newComment);

    //получение свободного id
    int         getFreeIdInS_T();
    int         getFreeIdPic_Inf();

    //получение нового порядкового номера для Семестра(Term)
    int         getTermSerialNumber(int term);

    //проверака на наличие семестра в БД, возвращает true, если семестр присутствует в БД
    bool        hasTerm(int term);

    //изменение порядкового номера у семестров, больших данного term
    void        changeTermSerialNumber(int term);

    //уменьшение порядковых номеров для "детей" заданного "родителя".
    //number - значение порядкового номера. Все значения, больше данного будут уменьшены на 1.
    //type - "тип родителя".
    void        decrimentSerialNimber(int parentId, int number, int type);

    //получение id данного семестра
    int         getIdTerm(QString term);

    //получение нового порядкового номера для предмета
    int         getSubjSerialNumber(int idParent);
};

#endif // DATABASEHANDLER_H
