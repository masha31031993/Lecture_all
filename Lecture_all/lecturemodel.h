#ifndef LECTUREMODEL_H
#define LECTUREMODEL_H

#include <QAbstractItemModel>
#include <databasehandler.h>
#include <QModelIndex>
#include <QVariant>


enum h_type{ROOT, TERM = 1, COURSE, THEME, IMAGE};

struct IData
{
    QString name;
    QString comment;
    QString tags;
};

struct DataWrapper
{
    quint16 id;
    h_type type;
    IData *data;
    int number;
    DataWrapper *parent;
    QList<DataWrapper *> children;
    int count;
};

class LectureModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit LectureModel(QString dbPath, QObject *parent = 0);
    ~LectureModel();
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent);
    bool hasChildren(const QModelIndex &parent) const;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //Qt::DropAction supportedDropActions() const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    //Добавление новых строк в БД и модель
    bool insertRows(int row, int count, const QModelIndex &parent);
    Q_INVOKABLE void insertTerm(QString sterm);
    Q_INVOKABLE void insertSubject(QString term, QString newSubject);
    //Q_INVOKABLE void insertTheme(QString term, QString subject, QString newTheme);



private:

    QString dbName;
    DataBaseHandler *dataBase;
    DataWrapper* root;
    int idForInsert;

   static const int INSERT_ID_ROLE = Qt::UserRole + 1;
   static const int INSERT_TYPE_ROLE = Qt::UserRole + 2;
   static const int INSERT_NAME_ROLE = Qt::UserRole + 3;
   static const int INSERT_TAG_ROLE = Qt::UserRole + 4;
   static const int INSERT_COMMENT_ROLE = Qt::UserRole + 5;
   static const int INSERT_TERM_ROLE = Qt::UserRole + 6;
   static const int INSERT_PARENT_ID_ROLE = Qt::UserRole + 7;

    //активируем функцию перетаскивания


};

#endif // LECTUREMODEL_H
