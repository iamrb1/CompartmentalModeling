import QtQuick
import Utilities
import Components

Item {
    id: connectionUI
    property CompartmentUI source
    property CompartmentUI target

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
            origin.x: 0
            origin.y: 0
            angle: connectionUI.calculateAngle()
        }
    }

    // Keep the connection positioned at source
    x: source.x + source.width / 2
    y: source.y + source.height / 2

    function calculateLength() {
        var dx = target.x + target.width / 2 - (source.x + source.width / 2);
        var dy = target.y + target.height / 2 - (source.y + source.height / 2);
        return Math.sqrt(dx * dx + dy * dy);
    }

    function calculateAngle() {
        var dx = target.x + target.width / 2 - (source.x + source.width / 2);
        var dy = target.y + target.height / 2 - (source.y + source.height / 2);
        return Math.atan2(dy, dx) * 180 / Math.PI;
    }

    // Update the arrow when positions change
    // onXChanged: arrowCanvas.requestPaint()
    // onYChanged: arrowCanvas.requestPaint()
}
