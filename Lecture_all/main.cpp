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
    /*DataBaseHandler* testhand = new DataBaseHandler("../LecturesDB");
    //testhand->printPictures_info();
    testhand->deleteAllChilds(16);*/


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
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.setDragEnabled(true);
    view.viewport()->setAcceptDrops(true);
    view.setDropIndicatorShown(true);
    view.show();*/
// приложение
    LectureModel *ip = new LectureModel("../LecturesDB");
    QQmlEngine engine;
    engine.rootContext()->setContextProperty("myModel", ip);
    QQmlComponent component(&engine, QUrl("qrc:/main.qml"));
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    component.create();
    return app.exec();
}
