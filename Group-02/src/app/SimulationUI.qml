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
                        onTriggered: {
                            ThemeManager.setTheme(modelData.value)
                        }
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

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
            }
        }

        Rectangle {
            id: simulationUI
            Layout.fillHeight: parent
            Layout.fillWidth: parent
            color: ThemeManager.palette.window
            border.color: ThemeManager.palette.shadow


            Repeater {
                model: simulation.compartments

                delegate: CompartmentUI {
                    id: compartmentUI
                    x: modelData.x || 100 + (index % 2) * 200
                    y: modelData.y || 100 + Math.floor(index / 2) * 100
                    color: ThemeManager.palette.base
                    border.color: ThemeManager.palette.shadow
                    compartment: modelData
                }
            }
        }

        // --- Sidebar ---
        SidebarUI {
            simulation: simulation
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
