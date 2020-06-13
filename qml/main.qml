import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    id: _root
    visible: true
    width: 640
    height: 480
    title: qsTr("Memory Game")

    GameArea {
        anchors.fill: parent
    }
}
