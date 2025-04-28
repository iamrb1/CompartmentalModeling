import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

import Application

Item {
    id: errorModule
    property Item container
    property Simulation parentSimulation
    property string errorMessage: ""
    property real messageType: 0

    // Function to get color based on message type
    function getTypeColor(type) {
        switch (type) {
        case Simulation.INFO:
            return ThemeManager.palette.info || "#3498db"
        case Simulation.WARNING:
            return ThemeManager.palette.warning || "#f39c12"
        case Simulation.ERR:
            return ThemeManager.palette.danger || "#e74c3c"
        default:
            return ThemeManager.palette.info || "#3498db"
        }
    }

    // Function to get text
    function getTypeText(type) {
        switch (type) {
        case Simulation.INFO:
            return "Info"
        case Simulation.WARNING:
            return "Warning"
        case Simulation.ERR:
            return "Error"
        default:
            return "Info"
        }
    }

    // Function to get fallback emoji for icons
    function getTypeEmoji(type) {
        switch (type) {
        case Simulation.INFO:
            return "ℹ️"
        case Simulation.WARNING:
            return "⚠️"
        case Simulation.ERR:
            return "❌"
        default:
            return "ℹ️"
        }
    }

    Connections {
        target: parentSimulation
        function onPromptMessage(message, type, mode) {
            errorModule.errorMessage = message
            errorModule.messageType = type

            if (mode === Simulation.TOAST) {
                toastPopup.open()
            } else if (mode === Simulation.DIALOG) {
                messageDialog.open()
            }
        }
    }

    // Message Dialog
    Dialog {
        id: messageDialog
        title: ""
        modal: true
        closePolicy: Popup.CloseOnEscape
        parent: container
        anchors.centerIn: parent
        width: 350
        height: contentLayout.implicitHeight + 100
        padding: 0

        background: Rectangle {
            color: ThemeManager.palette.base || "white"
            border.color: ThemeManager.palette.mid || "#cccccc"
            border.width: 1
            radius: 4
        }

        header: Rectangle {
            color: getTypeColor(errorModule.messageType)
            height: 36

            Text {
                text: getTypeText(errorModule.messageType)
                color: ThemeManager.palette.text
                font.pixelSize: 14
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 16
            }
        }

        contentItem: ColumnLayout {
            id: contentLayout
            spacing: 20
            anchors.margins: 16

            Item { height: 8 } // Top spacing

            Text {
                text: errorModule.errorMessage
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: ThemeManager.palette.text || "black"
                font.pixelSize: 14
            }

            Item { height: 8 } // Bottom spacing
        }

        footer: Rectangle {
            color: "transparent"
            implicitHeight: 50

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12
                text: "OK"
                width: 80
                height: 30

                background: Rectangle {
                    color: parent.down ? Qt.darker(getTypeColor(errorModule.messageType), 1.1) : getTypeColor(errorModule.messageType)
                    radius: 3
                }

                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 13
                    font.bold: true
                    color: ThemeManager.palette.text || "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: messageDialog.close()
            }
        }
    }

    // Toast Popup
    Popup {
        id: toastPopup
        parent: container
        anchors.centerIn: parent
        y: parent ? parent.height - height - 20 : 0
        width: Math.min(parent ? parent.width * 0.6 : 300, 350)
        height: toastContent.implicitHeight + 16
        padding: 0

        // Fade in animation
        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 150 }
        }

        // Fade out animation
        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 150 }
        }

        background: Rectangle {
            color: ThemeManager.palette.base || "#ffffff"
            border.color: ThemeManager.palette.mid || "#cccccc"
            border.width: 1
            radius: 3
        }

        contentItem: RowLayout {
            id: toastContent
            spacing: 10
            anchors.margins: 8

            Rectangle {
                Layout.preferredWidth: 3
                Layout.fillHeight: true
                color: getTypeColor(errorModule.messageType)
                radius: 1
            }

            Text {
                text: getTypeEmoji(errorModule.messageType)
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                text: errorModule.errorMessage
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                color: ThemeManager.palette.text || "black"
                font.pixelSize: 13
                verticalAlignment: Text.AlignVCenter
                Layout.leftMargin: 4
                Layout.minimumHeight: 24
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        // Timer to auto-close the toast
        Timer {
            id: autoCloseTimer
            interval: 3000 // 3 seconds
            running: false
            repeat: false
            onTriggered: toastPopup.close()
        }

        onVisibleChanged: {
            if (visible) {
                autoCloseTimer.restart()
            }
        }
    }
}
