import QtQuick
import cseg2
import Utilities
import Components

Item {
    id: connectionUI
    property Connection connection
    property Compartment source: connection ? connection.source : null
    property Compartment target: connection ? connection.target : null
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
    x: source ? source.x + compartmentWidth / 2 : 0
    y: source ? source.y + compartmentHeight / 2 : 0

    function calculateLength() {
        if (!(source && target)) {
            return 0;
        }

        const dx = target.x - source.x;
        const dy = target.y - source.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    function calculateAngle() {
        if (!(source && target)) {
            return 0;
        }

        const dx = target.x - source.x;
        const dy = target.y - source.y;
        return Math.atan2(dy, dx) * 180 / Math.PI;
    }
}
