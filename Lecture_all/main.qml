import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0
import QtQml.Models 2.2

ApplicationWindow
{
    id: applicationWindow
    visible: true
    x: Screen.width/2 - applicationWindow.width/2
    y: Screen.height/2 - applicationWindow.height/2
    width: 830
    height: 700
    title: qsTr("Лекции")
    menuBar: MenuBar {
        style: MenuBarStyle {
            background: Rectangle {
                color: "#ffffff"
                border.color: "#b5d5ee"
                border.width: 1
            }
        }
        /*Menu {
            title: qsTr("Файл")
            MenuItem {
                text: qsTr("&Закрыть")
                onTriggered: Qt.quit()
            }
        }*/
        Menu {
            id: menuAdd
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
                onTriggered: myModel.removeUnit()
            }
        }
        Menu {
            id: menuPrintImage
            enabled: false
            title: qsTr("Печать")
            MenuItem {
                text: qsTr("&Печать изображения")
                onTriggered: myModel.printImage(image.source)
            }
            MenuItem {
                text: qsTr("&Печать со свойстами")
                onTriggered: item_print.visible = true
            }
        }
    }
    TabView {
        id: tabView
        anchors.fill: parent
        style: TabViewStyle {
            frameOverlap: 1
            tabBar: Rectangle {
                color: "#ffffff"
                border.color: "#b5d5ee"
                border.width: 1
            }
            tab: Rectangle {
                color: styleData.selected ? "#b5d5ee" :"#ffffff" //1-выбор
                border.color:  "#ffffff"
                radius: 1
                implicitWidth: tabView.width/tabView.count
                implicitHeight: 25
                Text {
                    anchors.centerIn: parent
                    text: styleData.title
                    color: "black"
                }
            }
        }
        Tab {
            id: image_one
            title: "Изображение"
            Background {
                id: form_image_one
                TreeView {
                    id: treeView
                    x: 0
                    y: 0
                    width: parent.width/3
                    height: parent.height/1.1
                    model: myModel
                    selection: ItemSelectionModel {
                              model: myModel
                         }
                    TableViewColumn {
                        title: "Оглавление"
                        role: "display"
                        width: 500
                    }
                    MouseArea {
                        id: mouseArea_tree
                        anchors.bottomMargin: 21
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.RightButton | Qt.LeftButton
                        onEntered:
                            if (myModel.rowCount(treeView.rootIndex) === 0)
                                item_term.visible = true
                        onClicked: {
                            switch (mouse.button) {
                            case Qt.LeftButton:
                                var index_1 = parent.indexAt(mouse.x, mouse.y)
                                if (index_1.valid) {
                                    parent.isExpanded(index_1) ? parent.collapse(index_1) : parent.expand(index_1)
                                    if(myModel.data(index_1,1)) {
                                        image.source = myModel.data(index_1,1)
                                        myModel.setIndexOpenImage(index_1)
                                        button_improve.enabled = true
                                        slider_image.visible = true
                                    }
                                }
                                break
                            case Qt.RightButton:
                                var index_2 = parent.indexAt(mouse.x, mouse.y)
                                if (index_2.valid) {
                                    addSubject.visible = myModel.showMenuItem(index_2,2)
                                    addTheme.visible = myModel.showMenuItem(index_2,3)
                                    addImage.visible = myModel.showMenuItem(index_2,4)
                                    myModel.setSelIndex(index_2)
                                    menuAdd.popup()
                                }
                                break
                            default: break
                            }
                        }
                    }
                }
                Rectangle {
                    id: r_button
                    anchors.left: treeView.right
                    width: form_image_one.width*2/3   //ширина
                    height: 30
                    color: "#00000000" // прозрачность
                    Button {
                        id: button_improve
                        y: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        width: 92
                        height: 27
                        text: "Улучшить"
                        enabled: false
                        onClicked: {
                            image.source = myModel.improveImage(image.source)
                        }
                    }
                    Button {
                        id: button_save
                        y: 0
                        anchors.left: button_improve.right
                        anchors.leftMargin: 3
                        width: 92
                        height: 27
                        text: "Сохранить"
                        enabled: false
                        onClicked: {
                            image.source = myModel.save(image.source, image.scale)
                        }
                    }
                    Button {
                        id: button_cut
                        anchors.left: button_save.right
                        anchors.leftMargin: 3
                        enabled: false
                        width: 92
                        height: 27
                        text: "Обрезать"
                        property bool click: false
                        onClicked: {
                            image.source = myModel.cut(canvas.firstX,canvas.firstY,canvas.lastX,canvas.lastY,image.source)
                            button_save.enabled = true
                            button_turn.enabled = true
                            click = true
                            canvas.requestPaint()
                        }
                    }
                    Button {
                        id: button_turn
                        anchors.left: button_cut.right
                        anchors.leftMargin: 3
                        width: 27
                        height: 27
                        iconSource: "image_button/right.png"
                        enabled: false
                        onClicked: slider_rotation.visible = true
                    }
                    Slider {
                        id: slider_rotation
                        anchors.left: button_turn.right
                        anchors.leftMargin: 3
                        width: 107
                        height: 27
                        stepSize: 0.001
                        visible: false
                        onEnabledChanged: image.rotation = slider_rotation.value
                    }
                    Slider {
                        id: slider_image
                        width: 107
                        height: 27
                        anchors.left: slider_rotation.right
                        anchors.leftMargin: 3
                        maximumValue: 1
                        minimumValue: 0.4
                        visible: false
                    }
                }
                Rectangle {
                    id: r_image
                    anchors.top: r_button.bottom
                    anchors.left: treeView.right
                    width: form_image_one.width*2/3
                    height: form_image_one.height-30
                    color: "#00000000"
                    ScrollView {
                        id: scrollView
                        anchors.fill: parent
                        Image {
                            id: image
                            rotation: slider_rotation.value*360
                            scale: slider_image.value
                            mipmap: true
                            fillMode: Image.PreserveAspectFit
                            Canvas {
                                id: canvas
                                anchors.fill: parent
                                property int firstX: 0
                                property int firstY: 0
                                property int lastX: 0
                                property int lastY: 0
                                onPaint: {
                                    var ctx = getContext("2d")
                                    if (button_cut.click == true)
                                        ctx.clearRect(0,0,canvas.width,canvas.height)
                                    else {
                                        ctx.lineWidth = 1
                                        ctx.clearRect(0,0,canvas.width,canvas.height)
                                        ctx.beginPath()
                                        ctx.rect(firstX,firstY,lastX - firstX,lastY - firstY)
                                        ctx.stroke()
                                    }
                                }
                            }
                            MouseArea {
                                anchors.fill: parent // действуем в пределах всего элемента Image
                                id: mouseArea_image
                                hoverEnabled: false
                                onPressed: {
                                    button_cut.click = false
                                    button_cut.enabled = true
                                    canvas.firstX = mouseX
                                    canvas.firstY = mouseY
                                }
                                onPositionChanged: {
                                    canvas.lastX = mouseX
                                    canvas.lastY = mouseY
                                    canvas.requestPaint()
                                }
                                onReleased: {
                                    canvas.lastX = (mouseX > image.paintedWidth) ? image.paintedWidth : mouseX
                                    canvas.lastY = (mouseY > image.paintedHeight) ? image.paintedHeight : mouseY
                                    canvas.lastX = (canvas.lastX < canvas.firstX) ? 0 : canvas.lastX
                                    canvas.lastY = (canvas.lastY < canvas.firstY) ? 0 : canvas.lastY
                                    if (canvas.lastX - canvas.firstX < 0) {
                                        canvas.lastX = canvas.firstX
                                        canvas.firstX = 0
                                    }
                                    if (canvas.lastY - canvas.firstY < 0) {
                                        canvas.lastY = canvas.firstY
                                        canvas.firstY = 0
                                    }
                                    canvas.requestPaint()
                                }
                            }
                        }
                    }
                }

                Label {
                    id: text1
                    x: 5
                    y: 650
                    width: 50
                    height: 30
                    font.pixelSize: 12
                    color: "black"
                }
            }
        }
        Tab {
            id: search
            title: "Поиск изображений"
            Background {
                id: form_search
                Rectangle {
                    id: search_button
                    width: parent.width
                    height: 37
                    color: "#00000000"
                    TextField {
                        id: textField_search
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        width: parent.width - button_search.width - 30
                        height: 27
                        placeholderText: qsTr("Введите необходимые тэги для поиска")
                    }
                    Button {
                        id: button_search
                        anchors.left: textField_search.right
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        width: 92
                        height: 27
                        text: qsTr("Поиск")
                    }
                }
                /*GridView {
                    id: gridView1
                    anchors.top: search_button.bottom
                    width: parent.width
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.topMargin: 10
                    cellHeight: 550
                    cellWidth: 300
                    model:  myModel // !!! выводит столько же картинок (вставленных для проверки) сколько и семестров
                    /*ColumnLayout {

                    }
                    delegate: Column {
                        Image {
                            width: 250
                            height: 500
                            source: "image_button/close.png"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            text: qsTr("Cеместр")
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            text: qsTr("Предмет")
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            text: qsTr("Тема")
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            text: qsTr("Название")
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        spacing: 5
                    }
                }*/
            }
        }
    }
    DialogRect {
        id: item_term
        Button {
            id: button_ok
            width: 92
            height: 27
            text: qsTr("ОК")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                //myModel.insertTerm(textField_term.text)
                myModel.insertUnit(textField_term.text,1)
                item_term.visible=false
                //reset
            }
        }
        TextField {
            id: textField_term
            width: 305
            height: 33
            //anchors.top: button_close1.bottom
            //anchors.topMargin: 15
            anchors.bottom: button_ok.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr("Введите номер семестра")
        }
    }
    DialogRect {
        id: item_subject
        TextField {
            id: textField_subject
            width: 305
            height: 33
            //anchors.top: button_close1.bottom
            //anchors.topMargin: 15
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
                myModel.insertUnit(textField_subject.text,2)
                item_subject.visible = false
            }
        }
    }
    DialogRect {
        id: item_theme
        TextField {
            id: textField_theme
            width: 305
            height: 33
            //anchors.top: button_close2.bottom
            //anchors.topMargin: 15
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
                myModel.insertUnit(textField_theme.text,3)
                item_theme.visible = false
            }
        }
    }
    Rectangle {
        id: item_image
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: 370
        height: 228
        visible: false
        color: "#55aaff"
        border.width: 1
        Button {
            id: button_close3
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            width: 27
            height: 27
            iconSource: "image_button/close.png"
            onClicked: item_image.visible = false
        }
        Button {
            id: button_ok3
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width: 92
            height: 27
            text: qsTr("ОК")
            onClicked: myModel.insertImage(textField_path.text,textField_image_comment.text,textField_image_tags.text)
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
                   onDoubleClicked: fileDialog.visible = true
               }
        }
        FileDialog {
            id: fileDialog
            title: "Выбор изображения"
            folder: shortcuts.home
            visible: false
            nameFilters: [ "Изображения (*.jpg *.png *.bmp *gif)", "Все файлы (*)" ]
            selectedNameFilter: "Изображения (*.jpg *.png *.bmp *gif)"
            onAccepted: textField_path.text = myModel.cutPath(fileDialog.fileUrl.toString())
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
    Rectangle {
        id: item_print
        width: 400
        height: 200
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        color: "#55aaff"
        border.width: 1
        Button {
            id: button_close4
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            width: 27
            height: 27
            iconSource: "image_button/close.png"
            onClicked: item_print.visible = false
        }
        TextField {
               id: textField_print
               width: 330
               height: 33
               anchors.top: button_close4.bottom
               anchors.topMargin: 15
               anchors.horizontalCenter: parent.horizontalCenter
               placeholderText: qsTr("Введите названия нужных тем для печати")
           }

        Button {
            id: button_ok4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width: 92
            height: 27
            text: qsTr("ОК")
        }
    }
}



