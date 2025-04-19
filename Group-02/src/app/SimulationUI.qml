import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Dialogs
import QtQuick.Shapes

import cseg2
import Utilities
import Components

ApplicationWindow {
    Simulation {
        id: simulation
    }

    id: mainFrameUI
    width: 800
    height: 600
    title: simulation.name + " - Compartmental Modelling System"
    visible: true

    palette: ThemeManager.palette

    menuBar: Rectangle {
        height: 40
        width: parent.width
        color: ThemeManager.palette.base

        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 10

            MenuBar {
                id: menuBar
                Layout.preferredWidth: implicitWidth
                background: Rectangle { color: "transparent" }

                Menu {
                    title: "File"
                    Action {
                        text: "New Simulation"
                        onTriggered: console.log("New Simulation")
                    }

                    Action {
                        text: "Open"
                        onTriggered: openFileDialog.open()
                    }

                    Action {
                        text: "Save"
                        onTriggered: saveFileDialog.open()
                    }

                    Action {
                        text: "Save As"
                        onTriggered: saveFileDialog.open()
                    }

                    MenuSeparator {}
                    Menu {
                        title: "Theme"
                        Repeater {
                            model: ThemeManager.themeList
                            MenuItem {
                                text: modelData.name
                                checkable: true
                                checked: modelData.value === ThemeManager.theme
                                onTriggered: ThemeManager.setTheme(modelData.value)
                            }
                        }
                    }
                    MenuSeparator {}
                    Action {
                        text: "Exit"
                        onTriggered: Qt.quit()
                    }
                }

                Menu {
                    title: "Help"
                    Action {
                        text: "About"
                        onTriggered: console.log("About clicked")
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                width: 50
                height: 25
                text: "Play"
                font.pixelSize: 12
                background: Rectangle {
                    color: ThemeManager.palette.button
                    border.color: ThemeManager.palette.shadow
                    radius: 3
                }
                onClicked: {
                    console.log("Play clicked")
                    //simulation play functionality
                    // simulation.play()
                }
            }

            Button {
                width: 50
                height: 25
                text: "Pause"
                font.pixelSize: 12
                background: Rectangle {
                    color: ThemeManager.palette.button
                    border.color: ThemeManager.palette.shadow
                    radius: 3
                }
                onClicked: {
                    console.log("Pause clicked")
                    //simulation pause functionality here
                    // simulation.pause()
                }
            }

            Item {
                width: 95
                Layout.preferredWidth: width
            }
        }
    }

    // === Layout ===
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // --- Toolbar ---
        ToolBar {
            id: toolBar
            Layout.fillHeight: true
            width: 40
            z: 10

            background: Rectangle {
                color: ThemeManager.palette.base
            }

            Column {
                anchors.fill: parent
                spacing: 10

                ToolButton {
                    icon.name: "Add compartment"
                    icon.source: "qrc:/resources/icons/plus.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name

                    onClicked: {
                        simulation.add_compartment()
                    }
                }
                ToolButton {
                    icon.name: "Add connection"
                    icon.source: "qrc:/resources/icons/arrow-right.svg"
                    icon.color: ThemeManager.palette.text
                    icon.height: 20
                    icon.width: 20

                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: icon.name
                }
            }
        }

        SplitView {
            id: mainContentSplitView
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Horizontal

            handle: Rectangle {
                implicitWidth: 4
                implicitHeight: 4
                color: ThemeManager.palette.shadow

                Rectangle {
                    width: 4
                    height: 30
                    radius: 2
                    anchors.centerIn: parent
                    color: ThemeManager.palette.mid
                    visible: SplitHandle.hovered || SplitHandle.pressed
                }
            }

            // --- Plot Container ---
            Rectangle {
                id: plotContainer
                SplitView.preferredWidth: 400
                SplitView.minimumWidth: 0
                SplitView.fillHeight: true
                color: ThemeManager.palette.base
                border.color: ThemeManager.palette.shadow

                TimePlotUI {
                    id: testPlot
                    width: parent.width - 20
                    height: parent.height - 20
                    x: (parent.width - width) / 2
                    y: (parent.height - height) / 2
                    title: "Compartmental Model Test Data"

                    Component.onCompleted: {
                        testPlot.xAxisTitle = "Time (hours)"
                        testPlot.yAxisTitle = "Population"
                        testPlot.seriesAName = "Susceptible"
                        testPlot.seriesBName = "Infected"
                        testPlot.seriesCName = "Recovered"

                        // Hard-coded test data
                        var testData = [
                            {time: 0, a: 1000, b: 10, c: 0},
                            {time: 1, a: 950, b: 50, c: 10},
                            {time: 2, a: 890, b: 90, c: 30},
                            {time: 3, a: 820, b: 120, c: 70},
                            {time: 4, a: 740, b: 140, c: 130},
                            {time: 5, a: 660, b: 150, c: 200},
                            {time: 6, a: 580, b: 150, c: 280},
                            {time: 7, a: 500, b: 140, c: 370},
                            {time: 8, a: 430, b: 120, c: 460},
                            {time: 9, a: 370, b: 100, c: 540},
                            {time: 10, a: 320, b: 80, c: 610},
                            {time: 11, a: 280, b: 60, c: 670},
                            {time: 12, a: 250, b: 40, c: 720},
                            {time: 13, a: 220, b: 30, c: 760},
                            {time: 14, a: 200, b: 20, c: 790},
                            {time: 15, a: 190, b: 10, c: 810}
                        ];

                        for (var i = 0; i < testData.length; i++) {
                            testPlot.addDataPoint(testData[i].time, testData[i].a, testData[i].b, testData[i].c);
                        }

                        testPlot.autoScaleAxes();
                    }
                }
            }

            // --- Simulation UI ---
            Rectangle {
                id: simulationUI
                SplitView.fillWidth: true
                SplitView.minimumWidth: 200
                SplitView.fillHeight: true
                color: ThemeManager.palette.window
                border.color: ThemeManager.palette.shadow

                Item {
                    id: clipContainer
                    anchors.fill: parent
                    clip: true

                    Item {
                        id: compartmentContainer
                        width: parent.width
                        height: parent.height
                        scale: 1.0
                        transformOrigin: Item.Center
                        anchors.centerIn: parent

                        Repeater {
                            model: simulation.compartments

                            delegate: CompartmentUI {
                                id: compartmentUI
                                x: modelData.x || 100 + (index % 2) * 200
                                y: modelData.y || 100 + Math.floor(index / 2) * 100
                                color: ThemeManager.palette.base
                                border.color: ThemeManager.palette.shadow
                                compartment: modelData
                            }
                        }
                    }
                }

                // Add scaling buttons in top right corner
                Row {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 5
                    spacing: 5
                    z: 10
                    visible: simulationUI.width >= (30 * 2 + 5 + 10) // Buttons disappear when screen is too small

                    Button {
                        text: "+"
                        width: 30
                        height: 30
                        onClicked: {
                            compartmentContainer.scale = Math.min(compartmentContainer.scale + 0.1, 2.0)
                        }
                    }

                    Button {
                        text: "-"
                        width: 30
                        height: 30
                        onClicked: {
                            compartmentContainer.scale = Math.max(compartmentContainer.scale - 0.1, 0.5)
                        }
                    }
                }
            }
        }

        // --- Sidebar ---
        SidebarUI {
            simulation: simulation
        }
    }

    FileDialog {
        id: saveFileDialog
        objectName: "saveFileDialog"
        title: "Save Simulation"
        nameFilters: ["Simulation files (*.sim)"]
        fileMode: FileDialog.SaveFile
        onAccepted: {
            simulation.save_xml(selectedFile)
        }
    }

    FileDialog {
        id: openFileDialog
        objectName: "openFileDialog"
        title: "Open Simulation"
        nameFilters: ["Simulation files (*.sim)"]
        fileMode: FileDialog.OpenFile
        onAccepted: {
            simulation.load_xml(selectedFile)
        }
    }
}