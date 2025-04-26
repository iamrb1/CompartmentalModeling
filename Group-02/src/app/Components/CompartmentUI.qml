/**
 @file CompartmentUI
 @author Nitish Maindoliya
 **/
import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

import Application
import Utilities
import Components

/**
 * UI component for the compartment itself
 */
Rectangle {
    property Compartment compartment: null

    id: compartmentUI

    /// Positioning
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
            /// set source or target and perform action accordingly
            if (simulation.connectionMode) {
                if (simulation.sourceCompartment) {
                    simulation.targetCompartment = compartment;
                    simulation.add_connection();
                } else {
                    simulation.sourceCompartment = compartment;
                }
            } else {
                simulation.sidebarCompartment = compartment;
            }
        }
    }

    /// Actual layout for text within compartment
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

    /**
     * Change the border color for a compartment based on target or source
     */
    function compartmentBorderColor() {
        var color = ThemeManager.palette.text
        if (simulation.sourceCompartment && simulation.sourceCompartment === compartment) {
            color = "blue"
        } else if (simulation.targetCompartment && simulation.targetCompartment === compartment) {
            color = "red"
        }
        return color;
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
