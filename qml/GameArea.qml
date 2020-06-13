import QtQuick 2.9
import Game 1.0

GridView {
    id: _root
    model: GameAreamModel {

    }

    cellHeight: height / _root.model.dimension
    cellWidth: width / _root.model.dimension

    delegate: Item {
        id: _backgroundDelegate
        width: _root.cellWidth
        height: _root.cellHeight

        Block {
            id: _block
            property bool needShowText: false

            anchors.fill: _backgroundDelegate
            anchors.margins: 1

            displayText: display
            textVisible: needShowText

            width: _root.cellWidth
            height: _root.cellHeight
        }

        Connections {
            target: _root.model
            onCheckVisibleStatus: {
                _block.needShowText = _root.model.getItemVisibility(index)
            }
        }

        MouseArea {
            anchors.fill: _backgroundDelegate
            onClicked: {
                _root.model.showItem(index)
                _block.needShowText = _root.model.getItemVisibility(index)
            }
        }
    }
}
