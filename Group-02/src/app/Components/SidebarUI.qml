/**
 * @file SidebarUI.qml
 * @author Rahul Baragur
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Application
import Utilities
import Components


/**
 Layout for the SidebarUI qml
 Comprised of the Interface for Compartments + Connections
 and the Variable section displayed on the right side of the program
 */
Rectangle {
    id: sidebarUI

    color: ThemeManager.palette.base
    border.color: ThemeManager.palette.shadow
    border.width: 1

    SplitView {
        orientation: Qt.Vertical
        anchors.fill: parent
        spacing: 0

        /// Resizeable layout allowing longer Compartment/Connection interface
        Rectangle {
            SplitView.fillWidth: true
            SplitView.minimumHeight: parent.height * 0.4
            SplitView.maximumHeight: parent.height * 0.7

            CompartmentEditUI {
                visible: simulation.sidebarCompartment
            }

            ConnectionEditUI {
                visible: simulation.sidebarConnection
            }

            /// Both UIs instantiated yet visible by selection of compartment or connection

            Rectangle {
                visible: !(simulation.sidebarConnection || simulation.sidebarCompartment)
                anchors.fill: parent
                color: ThemeManager.palette.base

                ColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    spacing: 15
                    anchors.topMargin: 30

                    Rectangle {
                        Layout.fillWidth: true
                        color: "transparent"
                    }

                    Text {
                        text: "Interface"
                        font.bold: true
                        font.pixelSize: 14
                        color: ThemeManager.palette.text
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Text {
                        text: "Select a compartment/connection to edit"
                        font.pixelSize: 12
                        color: ThemeManager.palette.shadow
                        Layout.alignment: Qt.AlignHCenter
                        wrapMode: Text.Wrap
                    }
                }
            }
        }

        /// Variable layout on the bottom of the sidebar
        /// Displays all variables and allows for editing + deletion of variable
        /// Adds variables to a QVariant map
        Rectangle {
            id: sidebarBottom
            SplitView.fillWidth: true
            SplitView.minimumHeight: parent.height * 0.3
            SplitView.maximumHeight: parent.height * 0.7
            color: ThemeManager.palette.base

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10


                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Item {
                        Layout.preferredWidth: 24
                    }


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


                    /// Plus button that adds a variable to the simulaton
                    Rectangle {
                        width: 24
                        height: 24
                        radius: 4
                        color: plusMouseArea.pressed ? ThemeManager.palette.mid : "transparent"
                        border.width: 1
                        border.color: ThemeManager.palette.mid
                        opacity: simulation.isRunning ? 0.5 : 1

                        Text {
                            anchors.centerIn: parent
                            text: "+"
                            font.pixelSize: 14
                            color: ThemeManager.palette.text
                        }

                        MouseArea {
                            id: plusMouseArea
                            anchors.fill: parent
                            enabled: !simulation.isRunning
                            onClicked: {
                                // console.log(Object.keys(simulation.variables))
                                simulation.add_variable()
                                // console.log(Object.keys(simulation.variables))
                            }
                        }
                    }
                }

                /// Variable rows dynamically generated within the UI
                /// Calls update function for the display
                ColumnLayout {
                    id: variableGrid
                    Layout.fillWidth: true
                    Layout.fillHeight: true

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
                                    enabled: !simulation.isRunning

                                    validator: DoubleValidator {
                                    }

                                    /// Editing a variable will update the display
                                    onEditingFinished: {
                                        simulation.update_variable(modelData[0], modelData[0], parseFloat(text))
                                    }
                                    clip: true
                                }
                            }

                            /// Pencil button for editing variable name + value assigned to the name
                            ToolButton {
                                id: editToolButton
                                icon.name: "Edit Variable"
                                icon.source: "qrc:/resources/icons/edit.svg"
                                icon.color: enabled ? ThemeManager.palette.text : ThemeManager.palette.mid
                                icon.height: 16
                                icon.width: 16
                                padding: 5
                                enabled: !simulation.isRunning

                                background: Rectangle {
                                    color: editToolButton.hovered ? ThemeManager.palette.midlight : "transparent"
                                    border.color: ThemeManager.palette.mid
                                    border.width: 1
                                    radius: 5
                                    opacity: simulation.isRunning ? 0.5 : 1
                                }

                                ToolTip.visible: hovered
                                ToolTip.delay: 500
                                ToolTip.text: icon.name

                                onClicked: {
                                    if (!simulation.isRunning) {
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

    /// This is a popup dialog for editing variable name and value
    Dialog {
        id: editDialog
        title: "Edit Variable"

        property bool canOpen: !simulation.isRunning

        // Override the open function to check if we can open
        function open() {
            if (canOpen) {
                visible = true;
            }
        }

        standardButtons: Dialog.NoButton
        modal: true
        closePolicy: Popup.CloseOnEscape
        width: 300

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        property string oldName: ""
        property double currentValue: 0.0

        /*
            Updates the variable selected to edit to its new name and new value
            Calls the Q_INVOKABLE update_variable function
         */
        function updateVariable(oldName, newName, newValue) {
            if (oldName === newName) {
                simulation.update_variable(oldName, newName, newValue)
                return;
            }

            const variables = simulation.variables
            /// check if the new name is already a name of another variable
            /// if so, then give an error
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

                /// Edit the variable name in a textfield instead if preferred
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

                /// Edit the variable value in a textfield if preferred
                TextField {
                    id: valueField
                    Layout.fillWidth: true
                    text: editDialog.currentValue
                    selectByMouse: true
                    validator: DoubleValidator {
                    }
                }
            }

            DialogButtonBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                leftPadding: -11

                /// Delete a variable within the dialog box that pops up
                Button {
                    text: "Delete"
                    DialogButtonBox.buttonRole: DialogButtonBox.DestructiveRole
                    onClicked: {
                        simulation.remove_variable(editDialog.oldName)
                        editDialog.close()
                    }
                }

                /// Cancel button to undo the process initiated
                Button {
                    text: "Cancel"
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                    onClicked: editDialog.reject()
                }

                /// Confirm user choice with the OK button
                Button {
                    text: "OK"
                    implicitWidth: 96
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                    onClicked: {
                        const newName = newNameField.text.trim()
                        const newValue = parseFloat(valueField.text)

                        if (newName !== "" && !isNaN(newValue)) {
                            editDialog.updateVariable(editDialog.oldName, newName, newValue)
                        }
                        /// Updates the variable as long as there is a new value

                        editDialog.accept()
                    }
                }

                alignment: Qt.AlignLeft
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
