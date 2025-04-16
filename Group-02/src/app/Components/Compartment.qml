import QtQuick
import QtQuick.Shapes
import Utilities

Rectangle {
    height: 100
    width: 150
    z: 2
    color: ThemeManager.palette.base
    border.color: ThemeManager.palette.text
    radius: 15

    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: parent
    }
}
