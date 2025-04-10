import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import QtCore

import Utilities

Window {
    id: window
    width: 800
    height: 600
    title: "Compartmental Modelling System"
    visible: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MenuBar {
            Layout.fillWidth: true

            background: Rectangle {
                color: ThemeManager.theme.background
            }

            Menu {
                title: "File"
                MenuItem {
                    text: "New Simulation"
                    onTriggered: console.log("New Simulation")
                }
                MenuItem {
                    text: "Open"
                    onTriggered: console.log("Open Simulation")
                }
                MenuItem {
                    text: "Save"
                    onTriggered: console.log("Save Simulation")
                }
                MenuItem {
                    text: "Save As"
                    onTriggered: console.log("Save Simulation As")
                }
                MenuSeparator {
                    contentItem: Rectangle {
                        color: ThemeManager.theme.accentForeground
                        height: 1
                    }
                }
                MenuItem {
                    text: "Exit"
                    onTriggered: Qt.quit()
                }
            }

            Menu {
                title: "Help"
                MenuItem {
                    text: "About"
                    onTriggered: console.log("About clicked")
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: rowLayout

            Rectangle {
                width: 40
                Layout.fillHeight: true
                color: ThemeManager.theme.background

                Column {
                    Layout.fillHeight: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20

                    ToolButton {
                        icon.name: "document-new"
                        onClicked: console.log("New clicked")
                    }
                    ToolButton {
                        icon.name: "document-open"
                        onClicked: console.log("Open clicked")
                    }
                }
            }
        }
    }
}
