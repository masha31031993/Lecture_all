#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QtSql>


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

    //обновление данных в таблицах
    void        updateParAndNumSubjects_and_themes(int idSubj, int newIdParent, int newNumber);
    bool        updateTerm(int id_subj, int newTerm);
    bool        updateId(int id_subj, int newId);
    bool        updateType(int id_subj, int newType);
    bool        updateName(int id_subj, QString newName);
    bool        updateParentId(int id_subj, int newParentId);
    /*void        deleteFromSubjects_and_themes(int idSubj);
    void        deleteFromPictures_info(int idImage);
    void        deleteFromSubjects_and_themes(int idSubj);
    void        deleteFromPictures_info(int idImage);*/

    //получение свободного id
    int         getFreeIdInS_T();

    //получение нового порядкового номера для Семестра(Term)
    int         getTermSerialNumber(int term);

    //проверака на наличие семестра в БД, возвращает true, если семестр присутствует в БД
    bool        hasTerm(int term);

    //изменение порядкового номера у семестров, больших данного term
    void        changeTermSerialNumber(int term);

    //получение id данного семестра
    int         getIdTerm(QString term);
    
    //получение нового порядкового номера для предмета
    int         getSubjSerialNumber(int idParent);
};

#endif // DATABASEHANDLER_H
