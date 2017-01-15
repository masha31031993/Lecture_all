#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql>
#include <QTreeView>
#include <QtGui>
#include "databasehandler.h"
#include "lecturemodel.h"
#include "searchmodel.h"
#include <QQmlComponent>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //DataBaseHandler* testhand = new DataBaseHandler("../LecturesDB");
    //SearchModel* sm = new SearchModel(testhand);
    /*LectureModel *ip = new LectureModel("../LecturesDB");
    SearchModel* sm = ip->getSearchModel();
    QListView view;
    view.setModel(sm);
    view.show();*/
    /*testhand->printPictures_info();
    qDebug() << testhand->getRowCountOfChild(1);

 return a.exec();*/
// widgets
    //LectureModel *model = new LectureModel("../LecturesDB");
    //LectureModel model{"../LecturesDB"};
    /*QTreeView view;
    view.setModel(model);
    view.setWindowTitle("Tree model");
    view.show();*/
// приложение
    LectureModel *ip = new LectureModel("../LecturesDB");
    SearchModel *sm = ip->getSearchModel();
    QQmlEngine engine, engine2;
    engine.rootContext()->setContextProperty("myModel", ip);
    engine.rootContext()->setContextProperty("searchModel", sm);
    QQmlComponent component(&engine, QUrl("qrc:/main.qml"));
    //engine2.rootContext()->setContextProperty("searchModel", sm);
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    component.create();









    return app.exec();




}
