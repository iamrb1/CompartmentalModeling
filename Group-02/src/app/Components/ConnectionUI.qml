import QtQuick
import cseg2
import Utilities
import Components

Item {
    id: connectionUI
    property Connection connection
    property Compartment source: connection.source
    property Compartment target: connection.target
    property int compartmentWidth: Constants.compartmentWidth
    property int compartmentHeight: Constants.compartmentHeight

    // Line component
    Rectangle {
        id: lineRect
        x: 0
        y: 0

        // Width is the distance between points
        width: connectionUI.calculateLength()
        height: 4
        color: ThemeManager.palette.text
        antialiasing: true

        // Set transform origin to the start point
        transformOrigin: Item.TopLeft

        transform: Rotation {
            id: rectRotation
            origin.x: 0
            origin.y: 0
            angle: connectionUI.calculateAngle()
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                simulation.sidebarConnection = connection
            }
        }
    }

    // Keep the connection positioned at source
    x: source.x + compartmentWidth / 2
    y: source.y + compartmentHeight / 2

    function calculateLength() {
        const dx = target.x - source.x;
        const dy = target.y - source.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    function calculateAngle() {
        const dx = target.x - source.x;
        const dy = target.y - source.y;
        return Math.atan2(dy, dx) * 180 / Math.PI;
    }
}
