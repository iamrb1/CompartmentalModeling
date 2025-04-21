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
    property Simulation parentSimulation
    property Simulation simulation: parentSimulation

    id: sidebarUI

    color: ThemeManager.palette.base
    border.color: ThemeManager.palette.shadow
    border.width: 1

    SplitView {
        orientation: Qt.Vertical
        anchors.fill: parent
        spacing: 0

        Rectangle {
            SplitView.fillWidth: true
            SplitView.minimumHeight: parent.height*0.3
            SplitView.maximumHeight: parent.height*0.7

            CompartmentEditUI{
                parentSimulation: simulation
                visible: parentSimulation.sidebarCompartment
            }

            ConnectionEditUI{
                parentSimulation: simulation
                visible: parentSimulation.sidebarConnection
            }

            Rectangle {
                visible: !(parentSimulation.sidebarConnection || parentSimulation.sidebarCompartment)
                anchors.fill: parent
                color: ThemeManager.palette.base

                ColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    spacing: 10

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

        // Variables section
        Rectangle {
            id: sidebarBottom
            SplitView.fillWidth: true
            SplitView.minimumHeight: parent.height*0.3
            SplitView.maximumHeight: parent.height*0.7
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
                                console.log(Object.keys(simulation.variables))
                                simulation.add_variable()
                                console.log(Object.keys(simulation.variables))
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
                            ToolButton {
                                id: editToolButton
                                icon.name: "Edit Variable"
                                icon.source: "qrc:/resources/icons/edit.svg"
                                icon.color: ThemeManager.palette.text
                                icon.height: 16
                                icon.width: 16
                                padding: 5

                                background: Rectangle {
                                    color: editToolButton.hovered ? ThemeManager.palette.midlight : "transparent"
                                    border.color: ThemeManager.palette.mid
                                    border.width: 1
                                    radius: 5
                                }

                                ToolTip.visible: hovered
                                ToolTip.delay: 500
                                ToolTip.text: icon.name

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
