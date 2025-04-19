import QtQuick
import QtQuick.Shapes
import Utilities
import Components

Rectangle {
    property Compartment compartment: null

    id: compartmentUI

    x: compartment.x || 0
    y: compartment.y || 0

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

    Text {
        anchors.centerIn: parent
        text: compartment.name + " (" + compartment.symbol + ")"
        font.pixelSize: 20
        color: ThemeManager.palette.text
    }

    onXChanged: {
        compartment.x = x
    }

    onYChanged: {
        compartment.y = y
    }
}
