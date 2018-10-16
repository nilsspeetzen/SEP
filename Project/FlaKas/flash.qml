import QtQuick 2.6
import QtQuick.Controls 2.2

Item{
    id: flash

    property int flashID: 0

    Rectangle {
        x: 500
        y: 200
        width: 100
        height: 50
        Drag.active: dragArea.drag.active
        color: "dodgerblue"

        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.target: parent
        }

        Button {
            width: 14
            height: 14
            x: 43
            y: 0
            text: flashID
            onClicked: {
                root.s_deleteFlash(flashID)
                flash.destroy()
            }
        }
		
		Button {
            width: 14
            height: 14
            x: 43
            y: 36
            text: "i"
            onClicked: {
                var newDataWindow = Qt.createComponent("dataWindow.qml");
                if (newDataWindow.status === Component.Ready) {
                    var dataWindowObject = newDataWindow.createObject(root)
					dataWindowObject.show()
                }
            }
        }

        Button {
            id: vout
            width: 10
            height: 10
            x: 100
            y: 5
            onClicked: {
                root.outConnectID = flashID
                root.outConnectPhase = 2
                root.activeOutConnection = true
                if(root.activeInConnection && (root.inConnectPhase === 2)) {
                    root.s_connectFlashes(root.outConnectID, root.inConnectID, root.outConnectPhase)
                    root.activeInConnection = false
                    root.activeOutConnection = false
                }
            }
        }

        Button {
            id: vin
            width: 10
            height: 10
            x: -10
            y: 5
            onClicked: {
                root.inConnectID = flashID
                root.inConnectPhase = 2
                root.activeInConnection = true
                if(root.activeOutConnection && (root.outConnectPhase === 2)) {
                    root.s_connectFlashes(root.outConnectID, root.inConnectID, root.outConnectPhase)
                    root.activeInConnection = false
                    root.activeOutConnection = false
                }
            }
        }

        Button {
            id: lout
            width: 10
            height: 10
            x: 100
            y: 35
            onClicked: {
                root.outConnectID = flashID
                root.outConnectPhase = 1
                root.activeOutConnection = true
                if(root.activeInConnection && (root.inConnectPhase === 1)) {
                    root.s_connectFlashes(root.outConnectID, root.inConnectID, root.outConnectPhase)
                    root.activeInConnection = false
                    root.activeOutConnection = false
                }
            }
        }

        Button {
            id: lin
            width: 10
            height: 10
            x: -10
            y: 35
            onClicked: {
                root.inConnectID = flashID
                root.inConnectPhase = 1
                root.activeInConnection = true
                if(root.activeOutConnection && (root.outConnectPhase === 1)) {
                    root.s_connectFlashes(root.outConnectID, root.inConnectID, root.outConnectPhase)
                    root.activeInConnection = false
                    root.activeOutConnection = false
                }
            }
        }
    }
}
