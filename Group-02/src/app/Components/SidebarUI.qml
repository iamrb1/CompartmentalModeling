/**
 * @file SidebarUI.qml
 * @author Rahul Baragur
 */

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


                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10


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
                                simulation.add_variable()
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
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: simulation ? Object.entries(simulation.variables) : []

                        delegate: RowLayout {
                            width: variableGrid.width
                            spacing: 10

                            Text {
                                text: modelData[0]
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
                                    text: modelData[1]
                                    color: ThemeManager.palette.text
                                    onEditingFinished: {
                                        simulation.update_variable(modelData[0], modelData[0], parseFloat(text))
                                    }
                                    clip: true
                                }
                            }

                            // Pencil button for editing variable name
                            Rectangle {
                                width: 24
                                height: 24
                                radius: 4
                                color: editMouseArea.pressed ? ThemeManager.palette.mid : "transparent"
                                border.width: 1
                                border.color: ThemeManager.palette.mid

                                Text {
                                    anchors.centerIn: parent
                                    text: "✎" // Pencil Unicode character
                                    font.pixelSize: 14
                                    color: ThemeManager.palette.text
                                }

                                MouseArea {
                                    id: editMouseArea
                                    anchors.fill: parent
                                    onClicked: {
                                        editDialog.oldName = modelData[0]
                                        editDialog.currentValue = modelData[1]
                                        editDialog.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Dialog for editing variable name and value
    Dialog {
        id: editDialog
        title: "Edit Variable"

        standardButtons: Dialog.NoButton
        modal: true
        closePolicy: Popup.CloseOnEscape
        width: 300

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        property string oldName: ""
        property double currentValue: 0.0

        function updateVariable(oldName, newName, newValue) {
            if (oldName === newName) {
                simulation.update_variable(oldName, newName, newValue)
                return;
            }

            const variables = simulation.variables
            for (let name in variables) {
                if (name === newName) {
                    errorDialog.message = "A variable with name '" + newName + "' already exists."
                    errorDialog.open()
                    return
                }
            }

            simulation.update_variable(oldName, newName, newValue)
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 15

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Name:"
                    color: ThemeManager.palette.text
                    Layout.preferredWidth: 60
                }

                TextField {
                    id: newNameField
                    Layout.fillWidth: true
                    text: editDialog.oldName
                    selectByMouse: true
                }
            }

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Value:"
                    color: ThemeManager.palette.text
                    Layout.preferredWidth: 60
                }

                TextField {
                    id: valueField
                    Layout.fillWidth: true
                    text: editDialog.currentValue.toFixed(2)
                    selectByMouse: true
                    validator: DoubleValidator {}
                }
            }

            DialogButtonBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight

                Button {
                    text: "Delete"
                    DialogButtonBox.buttonRole: DialogButtonBox.DestructiveRole
                    onClicked: {
                        simulation.remove_variable(editDialog.oldName)
                        editDialog.close()
                    }
                }

                Button {
                    text: "Cancel"
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                    onClicked: editDialog.reject()
                }

                Button {
                    text: "OK"
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                    onClicked: {
                        const newName = newNameField.text.trim()
                        const newValue = parseFloat(valueField.text)

                        if (newName !== "" && !isNaN(newValue)) {
                            editDialog.updateVariable(editDialog.oldName, newName, newValue)
                        }

                        editDialog.accept()
                    }
                }

                alignment: Qt.AlignRight
            }
        }
    }

    /// Error Dialog box for when the name is same
    Dialog {
        id: errorDialog
        title: "Error"
        standardButtons: Dialog.Ok
        modal: true
        closePolicy: Popup.CloseOnEscape

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        width: 300

        property string message: ""

        Label {
            anchors.fill: parent
            text: errorDialog.message
            wrapMode: Text.WordWrap
            color: ThemeManager.palette.text
        }
    }
}
