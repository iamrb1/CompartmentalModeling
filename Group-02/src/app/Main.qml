import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Dialogs
import QtQuick.Shapes
import QtCore

import Utilities
import Components

ApplicationWindow {
    id: window
    width: 800
    height: 600
    title: "Compartmental Modelling System"
    visible: true

    palette: ThemeManager.palette

    menuBar: MenuBar {
        id: menuBar
        Layout.fillWidth: true

        z: 10

        background: Rectangle {
            color: ThemeManager.palette.base
        }

        Menu {
            title: "File"
            Action {
                text: "New Simulation"
                onTriggered: console.log("New Simulation")
            }
            Action {
                text: "Open"
                onTriggered: openFileDialog.open()
            }
            Action {
                text: "Save"
                onTriggered: saveFileDialog.open()
            }
            Action {
                text: "Save As"
                onTriggered: saveFileDialog.open()
            }
            MenuSeparator {}
            Menu {
                title: "Theme"
                Repeater {
                    model: ThemeManager.themeList
                    MenuItem {
                        text: modelData.name
                        checkable: true
                        checked: modelData.value === ThemeManager.theme
                        onTriggered: {
                            ThemeManager.setTheme(modelData.value)
                        }
                    }
                }
            }
            MenuSeparator {}
            Action {
                text: "Exit"
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: "Help"
            Action {
                text: "About"
                onTriggered: console.log("About clicked")
            }
        }
    }

    // === Layout ===
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // --- Toolbar ---
        ToolBar {
            id: toolBar
            Layout.fillHeight: true
            width: 40
            z: 10

            background: Rectangle {
                color: ThemeManager.palette.base
            }

            Column {
                anchors.fill: parent
                spacing: 10

                ToolButton {
                    icon.name: "Add compartment"
                    icon.source: "qrc:/resources/icons/plus.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
                ToolButton {
                    icon.name: "Add connection"
                    icon.source: "qrc:/resources/icons/arrow-right.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
            }
        }

        // --- Content Area ---
        Rectangle {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: ThemeManager.palette.window
            border.color: ThemeManager.palette.shadow

            Text {
                anchors.centerIn: parent
                text: "Test"
            }

            Compartment {
                id: compartment1
                x: 10
                y: 10
            }
            Compartment {
                id: compartment2
                x: 200
                y: 250
            }
            Shape{
                id: connectionCanvas
                anchors.fill: parent

                layer.enabled: true
                layer.smooth: true

                ShapePath {
                    id: linePath
                    strokeWidth: 4
                    strokeColor: ThemeManager.palette.text
                    fillColor: "transparent"
                    capStyle: ShapePath.RoundCap

                    startX: compartment1.x + compartment1.width / 2
                    startY: compartment1.y + compartment1.height / 2

                    PathLine {
                        x: compartment2.x + compartment2.width / 2
                        y: compartment2.y + compartment2.height / 2
                    }
                }
            }

            // Canvas {
            //     id: connectionCanvas
            //     anchors.fill: parent

            //     onPaint: {
            //         const ctx = connectionCanvas.getContext("2d")
            //         ctx.clearRect(0, 0, connectionCanvas.width, connectionCanvas.height)

            //         const x1 = compartment1.x + compartment1.width / 2
            //         const y1 = compartment1.y + compartment1.height / 2
            //         const x2 = compartment2.x + compartment2.width / 2
            //         const y2 = compartment2.y + compartment2.height / 2

            //         console.log("Canvas painted", x1, y1, x2, y2)

            //         // Draw inner black line
            //         ctx.strokeStyle = "black"
            //         ctx.lineWidth = 2
            //         ctx.beginPath()
            //         ctx.moveTo(x1, y1)
            //         ctx.lineTo(x2, y2)
            //         ctx.stroke()
            //     }
            // }

            // // Trigger shape repaint when rect1 moves
            // Connections {
            //     target: compartment1
            //     onXChanged: connectionCanvas.requestPaint()
            //     onYChanged: connectionCanvas.requestPaint()
            // }

            // // Trigger shape repaint when rect2 moves
            // Connections {
            //     target: compartment2
            //     onXChanged: connectionCanvas.requestPaint()
            //     onYChanged: connectionCanvas.requestPaint()
            // }
        }

        // --- Sidebar ---
    }

    // === File Dialogs ===
    FileDialog {
        id: openFileDialog
        nameFilters: ["Simulation files (*.sim)"]
        fileMode: FileDialog.OpenFile
    }

    FileDialog {
        id: saveFileDialog
        nameFilters: ["Simulation files (*.sim)"]
        fileMode: FileDialog.SaveFile
    }
}
