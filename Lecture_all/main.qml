import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

ApplicationWindow{
    id: applicationWindow1
    visible: true
    width: 640
    height: 480
    color: "#55aaff"
    title: qsTr("Лекции")

    menuBar: MenuBar {
        /*Menu {
            title: qsTr("Файл")
            MenuItem {
                text: qsTr("&Закрыть")
                onTriggered: Qt.quit();
            }
        }*/
        Menu {
            title: qsTr("Добавить")
            MenuItem {
                text: qsTr("&Добавить предмет")
            }
            MenuItem {
                text: qsTr("&Добавить тему")
            }
            MenuItem {
                text: qsTr("&Добавить изображение")
            }
        }
        Menu{
            title: qsTr("Печать")
            MenuItem{
                text: qsTr("&Печать")}
            MenuItem{
                text: qsTr("&Печать со свойстами")
            }

        }
    }
    TreeView {
            id: treeView
            x: 7
            y: 32
            width: 243
            height: 374
            model: myModel
            TableViewColumn{
                        title:"Курсы"
                        role: "display"
                        width:500
                    }
            /*MouseArea {
                anchors.fill: parent
                id: mouseArea_1
            }*/
        }

        Button {
            id: button_turnright
            x: 504
            y: 418
            width: 28
            height: 27
            iconSource: "right.png"
        }

        Button {
            id: button_turnleft
            x: 346
            y: 418
            width: 28
            height: 27
            iconSource: "left.png"
        }

        Button {
            id: button_cut
            enabled: true
            tooltip: "Выделите изображение"
            x: 397
            y: 418
            width: 83
            height: 27
            text: "Обрезать"
        }
        Button {
            id: button_close
            x:604
            y:7
            width: 28
            height: 27
            iconSource: "close.png"
        }
        Image {
            id: image
            x: 256
            y: 7
            width: 376
            height: 365

            MouseArea {
                // действуем в пределах всего элемента Image
                anchors.fill: parent
                id: mouseArea_2
                anchors.rightMargin: 0
                anchors.bottomMargin: -36
                anchors.leftMargin: 0
                anchors.topMargin: 36
            }
        }

   }
