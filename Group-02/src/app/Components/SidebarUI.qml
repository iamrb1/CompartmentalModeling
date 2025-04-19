import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utilities
import Components
import cseg2

Rectangle {
    property Simulation simulation: null

    id: sidebarUI

    Layout.fillHeight: true
    width: 200
    Layout.preferredWidth: 200
    color: ThemeManager.palette.base
    border.color: ThemeManager.palette.shadow
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Connection section
        Rectangle {
            id: sidebarTop
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height * 0.5
            color: ThemeManager.palette.base

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Text {
                    text: "Connection"
                    font.bold: true
                    font.pixelSize: 14
                    color: ThemeManager.palette.text
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        text: "Start"
                        color: ThemeManager.palette.text
                        Layout.preferredWidth: 80
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        height: 30
                        border.width: 1
                        border.color: ThemeManager.palette.mid
                        color: "transparent"
                        TextInput {
                            anchors.fill: parent
                            anchors.margins: 5
                            verticalAlignment: TextInput.AlignVCenter
                            text: "A"
                            color: ThemeManager.palette.text
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        text: "End"
                        color: ThemeManager.palette.text
                        Layout.preferredWidth: 80
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        height: 30
                        border.width: 1
                        border.color: ThemeManager.palette.mid
                        color: "transparent"
                        TextInput {
                            anchors.fill: parent
                            anchors.margins: 5
                            verticalAlignment: TextInput.AlignVCenter
                            text: "B"
                            color: ThemeManager.palette.text
                        }
                    }
                }

                Text {
                    text: "Rate of Transfer"
                    color: ThemeManager.palette.text
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 30
                    border.width: 0
                    border.color: ThemeManager.palette.mid
                    color: "transparent"

                    TextInput {
                        anchors.fill: parent
                        text: "k1*A*B"
                        color: ThemeManager.palette.text
                        verticalAlignment: TextInput.AlignVCenter
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: ThemeManager.palette.mid
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    Layout.fillHeight: true
                } // Spacer
            }
        }

        // Divider
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: ThemeManager.palette.shadow
        }

        // Variables section
        Rectangle {
            id: sidebarBottom
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: parent.height * 0.5
            color: ThemeManager.palette.base

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // Header with variable controls
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    // Header Text - now centered with correct layout settings
                    Item {
                        Layout.fillWidth: true
                        height: 24

                        Text {
                            anchors.centerIn: parent
                            text: "Variables"
                            font.bold: true
                            font.pixelSize: 14
                            color: ThemeManager.palette.text
                        }
                    }

                    // Add this right after the Variables header in your QML
                    Item {
                        Layout.fillWidth: true
                        height: 30


                    }

                    // Plus button
                    Rectangle {
                        width: 24
                        height: 24
                        radius: 4
                        color: plusMouseArea.pressed ? ThemeManager.palette.mid : "transparent"
                        border.width: 1
                        border.color: ThemeManager.palette.mid

                        Text {
                            anchors.centerIn: parent
                            text: "+"
                            font.pixelSize: 14
                            color: ThemeManager.palette.text
                        }

                        MouseArea {
                            id: plusMouseArea
                            anchors.fill: parent
                            onClicked: {
                                // Add new variable logic would go here
                                console.log("Add new variable")
                                console.log(simulation.variables)
                                simulation.add_variable()
                                console.log(simulation.variables)
                            }
                        }
                    }
                }

                ColumnLayout {
                    id: variableGrid
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // Example variable rows (these would be dynamically generated)
                    ListView {
                        id: variablesListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true

                        model: simulation ? simulation.variableNames : []

                        delegate: RowLayout {
                            width: parent.width
                            spacing: 10

                            Text {
                                text: modelData
                                color: ThemeManager.palette.text
                                Layout.preferredWidth: 40
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 30
                                border.width: 1
                                border.color: ThemeManager.palette.mid
                                color: "transparent"

                                TextInput {
                                    anchors.fill: parent
                                    anchors.margins: 5
                                    verticalAlignment: TextInput.AlignVCenter
                                    text: simulation.getVariableValue(modelData).toFixed(2)
                                    color: ThemeManager.palette.text
                                    onEditingFinished: {
                                        simulation.update_variable(modelData, parseFloat(text))
                                    }
                                }
                            }

                            Rectangle {
                                width: 24
                                height: 24
                                radius: 4
                                color: removeMouseArea.pressed ? ThemeManager.palette.mid : "transparent"
                                border.width: 1
                                border.color: ThemeManager.palette.mid

                                Text {
                                    anchors.centerIn: parent
                                    text: "-"
                                    font.pixelSize: 14
                                    color: ThemeManager.palette.text
                                }

                                MouseArea {
                                    id: removeMouseArea
                                    anchors.fill: parent
                                    onClicked: {
                                        simulation.remove_variable(modelData)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
