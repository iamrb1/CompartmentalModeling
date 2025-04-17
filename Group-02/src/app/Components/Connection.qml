import QtQuick
import QtQuick.Shapes
import Utilities

ShapePath {
    property Compartment source
    property Compartment target

    id: linePath
    strokeWidth: 4
    strokeColor: ThemeManager.palette.text
    fillColor: "transparent"
    capStyle: ShapePath.RoundCap

    startX: source.x + source.width / 2
    startY: source.y + source.height / 2

    PathLine {
        x: target.x + target.width / 2
        y: target.y + target.height / 2
    }
}
