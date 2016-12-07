import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Item{
    visible: true
    width: 640
    height: 480
    property alias button_cut: button_cut

    TreeView {
            id: treeView
            x: 7
            y: 7
            width: 243
            height: 365
            backgroundVisible: true
            frameVisible: true

        }

        Button {
            id: button_turnright
            x: 513
            y: 400
            width: 28
            height: 27
        }

        Button {
            id: button_turnleft
            x: 345
            y: 400
            width: 28
            height: 27
        }

        Button {
            id: button_cut
            x: 401
            y: 400
            text: qsTr("Обрезать")
            tooltip: "Выделите изображение"
        }

        Image {
            id: image
            x: 256
            y: 7
            width: 376
            height: 365
            //source: "qrc:/qtquickplugin/images/template_image.png"
        }

   }
