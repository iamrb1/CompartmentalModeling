/**
 @file TimePlotUI
 @author Nitish Maindoliya, Rahul Baragur, Dominik Leisinger
 **/
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import QtQuick

import Application
import Utilities
import Components


/**
 * Layout for the Graph UI component which is stored within the GraphWindowUI
 * Graphs the series data dynamically with each time step
 */
Item {
    Layout.fillHeight: true
    Layout.fillWidth: true

    /// Reference to all series
    property var seriesData: ({})

    property int minY: 100000000
    property int maxY: -100000000

    Component.onCompleted: {
        loadSeries()
    }


    /**
     * Load the series from the simulation
     */
    function loadSeries() {
        // Clear previous series data
        chart.removeAllSeries()
        seriesData = {}
        simulation.compartments.forEach(compartment => {
            addSeriesToChart(
                compartment.symbol,
                compartment.name)
        })
        legendRepeater.model = Object.keys(seriesData)
        console.log("Series Properties: ", legendRepeater.model)
    }


    /**
     * Take series data and add it to chart created
     * @param id
     * @param name
     */
    function addSeriesToChart(id, name) {
        let series = chart.createSeries(ChartView.SeriesTypeLine, name,
            timeAxis, valueAxis)

        seriesData[id] = {
            "series": series,
            "name": name,
            "color": series.color
        }
    }

    Connections {
        target: simulation


        /**
         * Iterate through series and append values to seriesData
         * @param time
         * @param series
         */
        function onAddGraphingValues(time, series) {
            /// Update each series if visible
            for (let key in series) {
                if (key in seriesData) {
                    let data = series[key]
                    seriesData[key].series.append(time, data)

                    if (data > maxY) {
                        maxY = data + 10
                    } else if (data < minY) {
                        minY = data - 10
                    }
                }
            }

            // Update axes
            timeAxis.max = time
            // timeAxis.min = time - 60;
        }

        function onCompartmentsChanged() {
            loadSeries()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        /// the chart itself with Time as the x axis and the values as the y axis
        ChartView {
            id: chart
            Layout.fillHeight: true
            Layout.fillWidth: true
            antialiasing: true

            title: "Data"

            ValueAxis {
                id: timeAxis
                titleText: "Time (s)"
                min: 0
            }

            ValueAxis {
                id: valueAxis
                titleText: "Values"
                min: minY ? minY : 0
                max: maxY ? maxY : 100
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            Layout.minimumHeight: 70
            color: ThemeManager.palette.base
            border.width: 1
            border.color: ThemeManager.palette.mid

            ColumnLayout {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5

                Text {
                    Layout.alignment: Qt.AlignLeft
                    text: "Legend"
                    color: "black"
                    font.pixelSize: 10
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 15
                    clip: true

                    Repeater {
                        id: legendRepeater
                        Layout.fillWidth: true
                        model: Object.keys(seriesData)

                        Row {
                            spacing: 5
                            height: parent ? parent.height - 5 : 30

                            property string seriesId: modelData

                            CheckBox {
                                id: visibilityToggle
                                anchors.verticalCenter: parent.verticalCenter
                                checked: seriesData[seriesId].series.visible
                                onToggled: {
                                    seriesData[seriesId].series.visible = checked
                                }
                            }

                            Rectangle {
                                width: 15
                                height: 15
                                color: seriesData[seriesId].color
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Text {
                                text: seriesData[seriesId].name
                                anchors.verticalCenter: parent.verticalCenter
                                color: seriesData[seriesId].visibility ? "black" : "gray"
                            }
                        }
                    }
                }
            }
        }
    }
}
