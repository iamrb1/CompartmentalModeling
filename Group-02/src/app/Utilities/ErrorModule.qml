pragma Singleton
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Components
import cseg2


Item {
    // property Simulation parentSimulation
    // property Simulation simulation: parentSimulation

    id: errorModule

    // Properties for the error windows
    property bool isModalOpen: false
    property string errorMessage: ""

    Dialog {
        id: modalDialog
        title: "Error"
        modal: true
        closePolicy: Popup.CloseOnEscape
        anchors.centerIn: parent
        width: 400
        height: 200
        padding: 20

        background: Rectangle {
            color: ThemeManager.palette.base
            border.color: ThemeManager.palette.base
            border.width: 1
            radius: 8
        }

        header: Rectangle {
            color: ThemeManager.palette.danger
            height: 50

            Label {
                text: errorModule.errorMessage
                color: ThemeManager.palette.base
                font.pixelSize: 18
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
            }
        }

        contentItem: ColumnLayout {
            spacing: 20

            Image {
                source: "qrc:/resources/icons/warning.svg"
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 48
                Layout.preferredHeight: 48
                fillMode: Image.PreserveAspectFit
                sourceSize.width: 48
                sourceSize.height: 48
                visible: status === Image.Ready

                Text {
                    visible: parent.status !== Image.Ready
                    text: "⚠️"
                    font.pixelSize: 36
                    anchors.centerIn: parent
                }
            }

            Label {
                text: errorModule.errorMessage
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 14
            }
        }

        footer: Item {
            implicitHeight: 60
            RowLayout {
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter

                Button {
                    text: "OK"
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40

                    background: Rectangle {
                        color: parent.down ? ThemeManager.palette.mid : (parent.hovered ? ThemeManager.palette.mid : ThemeManager.palette.shadow)
                        border.color: ThemeManager.palette.ButtonText
                        border.width: 1
                        radius: 4
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 14
                        color: ThemeManager.palette.mid
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: modalDialog.close()
                }
            }
        }
    }

    // Popup error with OK button
    Popup {
        id: popupDialog
        x: 100
        y: 0
        width: 500
        height: 30

        contentItem: Rectangle {
            anchors.fill: parent
            color: ThemeManager.palette.danger

            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: errorModule.errorMessage
                font.bold: true
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        // Timer to auto-close the popup
        Timer {
            id: autoCloseTimer
            interval: 2000 // 2 seconds
            running: false
            repeat: false
            onTriggered: popupDialog.close()
        }

        onVisibleChanged: {
            if (visible) {
                autoCloseTimer.restart()
            }
        }
    }

    //function to show an error, either as modal or popup
    function showError(message, isModal) {
        errorMessage = message

        if (isModal) {
            // Show modal error dialog
            modalDialog.open()
            isModalOpen = true
        } else {
            // Show non-modal popup
            popupDialog.open()
        }
    }

    //modal error dialog blocks interaction until user presses OK
}