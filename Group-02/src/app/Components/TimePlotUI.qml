/**
 @file TimePlotUI
 @author Rahul Baragur, Dominik Leisinger
 **/
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import QtQuick
import Utilities

Item {
    Layout.fillHeight: true
    Layout.fillWidth: true


    // Reference to all series
    property var seriesData: ({})

    // Properties
    property var seriesProperties: ({})

    property int minY: 100000000;
    property int maxY: -100000000;


    Component.onCompleted: {
        loadSeries();
    }

    function loadSeries() {
        seriesData = {}
        seriesProperties = {}
        simulation.compartments.forEach((compartment) => {
            addSeriesToChart(compartment.symbol, compartment.name)
        });
    }

    function addSeriesToChart(id, name) {
        let series = chart.createSeries(ChartView.SeriesTypeLine, name, timeAxis, valueAxis);

        seriesData[id] = series

        seriesProperties[id] = {
            name: name,
            color: series.color,
            visibility: true,
        }
    }

    Connections {
        target: simulation

        function onAddGraphingValues(time, series) {
            // Update each series if visible
            for (let key in series) {
                if (key in seriesData) {
                    let data = series[key]
                    seriesData[key].append(time, data)

                    if (data > maxY) {
                        maxY = data + 10;
                    } else if (data < minY) {
                        minY = data - 10;
                    }
                }
            }

            // Update axes
            timeAxis.max = time;
            // timeAxis.min = time - 60;
        }

        // function onCompartmentsUpdated() {
        //     loadSeries();
        // }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ChartView {
            id: chart
            Layout.fillHeight: true
            Layout.fillWidth: true
            antialiasing: true
            legend.visible: true
            legend.alignment: Qt.AlignBottom

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

        // Rectangle {
        //     Layout.fillWidth: true
        //     Layout.preferredHeight: 80
        //     Layout.minimumHeight: 70
        //     color: ThemeManager.palette.base
        //     border.width: 1
        //     border.color: ThemeManager.palette.mid

        //     Text {
        //         anchors.top: parent.top
        //         anchors.left: parent.left
        //         anchors.margins: 5
        //         text: "Legend"
        //         color: "black"
        //         font.pixelSize: 10
        //     }

        //     RowLayout {
        //         spacing: 15
        //         clip: true

        //         Repeater {
        //             Layout.fillWidth: true
        //             model: Object.keys(seriesProperties)

        //             Component.onCompleted: {
        //                 console.log("Model: ", model)
        //                 console.log("Augh: ", Object.keys(seriesProperties))
        //             }

        //             Row {
        //                 spacing: 5
        //                 height : parent ? parent.height - 5 : 30

        //                 property string seriesId: modelData
        //                 property let seriesData: seriesProperties[seriesId]

        //                 Component.onCompleted: {
        //                     console.log(seriesId, seriesData)
        //                 }

        //                 CheckBox {
        //                     id: visibilityToggle
        //                     anchors.verticalCenter: parent.verticalCenter
        //                     checked: seriesData.visibility
        //                     onToggled: {
        //                         seriesProperties[seriesId].visibility = checked
        //                     }
        //                 }

        //                 Rectangle {
        //                     width: 15
        //                     height: 15
        //                     color: seriesData.color
        //                     anchors.verticalCenter: parent.verticalCenter
        //                 }

        //                 Text {
        //                     text: seriesData.name
        //                     anchors.verticalCenter: parent.verticalCenter
        //                     color: seriesVisible ? "black" : "gray"
        //                 }
        //             }
        //         }
        //     }
        // }

    }
}
