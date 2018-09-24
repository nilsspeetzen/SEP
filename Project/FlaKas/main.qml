import QtQuick 2.6
import QtQuick.Controls 2.2

ApplicationWindow {
    id: root
    width: 1600
    height: 900
    visible: true

    signal s_startOneFlash()
    signal s_addFlash()

    Row{
    id: splitscreen
    width: 1600
    height: 900

        Column {
            id: leftcol
            width: 600
            height: 408
            objectName: "col"

            Row {
                id: topbuttonrow
                objectName: "row"
                x: 0
                y: 0
                width: 600
                height: 100

                Button {
                    id: startButton
                    text: "OneFlash"
                    onClicked: {
                        root.s_startOneFlash()
                    }
                }

                Button {
                    id: addButton
                    text: "Add Flash"
                    onClicked: {
                        root.s_addFlash()
                        var newFlash = Qt.createComponent("flash.qml");
                        if (newFlash.status === Component.Ready) {
                            newFlash.setProperty("id", "blub");
                            var flashObject = newFlash.createObject(root);
                        }
                    }
                }
            }

            Popup {
                id: dataPopup
                x: 20
                y: 50
                Text {
                    id: dataDisplay
                    objectName: "dataDisplay"
                    font.pixelSize: 12
                }
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
            }
        }

        Rectangle {
            id: flashArea
            objectName: "flashArea"
            x:200
            y:0
            width: root.width - 200
            height: root.height
            color: "lightgrey"
        }
    }
}



