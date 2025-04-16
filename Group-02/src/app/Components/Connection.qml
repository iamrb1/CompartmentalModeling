import QtQuick

Item {
    property Compartment source
    property Compartment target

    Canvas {
        id: lineCanvas
        width: 300
        height: 300

        onPaint: {
            var ctx = getContext("2d");

            ctx.clearRect(0, 0, width, height);

            // Draw black inner line (thinner)
            ctx.beginPath();
            ctx.moveTo(source.x + source.width / 2, source.y + source.height / 2);
            ctx.lineTo(target.x + target.width / 2, target.y + target.height / 2);
            ctx.lineWidth = 2;
            ctx.strokeStyle = "black";
            ctx.stroke();
        }
    }
}
