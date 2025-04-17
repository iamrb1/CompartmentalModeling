import QtQuick
import QtQuick.Layouts
import Utilities

Rectangle {
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
                Item { Layout.fillHeight: true } // Spacer
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
                            }
                        }
                    }

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

                    // Minus button
                    Rectangle {
                        width: 24
                        height: 24
                        radius: 4
                        color: minusMouseArea.pressed ? ThemeManager.palette.mid : "transparent"
                        border.width: 1
                        border.color: ThemeManager.palette.mid

                        Text {
                            anchors.centerIn: parent
                            text: "-"
                            font.pixelSize: 14
                            color: ThemeManager.palette.text
                        }

                        MouseArea {
                            id: minusMouseArea
                            anchors.fill: parent
                            onClicked: {
                                // Remove variable logic would go here
                                console.log("Remove variable")
                            }
                        }
                    }
                }


                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 10
                    rowSpacing: 5

                    Text {
                        text: "k1:"
                        color: ThemeManager.palette.text
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        height: 24
                        border.width: 1
                        border.color: ThemeManager.palette.mid
                        color: "transparent"

                        TextInput {
                            anchors.fill: parent
                            anchors.margins: 2
                            text: "0.01"
                            color: ThemeManager.palette.text
                            verticalAlignment: TextInput.AlignVCenter
                            selectByMouse: true
                        }
                    }

                    Text {
                        text: "k2:"
                        color: ThemeManager.palette.text
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        height: 24
                        border.width: 1
                        border.color: ThemeManager.palette.mid
                        color: "transparent"

                        TextInput {
                            anchors.fill: parent
                            anchors.margins: 2
                            text: "0.1"
                            color: ThemeManager.palette.text
                            verticalAlignment: TextInput.AlignVCenter
                            selectByMouse: true
                        }
                    }

                    Text {
                        text: "k3:"
                        color: ThemeManager.palette.text
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        height: 24
                        border.width: 1
                        border.color: ThemeManager.palette.mid
                        color: "transparent"

                        TextInput {
                            anchors.fill: parent
                            anchors.margins: 2
                            text: "0.5"
                            color: ThemeManager.palette.text
                            verticalAlignment: TextInput.AlignVCenter
                            selectByMouse: true
                        }
                    }
                }
                Item { Layout.fillHeight: true } // Spacer
            }
        }
    }
}
