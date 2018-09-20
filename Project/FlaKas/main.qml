import QtQuick 2.6
import QtQuick.Controls 2.2
import mainwindow 1.0

ApplicationWindow {
    id: root
    width: 1600
    height: 900
    visible: true

    TextField {
        anchors.centerIn: parent
        text: MainWindow.userName
    }
}
