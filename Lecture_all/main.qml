import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

ApplicationWindow{
    id: applicationWindow
    visible: true
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
                    image.source = myModel.grayColor(image.source);
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
        id: form
        width: applicationWindow.width
        height: applicationWindow.height
        radius: 0
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        border.width: 0
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
                                        }
                                    }
                                }






                    }



            Rectangle {
                id: r_button
                x: 267
                y: 0
                width: form.width*2/3
                height: 30 //ширина
                color: "#00000000"

                Button {
                    id: button_turn
                    x: 187
                    y: 0
                    width: 28
                    height: 27
                    iconSource: "right.png"
                    enabled: false
                    onClicked: {
                       slider_rotation.visible = true;
                   }

                }

                Button {
                    id: button_cut
                    enabled: false
                    tooltip: "Выделите изображение"
                    x: 98
                    y: 0
                    width: 83
                    height: 27
                    text: "Обрезать"
                    //onclicked:
                }
                Button {
                    id: button_save
                    x: 0
                    y: 0
                    width: 92
                    height: 27
                    text: "Сохранить"
                    enabled: false
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
                    x: 236
                    y: 3
                    width: 107
                    height: 22
                    stepSize: 0.001
                    visible: false
                    onEnabledChanged: {
                        image.rotation = slider_rotation.value;
                    }
                }

            }
            Rectangle {
                id: r_image
                x: 267
                y: 30
                width: form.width*2/3
                height: form.height-30 //ширина
                color: "#00000000"

                Image {
                    id: image
                    anchors.topMargin: 25
                    anchors.bottomMargin: 85
                    anchors.rightMargin: 22
                    anchors.fill: parent
                    smooth: true
                    rotation: slider_rotation.value*360
                    scale: 0.5

                    MouseArea {
                // действуем в пределах всего элемента Image
                anchors.fill: parent
                id: mouseArea_image
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                hoverEnabled: false
                onEntered: {
                        button_cut.enabled = true;
                        button_save.enabled = true;
                        button_turn.enabled = true;
                    }
                }
                }
            }

                Slider {
                    id: sliderHorizontal_image
                    x: 278
                    y: 614
                    width: 511
                    height: 22
                }



                Slider {
                    id: sliderVertical_image
                    x: 767
                    y: 30
                    width: 22
                    height: 585
                    orientation: Qt.Vertical
                    value: 1

                }








        Rectangle {
            id: item_term
            x: 221
            y: 254
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
                    //myModel.insertTerm(textField_term.text);
                    myModel.insertUnit(textField_term.text,1);
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
            x: 195
            y: 201
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
                onClicked: {
                    //myModel.insertSubject(textField_subject.text, treeView.currentIndex);
                    myModel.insertUnit(textField_subject.text,2);
                    item_subject.visible = false;

                }

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


        }

        Rectangle {
            id: item_theme
            x: 221
            y: 254
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
                onClicked: {
                    myModel.insertUnit(textField_theme.text,3);
                    item_theme.visible = false;

                }
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
            x: 195
            y: 201
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
                   y: 44
                   width: 250
                   height: 33
                   id: textField_path
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


    }
}
