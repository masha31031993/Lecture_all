import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

ApplicationWindow{
    id: applicationWindow
    visible: true
    width: 700
    height: 540
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
            id: menu_add
            visible: false
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
            MenuItem {
                text: qsTr("&Удалить элемент")
                //onTriggered:
            }

        }

        Menu {
            title: qsTr("Обработка")
            MenuItem {
                text: qsTr("&Оттенки серого")
                onTriggered: {

                    image.source = myModel.grayColor(image.source);


}
            }
            MenuItem {
                text: qsTr("&Гауссовое размытие")
                //onTriggered:
            }
            MenuItem {
                text: qsTr("&Деление")
            }
            MenuItem {
                text: qsTr("&Откатить изменение")
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

    Rectangle {
        id: rectangle1
        radius: 0
        border.width: 0

        anchors.fill: parent
        gradient: Gradient {

            GradientStop {
                position: 0.102
                color: "#cbcbcb"
            }

            GradientStop {
                position: 0.284
                color: "#a8cff7"
            }

            GradientStop {
                position: 0.546
                color: "#55aaff"
            }

            GradientStop {
                position: 0.994
                color: "#3a43d7"
            }
        }

        TreeView {
            id: treeView
            x: 7
            y: 32
            width: 243
            height: 460
            model: myModel
           /* style: TreeViewStyle {
                alternateBackgroundColor: "#55aaff" //цвет полосок
                backgroundColor: "#55aaff" //цвет фона
                        }*/

            TableViewColumn {
                title: "Иерархия"
                role: "display"
                width:500
            }

            MouseArea {
                id: mouseArea_tree
                anchors.rightMargin: 0
                anchors.bottomMargin: 18
                anchors.fill:parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton

                onClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        var index_1 = parent.indexAt(mouse.x, mouse.y);
                        if (index_1.valid) {
                            parent.isExpanded(index_1) ? parent.collapse(index_1) : parent.expand(index_1);
                        }
                    }
                    if (mouse.button === Qt.RightButton) {
                        var index_2 = parent.indexAt(mouse.x, mouse.y);
                        if (index_2.valid) {
                            menu_add.popup();
                        }
                    }

                }

                onDoubleClicked: {
                    var index_image = parent.indexAt(mouse.x, mouse.y);
                    if(treeView.model.data(index_image,1)) {
                        image.source = parent.model.data(index_image,1);
                        label_name.text = parent.model.data(index_image,0);
                    }
                }
        }
        }

        Button {
            id: button_turnright
            x: 530
            y: 0
            width: 28
            height: 27
            iconSource: "right.png"
            onClicked: {
                image.rotation +=90;

            }
            // вызвать добавление в базу нового изображения
            //??? сохранить изображение
        }

        Button {
            id: button_turnleft
            x: 391
            y: 0
            width: 28
            height: 27
            iconSource: "left.png"
            onClicked: {
                //image.rotation -=90
                myModel.rotationLeft(label_name.text);
            }
            // вызвать добавление в базу нового изображения
        }

        Button {
            id: button_cut
            enabled: true
            tooltip: "Выделите изображение"
            x: 433
            y: 0
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
            width: 436
            height: 442
            transformOrigin: Item.Center

          /*  Slider {
                id: sliderHorizontal
                x: 88
                y: 381


            }*/

            /*MouseArea {
                // действуем в пределах всего элемента Image
                anchors.fill: parent
                id: mouseArea_image
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                hoverEnabled: false
                onClicked: {
                    var pos_x = mouse.x;
                    var pos_y = mouse.y;
                }
            }*/
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
                onClicked: {
                    myModel.insertTerm(textField_term.text);
                    item_term.visible=false;
                 //reset
                   }
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
            x: 109
            y: 121
            width: 429
            height: 238
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
                x: 172
                y: 192
                text: qsTr("ОК")
                onClicked: label1.text=textField_subject.text //Берет текст и выводит на label

            }

            Button {
                id: button_close1
                x: 398
                y: 3
                width: 28
                height: 27
                iconSource: "close.png"
                onClicked: item_subject.visible=false
            }

            Label {
                id: label1
                x: 29
                y: 182
                width: 79
                height: 32

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
            x: 109
            y: 121
            width: 429
            height: 238
            border.width: 1
            visible: false
            color: "#55aaff"
            border.color: "#000000"

            Button {
                id: button_ok3
                x: 173
                y: 191
                text: qsTr("ОК")
                //onclicked:
            }
            Button {
                id: button_close3
                x: 393
                y: 8
                width: 28
                height: 27
                iconSource: "close.png"
                onClicked: item_image.visible=false
            }
            TextField {
                   x: 90
                   y: 93
                   width: 250
                   height: 33
                   id: textField_image_comment
                   placeholderText: qsTr("Введите комментарий")
               }
            TextField {
                   x: 90
                   y: 142
                   width: 250
                   height: 33
                   id: textField_image_tags
                   placeholderText: qsTr("Введите тэги")
            }
        }

        Label {
            id: label_name
            x: 8
            y: 498
            width: 242
            height: 27

        }

    }
}

