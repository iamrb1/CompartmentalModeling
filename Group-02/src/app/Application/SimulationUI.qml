/**
 @file SimulationUI
 @author Nitish Maindoliya
 **/
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Dialogs
import QtQuick.Shapes
import Qt.labs.folderlistmodel 2.9

import Utilities
import Application
import Components

/// Main window for the entire application, contains every single other UI component and holds logic
ApplicationWindow {
    id: simulationUI
    Simulation {
        id: simulation
    }

    MessageModule {
        id: errorModule
        container: simulationCanvas
        parentSimulation: simulation
    }

    property var graphWindow: null

    /// component for creating graph window
    Component {
        id: graphWindowComponent
        GraphWindowUI {
        }
    }

    width: 800
    height: 600
    title: simulation.name + " - Compartmental Modelling System"
    visible: true

    palette: ThemeManager.palette

    /// Menu bar for loading and saving xml, creating a new simulation, and exiting
    /// Can change theme from light to dark
    menuBar: Rectangle {
        height: 50
        width: parent.width
        color: ThemeManager.palette.base

        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 10

            MenuBar {
                id: menuBar
                Layout.preferredWidth: implicitWidth
                background: Rectangle {
                    color: "transparent"
                }

                Menu {
                    title: "File"
                    Action {
                        text: "New Simulation"
                        onTriggered: simulation.clear()
                    }

                    Action {
                        text: "Open"
                        onTriggered: openFileDialog.open()
                    }

                    Action {
                        text: "Save"
                        onTriggered: simulation.save()
                    }

                    Action {
                        text: "Save As"
                        onTriggered: saveFileDialog.open()
                    }

                    MenuSeparator {
                    }
                    Menu {
                        title: "Theme"
                        Repeater {
                            model: ThemeManager.themeList
                            MenuItem {
                                text: modelData.name
                                checkable: true
                                checked: modelData.value === ThemeManager.theme
                                onTriggered: ThemeManager.setTheme(
                                    modelData.value)
                            }
                        }
                    }
                    MenuSeparator {
                    }
                    Action {
                        text: "Exit"
                        onTriggered: Qt.quit()
                    }
                }

                Menu {
                    title: "Help"
                    Action {
                        text: "About"
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                text: `Current step: ${simulation.currentTime}/${simulation.timeSteps}`
                color: ThemeManager.palette.text
            }

            Item {
                Layout.fillWidth: true
            }

            /// Layout on top right for steps, play, pause and reset buttons
            RowLayout {
                Layout.fillWidth: false

                Text {
                    text: "Step Time(ms):"
                    color: ThemeManager.palette.text
                    Layout.preferredWidth: 80
                }
                Item {
                    width: 7
                    height: 1
                }
                Rectangle {
                    Layout.preferredWidth: 60
                    height: 25
                    border.width: 1
                    border.color: ThemeManager.palette.mid
                    // color: "transparent"
                    color: simulation.isRunning ? Qt.rgba(
                        ThemeManager.palette.mid.r,
                        ThemeManager.palette.mid.g,
                        ThemeManager.palette.mid.b,
                        0.3) : "transparent"

                    TextInput {
                        anchors.fill: parent
                        anchors.margins: 5
                        verticalAlignment: TextInput.AlignVCenter
                        text: simulation.stepTime
                        color: ThemeManager.palette.text
                        enabled: !simulation.isRunning
                        validator: IntValidator {
                            bottom: 0
                        }

                        onEditingFinished: {
                            simulation.stepTime = parseInt(text)
                        }
                        clip: true
                    }
                }

                Text {
                    text: "Time Steps:"
                    color: ThemeManager.palette.text
                    Layout.preferredWidth: 70
                }

                Rectangle {
                    Layout.preferredWidth: 60
                    height: 25
                    border.width: 1
                    border.color: ThemeManager.palette.mid
                    // color: "transparent"
                    color: simulation.isRunning ? Qt.rgba(
                        ThemeManager.palette.mid.r,
                        ThemeManager.palette.mid.g,
                        ThemeManager.palette.mid.b,
                        0.3) : "transparent"

                    TextInput {
                        anchors.fill: parent
                        anchors.margins: 5
                        verticalAlignment: TextInput.AlignVCenter
                        text: simulation.timeSteps
                        color: ThemeManager.palette.text
                        enabled: !simulation.isRunning
                        validator: IntValidator {
                            bottom: 0
                        }

                        onEditingFinished: {
                            if (parseInt(text) < simulation.timeSteps) {
                                simulation.reset()
                            }
                            simulation.timeSteps = text
                        }
                        clip: true
                    }
                }
            }

            ToolButton {
                id: playButton
                icon.name: "Play Simulation"
                icon.source: "/icons/play_arrow.svg"
                icon.color: ThemeManager.palette.text
                icon.height: 20
                icon.width: 20
                checkable: true
                checked: simulation.isRunning

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: icon.name

                onClicked: {
                    simulation.start()
                }
            }

            ToolButton {
                id: pauseButton
                icon.name: "Pause Simulation"
                icon.source: "/icons/pause.svg"
                icon.color: ThemeManager.palette.text
                icon.height: 20
                icon.width: 20
                checkable: true
                checked: !simulation.isRunning

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: icon.name

                onClicked: {
                    simulation.pause()
                }
            }

            ToolButton {
                id: resetButton
                icon.name: "Reset Simulation"
                icon.source: "/icons/reset.svg"
                icon.color: ThemeManager.palette.text
                icon.height: 20
                icon.width: 20

                // checkable: true
                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: icon.name

                onClicked: {
                    simulation.reset()
                }
            }
        }
    }

    /// Layout for the left sidebar
    RowLayout {
        anchors.fill: parent
        spacing: 0

        /// Left sidebar can add compartment, connection, or show the graph
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
                    icon.source: "/icons/plus.svg"
                    icon.color: enabled ? ThemeManager.palette.text : ThemeManager.palette.mid
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name

                    enabled: !simulation.connectionMode && !simulation.isRunning

                    onClicked: {
                        simulation.add_compartment()
                    }
                }
                ToolButton {
                    icon.name: "Add connection"
                    icon.source: "/icons/arrow-right.svg"
                    icon.color: enabled ? ThemeManager.palette.text : ThemeManager.palette.mid
                    icon.height: 20
                    icon.width: 20

                    checkable: true

                    checked: !simulation.isRunning && simulation.connectionMode
                    enabled: !simulation.isRunning

                    onClicked: {
                        simulation.connectionMode = !simulation.connectionMode
                        checked = simulation.connectionMode
                        if (simulation.connectionMode) {
                            simulation.prompt("Select source and target compartments to make a connection", Simulation.INFO, Simulation.TOAST)
                        }
                    }

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
                /// Graph button to open graphwindowUI
                ToolButton {
                    icon.name: "Show Graphs"
                    icon.source: "/icons/graph.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name

                    onClicked: {
                        if (!graphWindow || !graphWindow.visible) {
                            graphWindow = graphWindowComponent.createObject()
                            graphWindow.show()
                        } else {
                            graphWindow.raise()
                            graphWindow.requestActivate()
                        }
                    }
                }
            }
        }

        /// Holds the Canvas to add compartments
        SplitView {
            orientation: Qt.Horizontal
            Layout.fillHeight: true
            Layout.fillWidth: true

            // Canvas
            CompartmentCanvasUI {
                id: simulationCanvas
                SplitView.fillWidth: true
                SplitView.minimumWidth: 200
                SplitView.fillHeight: true
            }

            // Right sidebar
            SidebarUI {
                id: sidebarUI
                SplitView.fillHeight: true
                SplitView.preferredWidth: 250
                SplitView.minimumWidth: 200
                SplitView.maximumWidth: 350
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
