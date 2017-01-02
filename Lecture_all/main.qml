import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0

ApplicationWindow{
    id: applicationWindow
    visible: true
    x: Screen.width/2 - applicationWindow.width/2
    y: Screen.height/2 - applicationWindow.height/2
    width: 800
    height: 700
    title: qsTr("Лекции")


    menuBar: MenuBar {

        /*Menu {
            title: qsTr("Файл")
            MenuItem {
                text: qsTr("&Закрыть")
                onTriggered: Qt.quit();
            }
        }*/
       style: MenuBarStyle {

           background: Rectangle {
               color: "#ffffff"
               //border.color: "#a8cff7"
               //border.width: 1
           }
       }

        Menu {
            id: menu_add
            visible: false
            title: qsTr("Добавить")
            MenuItem {
                id: addTerm
                text: qsTr("&Добавить семестр")
                onTriggered: item_term.visible=true
            }
            MenuItem {
                id: addSubject
                text: qsTr("&Добавить предмет")
                onTriggered: item_subject.visible=true
            }
            MenuItem {
                id: addTheme
                text: qsTr("&Добавить тему")
                onTriggered: item_theme.visible=true
            }
            MenuItem {
                id: addImage
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
                onTriggered:
                    /*image.source = */myModel.grayColor(image.source);
            }
            MenuItem {
                text: qsTr("&Размытие по Гауссу")
                onTriggered:
                    /*image.source = */myModel.gauss(image.source);
            }
            MenuItem {
                text: qsTr("&Деление")
                onTriggered:
                    image.source = myModel.division(image.source);
            }
        }

        Menu{
            title: qsTr("Печать")
            MenuItem {
                text: qsTr("&Печать")
                onTriggered:
                    myModel.print(image.source);
            }
            MenuItem {
                text: qsTr("&Печать со свойстами")
            }
        }
    }

    Rectangle {
        id: form
        width: 800
        height: 700
        anchors.fill: parent
        gradient: Gradient {

            GradientStop {
                position: 0.102
                color: "#ffffff"
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
            x: 0
            y: 0
            width: parent.width/3
            height: parent.height/1.1
            model: myModel

                    TableViewColumn {
                        title: "Иерархия"
                        role: "display"
                        width: 500
                    }

                    MouseArea {
                        id: mouseArea_tree
                        anchors.bottomMargin: 21
                        anchors.fill: parent
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
                                                addSubject.visible = myModel.showMenuItem(index_2,2);
                                                addTheme.visible = myModel.showMenuItem(index_2,3);
                                                addImage.visible = myModel.showMenuItem(index_2,4);
                                                myModel.setIndexFI(index_2);
                                                menu_add.popup();
                                            }
                                        }

                                    }

                        onDoubleClicked: {
                                        var index_image = parent.indexAt(mouse.x, mouse.y);
                                        if(myModel.data(index_image,1)) {
                                            image.source = myModel.data(index_image,1);
                                            slider_image.visible = true;
                                        }
                                    }
                                }






                    }



            Rectangle {
                id: r_button
                anchors.left: treeView.right
                width: form.width*2/3   //ширина
                height: 30
                color: "#00000000"

                Button {
                    id: button_save
                    y: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    width: 92
                    height: 27
                    text: "Сохранить"
                    enabled: false
                }

                Button {
                    id: button_cut
                    anchors.left: button_save.right
                    anchors.leftMargin: 5
                    enabled: false
                    tooltip: "Выделите изображение"
                    width: 92
                    height: 27
                    text: "Обрезать"
                    //onclicked:
                }

                Button {
                    id: button_turn
                    anchors.left: button_cut.right
                    anchors.leftMargin: 5
                    width: 27
                    height: 27
                    iconSource: "right.png"
                    enabled: false
                    onClicked: {
                       slider_rotation.visible = true;
                   }
                }
                //Button {
                    //id: button_close
                    //x:604
                    //y:7
                    //width: 28
                    //height: 27
                    //iconSource: "close.png"
                //}
                Slider {
                    id: slider_rotation
                    anchors.left: button_turn.right
                    anchors.leftMargin: 5
                    width: 107
                    height: 27
                    stepSize: 0.001
                    visible: false
                    onEnabledChanged: {
                        image.rotation = slider_rotation.value;
                    }
                }

                Slider {
                    id: slider_image
                    anchors.left: slider_rotation.right
                    anchors.leftMargin: 5
                    width: 107
                    height: 27
                    value: 0.5
                    maximumValue: 1
                    visible: false
                }
            }

            Rectangle {
                id: r_image
                anchors.top: r_button.bottom
                anchors.left: treeView.right
                width: form.width*2/3
                height: form.height-30 //ширина
                color: "#00000000"

                Image {
                    id: image
                    anchors.fill: parent
                    rotation: slider_rotation.value*360
                    scale: slider_image.value

                    MouseArea {
                        // действуем в пределах всего элемента Image
                        anchors.fill: parent
                        id: mouseArea_image
                        hoverEnabled: false
                        onEntered: {
                            button_cut.enabled = true;
                            button_save.enabled = true;
                            button_turn.enabled = true;
                        }
                    }
                }
            }

        Rectangle {
            id: item_term
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 370
            height: 132
            visible: false
            color: "#55aaff"
            border.width: 1

            Button {
                id: button_ok
                width: 92
                height: 27
                text: qsTr("ОК")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    //myModel.insertTerm(textField_term.text);
                    myModel.insertUnit(textField_term.text,1);
                    item_term.visible=false;
                 //reset
                   }
            }

            Button {
                id: button_close
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                width: 27
                height: 27
                iconSource: "close.png"
                onClicked: item_term.visible=false
            }

            TextField {
                id: textField_term
                width: 305
                anchors.top: button_close.bottom
                anchors.topMargin: 15
                anchors.bottom: button_ok.top
                anchors.bottomMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: qsTr("Введите номер семестра")
            }
        }

        Rectangle {
            id: item_subject
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 370
            height: 132
            visible: false
            color: "#55aaff"
            border.width: 1

         TextField {
                id: textField_subject
                width: 305
                anchors.top: button_close1.bottom
                anchors.topMargin: 15
                anchors.bottom: button_ok1.top
                anchors.bottomMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: qsTr("Введите название предмета")

            }

            Button {
                id: button_ok1
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                width: 92
                height: 27
                text: qsTr("ОК")
                onClicked: {
                    //myModel.insertSubject(textField_subject.text, treeView.currentIndex);
                    myModel.insertUnit(textField_subject.text,2);
                    item_subject.visible = false;

                }

            }

            Button {
                id: button_close1
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                width: 27
                height: 27
                iconSource: "close.png"
                onClicked: item_subject.visible=false
            }


        }

        Rectangle {
            id: item_theme
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 370
            height: 132
            visible: false
            color: "#55aaff"
            border.width: 1

            TextField {
                   id: textField_theme
                   width: 305
                   anchors.top: button_close2.bottom
                   anchors.topMargin: 15
                   anchors.bottom: button_ok2.top
                   anchors.bottomMargin: 15
                   anchors.horizontalCenter: parent.horizontalCenter
                   placeholderText: qsTr("Введите название темы")
               }

            Button {
                id: button_ok2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                width: 92
                height: 27
                text: qsTr("ОК")
                onClicked: {
                    myModel.insertUnit(textField_theme.text,3);
                    item_theme.visible = false;

                }
            }
            Button {
                id: button_close2
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                width: 27
                height: 27
                iconSource: "close.png"
                onClicked: item_theme.visible=false
            }
        }

        Rectangle {
            id: item_image
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 370
            height: 228
            border.width: 1
            visible: false
            color: "#55aaff"
            border.color: "#000000"

            Button {
                id: button_ok3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                width: 92
                height: 27
                text: qsTr("ОК")
                //onclicked:
            }
            Button {
                id: button_close3
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                width: 27
                height: 27
                iconSource: "close.png"
                onClicked: item_image.visible=false
            }

            TextField {
                   id: textField_path
                   width: 305
                   height: 33
                   anchors.horizontalCenter: parent.horizontalCenter
                   anchors.top: button_close3.bottom
                   anchors.topMargin: 15
                   placeholderText: qsTr("Путь к изображению")

                   MouseArea {
                       anchors.fill: parent
                       id: mouseArea_path

                       onDoubleClicked: {
                           fileDialog.visible = true;
                       }
                   }
               }

            FileDialog {
                                id: fileDialog
                                title: "Выбор изображения"
                                folder: shortcuts.home
                                visible: false
                                nameFilters: [ "Изображения (*.jpg *.png *.bmp *gif)", "Все файлы (*)" ]
                                selectedNameFilter: "Изображения (*.jpg *.png *.bmp *gif)"
                                onAccepted: {
                                    textField_path.text = myModel.cutPath(fileDialog.fileUrl.toString());
                                }
            }

            TextField {                   
                   id: textField_image_comment
                   width: 305
                   anchors.bottom: textField_image_tags.top
                   anchors.bottomMargin: 15
                   anchors.top: textField_path.bottom
                   anchors.topMargin: 15
                   anchors.horizontalCenter: parent.horizontalCenter
                   placeholderText: qsTr("Введите комментарий")
            }
            TextField {
                   id: textField_image_tags
                   width: 305
                   height: 33
                   anchors.bottom: button_ok3.top
                   anchors.bottomMargin: 15
                   anchors.horizontalCenter: parent.horizontalCenter
                   placeholderText: qsTr("Введите тэги")
            }

        }


    }
}
