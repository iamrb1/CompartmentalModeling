import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import cseg2
import Utilities
import Components

// Connection section
Rectangle {
    property Simulation parentSimulation
    property Connection selectedConnection: parentSimulation.sidebarConnection

    id: sidebarTop
    anchors.fill: parent
    color: ThemeManager.palette.base

    // function comparmentNames() {
    //     let names =

    // }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
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
                    text: selectedConnection ? selectedConnection.source.symbol : null
                    color: ThemeManager.palette.text
                }
            }
            // ComboBox {
            //     model: parentSimulation.compartments.map((compartment) => compartment.name + " (" + compartment.symbol + ")")
            //     delegate: ItemDelegate {
            //         text: index
            //         highlighted: ListView.isCurrentItem
            //     }
            // }
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
                    text: selectedConnection ? selectedConnection.target.symbol : null
                    color: ThemeManager.palette.text
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Rate of transfer"
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
                    text: selectedConnection ? selectedConnection.rateExpression : null
                    color: ThemeManager.palette.text

                    onEditingFinished: {
                        selectedConnection.rateExpression = text
                    }
                }
            }
        }

        Button {
            Layout.fillWidth: true

            icon.name: "Delete connection"
            icon.source: "qrc:/resources/icons/delete.svg"
            icon.color: ThemeManager.palette.danger
            icon.height: 20
            icon.width: 20

            text: icon.name

            ToolTip.visible: hovered
            ToolTip.delay: 500
            ToolTip.text: icon.name

            onClicked: {
                parentSimulation.remove_connection(selectedConnection)
            }
        }
    }
}
