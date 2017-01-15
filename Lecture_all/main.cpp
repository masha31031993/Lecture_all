#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql>
#include <QTreeView>
#include <QtGui>
#include "databasehandler.h"
#include "searchmodel.h"
#include "lecturemodel.h"
#include <QQmlComponent>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*DataBaseHandler* testhand = new DataBaseHandler("../LecturesDB");
    testhand->printPictures_info();
    qDebug() << testhand->getRowCountOfChild(1);

    QString str_insert = "INSERT INTO my_table(number, address, age) "
                         "VALUES (%1, '%2', %3);";
    str = str_insert.arg("14")
                    .arg("hello world str.")
                    .arg("37");
    b = a_query.exec(str);
    if (!b) {
        qDebug() << "Кажется данные не вставляются, проверьте дверь, может она закрыта?";*/

    /*QApplication a(argc, argv);

  // Вывод формы
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

 return a.exec();*/
// widgets
    /*LectureModel *model = new LectureModel("../LecturesDB");
    //LectureModel model{"../LecturesDB"};
    QTreeView view;
    view.setModel(model);
    view.setWindowTitle("Tree model");
    view.show();*/
// приложение
    LectureModel *ip = new LectureModel("../LecturesDB");
    QQmlEngine engine;
    SearchModel *sm = ip->getSearchModel();
    engine.rootContext()->setContextProperty("myModel", ip);
    engine.rootContext()->setContextProperty("searchModel", sm);
    QQmlComponent component(&engine, QUrl("qrc:/main.qml"));
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    component.create();
    return app.exec();




}
