#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql>
#include <QTreeView>
#include <QtGui>
#include "databasehandler.h"
#include "lecturemodel.h"
#include <QQmlComponent>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DataBaseHandler* testhand = new DataBaseHandler("../LecturesDB");
    //testhand->insertIntoSubjects_and_themes(100,7,3,"NewSubj",10,89);
    //testhand->printPictures_info();
    //testhand->deleteFromPictures_info(133);
    //testhand->printSubjects_and_themes();
    //testhand->deleteFromSubjects_and_themes(100);
    //testhand->insertIntoPictures_info(133, "NewTag", "New Comment", "It is the path", 1,5);
    //testhand->updateParAndNumSubjects_and_themes(3,0,2);
    //testhand->changeTermSerialNumber(3);
    qDebug() << testhand->getSubjSerialNumber(1);





    //qDebug() << testhand->getFreeIdInS_T();
    /*QApplication a(argc, argv);

  // Вывод формы
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

 return a.exec();*/
// widgets
    /*LectureModel *model = new LectureModel("../LecturesDB");
    //LectureModel model{"../LecturesDB"};
    QTreeView view;
    view.setDragEnabled(true);
    view.setAcceptDrops(true);
    view.setDropIndicatorShown(true);
    view.setDragDropMode(QAbstractItemView::InternalMove);
    view.setModel(model);
    view.setWindowTitle("Tree model");
    view.show();*/
// приложение
    /*LectureModel *ip = new LectureModel("../LecturesDB");
    QQmlEngine engine;
    engine.rootContext()->setContextProperty("myModel", ip);
    QQmlComponent component(&engine, QUrl("qrc:/main.qml"));
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    component.create();*/
    return app.exec();



}
