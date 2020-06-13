import QtQuick 2.0

Rectangle {
    id: _root

    property string displayText: ""
    property bool textVisible: false

    color: "steelblue"
    radius: 5
    border.color: "black"
    border.width: 1

    Text {
        id: _text
        anchors.centerIn: _root

        text: _root.displayText
        visible: _root.textVisible
        font {
            pointSize: Math.min(_root.width, _root.height) / 3
            bold: true
        }
    }
}
