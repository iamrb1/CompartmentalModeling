/**
 @file GraphWindowUI.qml
 @author Rahul Baragur, Dominik Leisinger
 **/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Application
import Utilities
import Components


/**
 * Layout for the window that holds the Graph
 * Stores a TimePlotUI (the graph) within
 */
ApplicationWindow {
    id: graphWindow
    title: `${simulation.name} Graph`
    width: 800
    height: 600
    visible: true
    palette: ThemeManager.palette

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        TimePlotUI {
            id: timePlot
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
