import QtQuick 2.6
import QtQuick.Controls 2.2

ApplicationWindow {
    id: root
    width: 1600
    height: 900
    visible: true

    //Phase: 1=Liquid 2=Gas
    property int currentID: 1
    property int outConnectID: 1
    property int outConnectPhase: 1
    property int inConnectID: 1
    property int inConnectPhase: 2
    property bool activeOutConnection: false
    property bool activeInConnection: true

    signal s_startCascade()
    signal s_addFlash(int id)
    signal s_deleteFlash(int id)
    signal s_connectFlashes(int id1, int id2, int phase)

    Row {
        id: topbuttonrow
        objectName: "row"
        x: 0
        y: 0
        width: 600
        height: 100

        Button {
            id: startButton
            text: "Start"
            onClicked: {
                root.s_startCascade()
            }
        }

        Button {
            id: addFlashButton
            text: "Add Flash"
            onClicked: {
                root.s_addFlash(currentID)
                var newFlash = Qt.createComponent("flash.qml");
                if (newFlash.status === Component.Ready) {
                    var flashObject = newFlash.createObject(root)
                    flashObject.flashID = root.currentID
                    root.currentID +=1
                }
            }
        }
    }

}



