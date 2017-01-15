import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: rect
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter
    width: 370
    height: 132
    visible: false
    color: "#55aaff"
    border.width: 1
    Button {
        id: button_close
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        width: 35
        height: 35
        style: ButtonStyle {
            background: Rectangle {
                color: control.pressed ? "#55aaff"  : "#b5d5ee"
                border.color: "#ffffff"
                radius: 2
                Image {
                    width: 34
                    height: 34
                    source: "image_button/close.png"
                }
            }
        }
        onClicked: rect.visible = false
    }
}

