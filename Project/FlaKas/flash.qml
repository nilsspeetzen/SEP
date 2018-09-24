import QtQuick 2.6
import QtQuick.Controls 2.2

Rectangle {
    x: 500
    y: 200
    width: 100
    height: 50
    Drag.active: dragArea.drag.active
    color: "green"
    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: parent
    }

    Button {
        id: flashX
        width: 10
        height: 10
    }
}
