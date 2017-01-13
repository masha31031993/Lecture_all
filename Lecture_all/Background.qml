import QtQuick 2.7

Item {
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.102
                color: "#b5d5ee"
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
    }
}

