import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

ApplicationWindow{
    id: applicationWindow
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
                text: qsTr("&Добавить семестр")
                onTriggered: item_term.visible=true
            }
            MenuItem {
                text: qsTr("&Добавить предмет")
                onTriggered: item_subject.visible=true
            }
            MenuItem {
                text: qsTr("&Добавить тему")
                onTriggered: item_theme.visible=true
            }
            MenuItem {
                text: qsTr("&Добавить изображение")
                onTriggered: item_image.visible=true
            }
        }

        Menu{
            title: qsTr("Печать")
            MenuItem {
                text: qsTr("&Печать")
            }
            MenuItem {
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

        TableViewColumn {
            title:"Иерархия"
            role: "display"
            width:500
        }

        onDoubleClicked: {
            if(model.data(index,1)){
                image.source=model.data(index,1);
            }
        }
    }

    Button {
        id: button_turnright
        x: 504
        y: 418
        width: 28
        height: 27
        iconSource: "right.png"
        //onclicked:
    }

    Button {
        id: button_turnleft
        x: 346
        y: 418
        width: 28
        height: 27
        iconSource: "left.png"

        //onclicked:
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
        //onclicked:
    }

    /*Button {
            id: button_close
            x:604
            y:7
            width: 28
            height: 27
            iconSource: "close.png"
        }*/

    Image {
        id: image
        x: 256
        y: 32
        width: 376
        height: 374

        MouseArea {
            // действуем в пределах всего элемента Image
            anchors.fill: parent
            id: mouseArea_2
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
        }
    }

    Rectangle {
        id: item_term
        x: 132
        y: 160
        width: 377
        height: 132
        visible: false
        color: "#55aaff"
        border.width: 1

        Button {
            id: button_ok
            x: 151
            y: 82
            text: qsTr("ОК")
            //onclicked:
        }

        Button {
            id: button_close
            x: 348
            y: 1
            width: 28
            height: 27
            iconSource: "close.png"
            onClicked: item_term.visible=false
        }

        TextField {
            x: 66
            y: 31
            width: 250
            height: 33
            id: textField_term
            placeholderText: qsTr("Введите номер семестра")
        }


    }

    Rectangle {
        id: item_subject
        x: 136
        y: 165
        width: 377
        height: 132
        visible: false
        color: "#55aaff"
        border.width: 1

     TextField {
            x: 66
            y: 31
            width: 250
            height: 33
            id: textField_subject
            placeholderText: qsTr("Введите название предмета")
        }

        Button {
            id: button_ok1
            x: 151
            y: 82
            text: qsTr("ОК")
            //onclicked:
        }
        Button {
            id: button_close1
            x: 348
            y: 1
            width: 28
            height: 27
            iconSource: "close.png"
            onClicked: item_subject.visible=false
        }
    }

    Rectangle {
        id: item_theme
        x: 129
        y: 151
        width: 377
        height: 132
        visible: false
        color: "#55aaff"
        border.width: 1

        TextField {
               x: 66
               y: 31
               width: 250
               height: 33
               id: textField_theme
               placeholderText: qsTr("Введите название темы")
           }

        Button {
            id: button_ok2
            x: 151
            y: 82
            text: qsTr("ОК")
            //onclicked:
        }
        Button {
            id: button_close2
            x: 348
            y: 1
            width: 28
            height: 27
            iconSource: "close.png"
            onClicked: item_theme.visible=false
        }
    }

    Rectangle {
        id: item_image
        x: 129
        y: 151
        width: 377
        height: 132
        border.width: 1
        visible: false
        color: "#55aaff"
        border.color: "#000000"

        Button {
            id: button_ok3
            x: 151
            y: 82
            text: qsTr("ОК")
            //onclicked:
        }
        Button {
            id: button_close3
            x: 348
            y: 1
            width: 28
            height: 27
            iconSource: "close.png"
            onClicked: item_image.visible=false
        }
    }
}
