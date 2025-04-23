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
        onErrorModuleShow: ErrorModule.showError("errorMessage", true)
    }

    ///CALL THIS LINE TO GO TO ERROR MODULE. TRUE FOR MODAL FALSE FOR POPUP
    //simulation.throw_error("Something broke", false)

    Connections {
        target: simulation
        // function onErrorModuleShow(errorMessage) {
        //     ErrorModule.showError(errorMessage, false) // or true for modal
        // }
    }


    property var graphWindow: null

    /// component for creating graph window
    Component {
        id: graphWindowComponent
        GraphWindowUI {
        }
    }


    id: simulationUI
    width: 800
    height: 600
    title: simulation.name + " - Compartmental Modelling System"
    visible: true

    palette: ThemeManager.palette

    menuBar: Rectangle {
        height: 40
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

                    MenuSeparator {}
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

            RowLayout {
                Layout.fillWidth: false

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
                    color: simulation.isRunning ? Qt.rgba(ThemeManager.palette.mid.r, ThemeManager.palette.mid.g, ThemeManager.palette.mid.b, 0.3) : "transparent"


                    TextInput {
                        anchors.fill: parent
                        anchors.margins: 5
                        verticalAlignment: TextInput.AlignVCenter
                        text: simulation.timeSteps
                        color: ThemeManager.palette.text
                        enabled: !simulation.isRunning
                        validator: IntValidator{}

                        onEditingFinished: {
                            simulation.timeSteps = text;
                        }
                        clip: true
                    }
                }
            }

            ToolButton {
                id: playButton
                icon.name: "Play Simulation"
                icon.source: "qrc:/resources/icons/play_arrow.svg"
                icon.color: ThemeManager.palette.text
                icon.height: 20
                icon.width: 20
                checkable: true
                checked: simulation.isRunning

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: icon.name

                onClicked: {
                    simulation.start();
                }
            }

            ToolButton {
                id: pauseButton
                icon.name: "Pause Simulation"
                icon.source: "qrc:/resources/icons/pause.svg"
                icon.color: ThemeManager.palette.text
                icon.height: 20
                icon.width: 20
                checkable: true
                checked: !simulation.isRunning

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: icon.name

                onClicked: {
                    simulation.pause();
                }
            }

            ToolButton {
                id: resetButton
                icon.name: "Reset Simulation"
                icon.source: "qrc:/resources/icons/reset.svg"
                icon.color: ThemeManager.palette.text
                icon.height: 20
                icon.width: 20

                // checkable: true
                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: icon.name

                onClicked: {
                    simulation.reset();
                }
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
                    icon.source: "qrc:/resources/icons/arrow-right.svg"
                    icon.color: enabled ? ThemeManager.palette.text : ThemeManager.palette.mid
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

                    onClicked: {
                        if (!graphWindow || !graphWindow.visible) {
                            graphWindow = graphWindowComponent.createObject(simulationUI)
                            graphWindow.simulation = simulation // Make sure this line is present
                            graphWindow.show()
                        } else {
                            graphWindow.raise()
                            graphWindow.requestActivate()
                        }
                    }
                }
            }
        }

        SplitView {
            orientation: Qt.Horizontal
            Layout.fillHeight: true
            Layout.fillWidth: true

            // --- Simulation UI ---
            CompartmentCanvasUI {
                id: simulationCanvas
                SplitView.fillWidth: true
                SplitView.minimumWidth: 200
                SplitView.fillHeight: true
            }

            // --- Sidebar ---
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
