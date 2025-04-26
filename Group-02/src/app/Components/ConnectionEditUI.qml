/**
 @file ConnectionEditUI
 @author Nitish Maindoliya, Rahul Baragur
 **/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Application
import Utilities
import Components

/**
 * Sidebar interface will change to this UI when a connection is selected
 */
Rectangle {
    property Connection selectedConnection: simulation.sidebarConnection

    onSelectedConnectionChanged: {
        setConnectionComboboxes();
    }

    Connections {
        target: simulation

        /**
         * Updates the UI combo boxes when the selected connection is changed
         */
        function onCompartmentsChanged() {
            setConnectionComboboxes();
        }
    }

    /**
     * Updates source and target combo box for the selected connection for display on UI
     */
    function setConnectionComboboxes() {
        if (!selectedConnection) {
            return
        }

        sourceComboBox.isInternal = true;
        for (let i = 0; i < simulation.compartments.length; i++) {
            if (simulation.compartments[i].symbol === selectedConnection.source.symbol) {
                sourceComboBox.currentIndex = i
                break
            }
        }
        sourceComboBox.isInternal = false;

        targetComboBox.isInternal = true;
        for (let j = 0; j < simulation.compartments.length; j++) {
            if (simulation.compartments[j].symbol === selectedConnection.target.symbol) {
                targetComboBox.currentIndex = j
                break
            }
        }
        targetComboBox.isInternal = false;
    }

    id: connectionEditor
    anchors.fill: parent
    color: ThemeManager.palette.base

    /// Layout for the Connection Edit Interface with combo boxes
    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 10

        Item {
            Layout.preferredHeight: 10
        }

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
                Layout.preferredWidth: 40
            }
            ComboBox {
                id: sourceComboBox
                model: simulation.compartments.map(
                        (compartment, index) => compartment.name + " ("
                        + compartment.symbol + ")")

                Layout.fillWidth: true
                Layout.fillHeight: true
                enabled: !simulation.isRunning

                property bool isInternal: false

                onActivated: {
                    if (currentIndex >= 0 && selectedConnection) {
                        selectedConnection.source = simulation.compartments[currentIndex]
                    }
                }

                onCurrentIndexChanged: {
                    if (isInternal && currentIndex >= 0 && selectedConnection) {
                        selectedConnection.source = simulation.compartments[currentIndex]
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "End"
                color: ThemeManager.palette.text
                Layout.preferredWidth: 40
            }
            ComboBox {
                id: targetComboBox
                model: simulation.compartments.map(
                        (compartment, index) => compartment.name + " ("
                        + compartment.symbol + ")")

                Layout.fillWidth: true
                Layout.fillHeight: true
                enabled: !simulation.isRunning

                property bool isInternal: false

                onActivated: {
                    if (currentIndex >= 0 && selectedConnection) {
                        selectedConnection.target = simulation.compartments[currentIndex]
                    }
                }

                onCurrentIndexChanged: {
                    if (isInternal && currentIndex >= 0 && selectedConnection) {
                        selectedConnection.target = simulation.compartments[currentIndex]
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Rate of Transfer"
                color: ThemeManager.palette.text
                Layout.preferredWidth: 100
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
                    text: selectedConnection ? selectedConnection.rateExpression : null
                    color: ThemeManager.palette.text
                    enabled: !simulation.isRunning

                    // validator: DoubleValidator {
                    //     bottom: 0.0
                    //     top: 1000000.0
                    //     decimals: 6
                    //     locale: "en_US"
                    //     notation: DoubleValidator.StandardNotation
                    // }

                    onEditingFinished: {
                        selectedConnection.rateExpression = text
                    }
                }
            }
        }

        /// Button to delete the connection calls remove_connection on click
        Button {
            Layout.fillWidth: true
            enabled: !simulation.isRunning

            icon.name: "Delete connection"
            icon.source: "qrc:/resources/icons/delete.svg"
            icon.color: enabled ? ThemeManager.palette.danger : ThemeManager.palette.mid
            icon.height: 20
            icon.width: 20

            text: icon.name

            ToolTip.visible: hovered
            ToolTip.delay: 500
            ToolTip.text: icon.name

            onClicked: {
                simulation.remove_connection(selectedConnection)
            }
        }
    }
}
