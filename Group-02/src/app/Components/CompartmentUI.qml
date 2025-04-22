import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

import cseg2
import Utilities
import Components

Rectangle {
    property Compartment compartment: null

    id: compartmentUI

    x: compartment.x || 0
    y: compartment.y || 0

    height: Constants.compartmentHeight
    width: Constants.compartmentWidth
    z: 2
    color: ThemeManager.palette.base
    border.color: compartmentBorderColor()
    radius: 15

    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: parent

        onClicked: {
            if (simulation.connectionMode) {
                if (simulation.sourceCompartment) {
                    simulation.targetCompartment = compartment;
                } else {
                    simulation.sourceCompartment = compartment;
                }
            } else {
                simulation.sidebarCompartment = compartment;
            }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        width: parent.width

        Text {
            Layout.fillWidth: true
            text: compartment && compartment.name + " (" + compartment.symbol + ")"
            font.pixelSize: 14
            color: ThemeManager.palette.text
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            Layout.fillWidth: true
            text: compartment && compartment.currentAmount
            font.pixelSize: 12
            color: ThemeManager.palette.text
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    function compartmentBorderColor() {
        if (simulation.sourceCompartment === compartment){
            return "blue"
        } else if (simulation.targetCompartment === compartment) {
            return "red"
        } else {
            return ThemeManager.palette.text
        }
    }

    onXChanged: {
        if (compartment) {
            compartment.x = x
        }
    }

    onYChanged: {
        if (compartment) {
            compartment.y = y
        }
    }
}
