/**
 @file CompartmentEditUI
 @author Nitish Maindoliya
 **/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Application
import Utilities
import Components

/**
 * Actual layout for sidebar compartment properties UI
 */
Rectangle {
    property Compartment selectedCompartment: simulation.sidebarCompartment

    id: sidebarTop
    anchors.fill: parent
    color: ThemeManager.palette.base

    /// Layout within the sidebar interface area
    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 10

        Item {
            Layout.preferredHeight: 10
        }

        Text {
            text: "Compartment"
            font.bold: true
            font.pixelSize: 14
            color: ThemeManager.palette.text
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Name"
                color: ThemeManager.palette.text
                Layout.preferredWidth: 80
            }
            /// Edit box for name
            Rectangle {
                Layout.fillWidth: true
                height: 30
                border.width: 1
                border.color: ThemeManager.palette.mid
                // color: "transparent"
                color: simulation.isRunning ? Qt.rgba(ThemeManager.palette.mid.r, ThemeManager.palette.mid.g, ThemeManager.palette.mid.b, 0.3) : "transparent"
                TextInput {
                    anchors.fill: parent
                    anchors.margins: 5
                    verticalAlignment: TextInput.AlignVCenter
                    text: selectedCompartment ? selectedCompartment.name : null
                    color: ThemeManager.palette.text
                    enabled: !simulation.isRunning

                    // Alpha numeric non empty with spaces
                    validator: RegularExpressionValidator {
                        regularExpression: /^[a-zA-Z0-9 ]+$/
                    }
                    onEditingFinished: {
                        selectedCompartment.name = text
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Symbol"
                color: ThemeManager.palette.text
                Layout.preferredWidth: 80
            }
            /// Edit box for symbol
            Rectangle {
                Layout.fillWidth: true
                height: 30
                border.width: 1
                border.color: ThemeManager.palette.mid
                // color: "transparent"
                color: simulation.isRunning ? Qt.rgba(ThemeManager.palette.mid.r, ThemeManager.palette.mid.g, ThemeManager.palette.mid.b, 0.3) : "transparent"
                TextInput {
                    anchors.fill: parent
                    anchors.margins: 5
                    verticalAlignment: TextInput.AlignVCenter
                    text: selectedCompartment ? selectedCompartment.symbol : null
                    color: ThemeManager.palette.text
                    enabled: !simulation.isRunning

                    // Alpha numeric non empty
                    validator: RegularExpressionValidator {
                        regularExpression: /^[a-zA-Z0-9]+$/
                    }

                    onEditingFinished: {
                        selectedCompartment.symbol = text
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Initial Amount"
                color: ThemeManager.palette.text
                Layout.preferredWidth: 80
            }
            /// Edit box for Initial Amount
            Rectangle {
                Layout.fillWidth: true
                height: 30
                border.width: 1
                border.color: ThemeManager.palette.mid
                // color: "transparent"
                color: simulation.isRunning ? Qt.rgba(ThemeManager.palette.mid.r, ThemeManager.palette.mid.g, ThemeManager.palette.mid.b, 0.3) : "transparent"
                TextInput {
                    anchors.fill: parent
                    anchors.margins: 5
                    verticalAlignment: TextInput.AlignVCenter
                    text: selectedCompartment ? selectedCompartment.initialAmount : null
                    color: ThemeManager.palette.text
                    enabled: !simulation.isRunning

                    validator: DoubleValidator {
                    }

                    onEditingFinished: {
                        selectedCompartment.initialAmount = text
                    }
                }
            }
        }

        /// Delete button for compartment, calls remove_compartment c++ function
        Button {
            Layout.fillWidth: true
            enabled: !simulation.isRunning

            icon.name: "Delete compartment"
            icon.source: "qrc:/resources/icons/delete.svg"
            icon.color: enabled ? ThemeManager.palette.danger : ThemeManager.palette.mid
            icon.height: 20
            icon.width: 20

            text: icon.name

            ToolTip.visible: hovered
            ToolTip.delay: 500
            ToolTip.text: icon.name

            onClicked: {
                simulation.remove_compartment(selectedCompartment.symbol)
            }
        }
    }
}
