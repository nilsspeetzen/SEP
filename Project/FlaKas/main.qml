import QtQuick 2.6
import QtQuick.Controls 2.2
import QtDataVisualization 1.2

ApplicationWindow {
    id: root
    width: 1600
    height: 900
    visible: true

    signal s_startOneFlash()

    Column {
        id: col
        width: 600
        height: 408
        objectName: "col"

        Row {
            id: row
            objectName: "row"
            x: 0
            y: 0
            width: 600
            height: 100

            Button {
                id: butt
                text: "OneFlash"
                onClicked: root.s_startOneFlash()
            }

            TextField {
                placeholderText: "kek"
            }
            TextField {
                id: tf
                objectName: "tf"
                property int number: 0;
                text: number

            }
        }
    }
}
