/**
    @file CompartmentCanvasUI.qml
    @author Dominik Leisinger
    @brief represents the content area of the program, where compartments and connections reside
    @comment used ClaudeAI to learn about Flickable and its properties, as well as the need for boundary expansion animation logic
 **/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import cseg2
import Utilities

Rectangle {
    id: root
    color: ThemeManager.palette.window
    border.color: ThemeManager.palette.shadow
    clip: true


    property Simulation simulation: null
    property real expansionRate: 0.2
    property real paddingSize: 75  // Add padding around compartments

    // Content Area Boundaries
    property real minContentX: 0
    property real minContentY: 0
    property real maxContentX: width
    property real maxContentY: height

    // Adjusted Content Area Boundaries
    property real targetMinX: 0
    property real targetMinY: 0
    property real targetMaxX: width
    property real targetMaxY: height

    Component.onCompleted: {
        console.log(simulation)
        updateTargetBoundaries()
    }

    // Update target boundaries based on compartment positions
    function updateTargetBoundaries() {
        if (!simulation) return

        let minX = 0
        let minY = 0
        let maxX = width
        let maxY = height

        // Check all compartments to adjust content area boundaries
        for(let i = 0; i < simulation.compartments.length; i++) {
                    const comp = simulation.compartments[i]

                    // Get the compartment width and height if available
                    const compWidth = comp.width || 100  // Default width if not specified
                    const compHeight = comp.height || 100  // Default height if not specified

                    // Check top-left corner for minimum boundaries
                    if(comp.x < minX) minX = comp.x
                    if(comp.y < minY) minY = comp.y

                    // Check bottom-right corner for maximum boundaries
                    if(comp.x + compWidth > maxX) maxX = comp.x + compWidth
                    if(comp.y + compHeight > maxY) maxY = comp.y + compHeight
                }

        targetMinX = minX - paddingSize
        targetMinY = minY - paddingSize
        targetMaxX = maxX + paddingSize
        targetMaxY = maxY + paddingSize

        // Start animation timer if not already running
        if (!boundaryAnimation.running) {
            boundaryAnimation.start()
        }
    }

    // Timer to gradually animate boundary changes (Expansion is too erratic without)
    Timer {
        id: boundaryAnimation
        interval: 16  // ~60fps
        repeat: true
        running: false

        onTriggered: {
            let changed = false

            // Slightly adjust the boundary according to expansion rate
            if (Math.abs(minContentX - targetMinX) > 0.5) {
                minContentX += (targetMinX - minContentX) * expansionRate
                changed = true
            } else {
                minContentX = targetMinX
            }


            if (Math.abs(minContentY - targetMinY) > 0.5) {
                minContentY += (targetMinY - minContentY) * expansionRate
                changed = true
            } else {
                minContentY = targetMinY
            }


            if (Math.abs(maxContentX - targetMaxX) > 0.5) {
                maxContentX += (targetMaxX - maxContentX) * expansionRate
                changed = true
            } else {
                maxContentX = targetMaxX
            }


            if (Math.abs(maxContentY - targetMaxY) > 0.5) {
                maxContentY += (targetMaxY - maxContentY) * expansionRate
                changed = true
            } else {
                maxContentY = targetMaxY
            }

            updateFlickableContent()

            // Stop animation if we've reached the target
            if (!changed) {
                boundaryAnimation.stop()
            }
        }
    }

    // Update the flickable(scroll) content area based on adjusted boundaries
    function updateFlickableContent() {
        const contentWidth = maxContentX - minContentX
        const contentHeight = maxContentY - minContentY

        scrollView.contentWidth = Math.max(contentWidth, width)
        scrollView.contentHeight = Math.max(contentHeight, height)

        // Adjust content item position to account for negative (Moving left) coordinates
        contentItem.x = -minContentX
        contentItem.y = -minContentY
    }

    // Reset view to initial scale
    function resetView() {
        compartmentContainer.scale = 1.0

        // Reset content boundaries
        targetMinX = 0
        targetMinY = 0
        targetMaxX = width
        targetMaxY = height

        minContentX = targetMinX
        minContentY = targetMinY
        maxContentX = targetMaxX
        maxContentY = targetMaxY

        //Update resetted content area size, then again with current compartment locations
        updateFlickableContent()

        updateTargetBoundaries()
    }


    function zoomIn() {
        compartmentContainer.scale = Math.min(compartmentContainer.scale + 0.1, 2.0)
        updateTargetBoundaries()
    }


    function zoomOut() {
        compartmentContainer.scale = Math.max(compartmentContainer.scale - 0.1, 0.5)
        updateTargetBoundaries()
    }


    Component.onCompleted:
    {
        {
            if (compartment.x === undefined) {
                x = 100 + (index % 2) * 200
            } else {
                x = compartment.x
            }

            if (compartment.y === undefined) {
                y = 100 + Math.floor(index / 2) * 100
            } else {
                y = compartment.y
            }
        }

        updateTargetBoundaries()

    }

    onSimulationChanged: {
        if (simulation) {
            // Connect to simulation changes
            simulation.compartmentsChanged.connect(updateTargetBoundaries)
        }
        updateTargetBoundaries()
    }

    // Flickable provides scrolling functionality
    Flickable {
        id: scrollView
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: parent.height

        flickableDirection: Flickable.HorizontalAndVerticalFlick
        boundsBehavior: Flickable.StopAtBounds

        ScrollBar.vertical: ScrollBar {
            active: scrollView.contentHeight > scrollView.height
            policy: ScrollBar.AsNeeded
        }
        ScrollBar.horizontal: ScrollBar {
            active: scrollView.contentWidth > scrollView.width
            policy: ScrollBar.AsNeeded
        }

        // Content Area
        Item {
            id: contentItem
            width: scrollView.contentWidth
            height: scrollView.contentHeight

            Item {
                id: compartmentContainer
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                scale: 1.0
                transformOrigin: Item.TopLeft

                Repeater {
                    model: simulation ? simulation.compartments : []

                    delegate: CompartmentUI {
                        compartment: modelData
                        parentSimulation: simulation

                        x: modelData.x
                        y: modelData.y

                        // Update content boundaries when this compartment moves
                        onXChanged: root.updateTargetBoundaries()
                        onYChanged: root.updateTargetBoundaries()
                    }
                }

                Repeater {
                    model: simulation ? simulation.connections : []

                    delegate: ConnectionUI {
                        connection: modelData
                        parentSimulation: simulation
                    }
                }
            }
        }
    }

    // Zoom controls
    Row {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 5
        spacing: 5
        z: 10

        Button {
            text: "+"
            width: 30
            height: 30
            onClicked: root.zoomIn()
        }

        Button {
            text: "-"
            width: 30
            height: 30
            onClicked: root.zoomOut()
        }

        Button {
            text: "⟲"
            width: 30
            height: 30
            onClicked: root.resetView()
        }
    }
}
