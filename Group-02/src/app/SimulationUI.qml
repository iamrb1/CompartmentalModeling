import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Dialogs
import QtQuick.Shapes

import cseg2
import Utilities
import Components

ApplicationWindow {
    Simulation {
        id: simulation
    }

    id: mainFrameUI
    width: 800
    height: 600
    title: simulation.name + " - Compartmental Modelling System"
    visible: true

    palette: ThemeManager.palette

    menuBar: MenuBar {
        id: menuBar
        Layout.preferredWidth: implicitWidth
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
                        onTriggered: ThemeManager.setTheme(modelData.value)
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

                    onClicked: {
                        simulation.add_compartment()
                    }
                }
                ToolButton {
                    icon.name: "Add connection"
                    icon.source: "qrc:/resources/icons/arrow-right.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    checkable: true

                    onClicked: {
                        simulation.connectionMode = !simulation.connectionMode
                        checked = simulation.connectionMode
                        if (simulation.connectionMode) {
                            connectionMessagePopup.open()
                        }
                    }

                    Popup {
                        id: connectionMessagePopup
                        x: (simulationUI.width - width) / 2
                        y: 0
                        width: 500
                        height: 30

                        contentItem: Rectangle {
                            anchors.fill: parent
                            color: ThemeManager.palette.base

                            Text {
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "Select source and target components to form a connection"
                            }
                        }

                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        // Timer to auto-close the popup
                        Timer {
                            id: autoCloseTimer
                            interval: 2000 // 2 seconds
                            running: false
                            repeat: false
                            onTriggered: connectionMessagePopup.close()
                        }

                        onVisibleChanged: {
                            if (visible) {
                                autoCloseTimer.restart()
                            }
                        }
                    }

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
                ToolButton {
                    icon.name: "Show Graphs"
                    icon.source: "qrc:/resources/icons/graph.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
            }
        }

        // // --- Plot Container ---
        // Rectangle {
        //     id: plotContainer
        //     SplitView.preferredWidth: 400
        //     SplitView.minimumWidth: 0
        //     SplitView.fillHeight: true
        //     color: ThemeManager.palette.base
        //     border.color: ThemeManager.palette.shadow

        //     TimePlotUI {
        //         id: testPlot
        //         width: parent.width - 20
        //         height: parent.height - 20
        //         x: (parent.width - width) / 2
        //         y: (parent.height - height) / 2
        //         title: "Compartmental Model Test Data"

        //         Component.onCompleted: {
        //             testPlot.xAxisTitle = "Time (hours)"
        //             testPlot.yAxisTitle = "Population"
        //             testPlot.seriesAName = "Susceptible"
        //             testPlot.seriesBName = "Infected"
        //             testPlot.seriesCName = "Recovered"

        //             // Hard-coded test data
        //             var testData = [
        //                 {time: 0, a: 1000, b: 10, c: 0},
        //                 {time: 1, a: 950, b: 50, c: 10},
        //                 {time: 2, a: 890, b: 90, c: 30},
        //                 {time: 3, a: 820, b: 120, c: 70},
        //                 {time: 4, a: 740, b: 140, c: 130},
        //                 {time: 5, a: 660, b: 150, c: 200},
        //                 {time: 6, a: 580, b: 150, c: 280},
        //                 {time: 7, a: 500, b: 140, c: 370},
        //                 {time: 8, a: 430, b: 120, c: 460},
        //                 {time: 9, a: 370, b: 100, c: 540},
        //                 {time: 10, a: 320, b: 80, c: 610},
        //                 {time: 11, a: 280, b: 60, c: 670},
        //                 {time: 12, a: 250, b: 40, c: 720},
        //                 {time: 13, a: 220, b: 30, c: 760},
        //                 {time: 14, a: 200, b: 20, c: 790},
        //                 {time: 15, a: 190, b: 10, c: 810}
        //             ];

        //             for (var i = 0; i < testData.length; i++) {
        //                 testPlot.addDataPoint(testData[i].time, testData[i].a, testData[i].b, testData[i].c);
        //             }

        //             testPlot.autoScaleAxes();
        //         }
        //     }
        // }
        SplitView {
            orientation: Qt.Horizontal
            Layout.fillHeight: true
            Layout.fillWidth: true

            // --- Simulation UI ---
            Rectangle {
                id: simulationUI
                SplitView.fillWidth: true
                SplitView.minimumWidth: 200
                SplitView.fillHeight: true
                color: ThemeManager.palette.window
                border.color: ThemeManager.palette.shadow

                Item {
                    id: compartmentContainer
                    clip: true
                    anchors.fill: parent
                    scale: 1.0
                    transformOrigin: Item.Center
                    anchors.centerIn: parent

                    Repeater {
                        model: simulation.compartments

                        delegate: CompartmentUI {
                            compartment: modelData
                            parentSimulation: simulation

                            x: compartment.x || 100 + (index % 2) * 200
                            y: compartment.y || 100 + Math.floor(
                                   index / 2) * 100
                        }
                    }

                    Repeater {
                        model: simulation.connections

                        delegate: ConnectionUI {
                            connection: modelData
                            parentSimulation: simulation
                        }
                    }
                }

                // Add scaling buttons in top right corner
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
                        onClicked: {
                            compartmentContainer.scale = Math.min(
                                        compartmentContainer.scale + 0.1, 2.0)
                        }
                    }

                    Button {
                        text: "-"
                        width: 30
                        height: 30
                        onClicked: {
                            compartmentContainer.scale = Math.max(
                                        compartmentContainer.scale - 0.1, 0.5)
                        }
                    }
                }
            }

            // --- Sidebar ---
            SidebarUI {
                id: sidebarUI
                SplitView.fillHeight: true
                SplitView.preferredWidth: 250
                SplitView.minimumWidth: 200
                SplitView.maximumWidth: 350
                parentSimulation: simulation
            }
        }
    }

    FileDialog {
        id: saveFileDialog
        objectName: "saveFileDialog"
        title: "Save Simulation"
        nameFilters: ["Simulation files (*.sim)"]
        fileMode: FileDialog.SaveFile
        onAccepted: {
            simulation.save_xml(selectedFile)
        }
    }

    FileDialog {
        id: openFileDialog
        objectName: "openFileDialog"
        title: "Open Simulation"
        nameFilters: ["Simulation files (*.sim)"]
        fileMode: FileDialog.OpenFile
        onAccepted: {
            simulation.load_xml(selectedFile)
        }
    }
}
