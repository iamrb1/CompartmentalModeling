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
            Compartment {
                id: compartment3
                x: 500
                y: 500
            }
            Shape{
                id: connectionCanvas
                anchors.fill: parent

                layer.enabled: true
                layer.smooth: true

                Connection {
                    source: compartment1
                    target: compartment2
                }

                Connection {
                    source: compartment1
                    target: compartment3
                }
            }
        }

        // --- Sidebar ---
        Sidebar {

        }


    }
    signal saveRequested(url fileUrl)
    signal loadRequested(url fileUrl)

    FileDialog {
        id: saveFileDialog
        objectName: "saveFileDialog"
        title: "Save Simulation"
        nameFilters: ["Simulation files (*.xml)"]
        fileMode: FileDialog.SaveFile
        onAccepted: {
            mainFrame.save_simulation(selectedFile)
        }
    }

    FileDialog {
        id: openFileDialog
        objectName: "openFileDialog"
        title: "Open Simulation"
        nameFilters: ["Simulation files (*.xml)"]
        fileMode: FileDialog.OpenFile
        onAccepted: {
            mainFrame.load_simulation(selectedFile)
        }
    }
}
