/**
  @file TimePlotUI
  @author Dominik Leisinger
**/
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import QtQuick

Item {
    id: root
    width: 400
    height: 300

    // Properties to control the plot
    property var timeData: []
    property var aData: []
    property var bData: []
    property var cData: []
    property string title: "Time Series Plot"
    property string xAxisTitle: "Time"
    property string yAxisTitle: "Value"
    property real minY: 0
    property real maxY: 100
    property real minX: 0
    property real maxX: 100

    // Series names - add these properties
    property string seriesAName: "A"
    property string seriesBName: "B"
    property string seriesCName: "C"

    // Colors
    property color seriesAColor: "steelblue"
    property color seriesBColor: "firebrick"
    property color seriesCColor: "forestgreen"

    // Method to add new data points
    function addDataPoint(time, a, b, c) {
        if (isNaN(time) || isNaN(a) || isNaN(b) || isNaN(c)) {
            console.error("Invalid data point values:", time, a, b, c);
            return;
        }
        // Create new arrays instead of modifying existing ones
        // This ensures the UI updates properly
        var newTimeData = [...timeData];
        var newAData = [...aData];
        var newBData = [...bData];
        var newCData = [...cData];

        newTimeData.push(time);
        newAData.push(a);
        newBData.push(b);
        newCData.push(c);

        // Limit data points to prevent performance issues
        const maxPoints = 1000;
        if (newTimeData.length > maxPoints) {
            newTimeData.shift();
            newAData.shift();
            newBData.shift();
            newCData.shift();
        }

        // Update the properties
        timeData = newTimeData;
        aData = newAData;
        bData = newBData;
        cData = newCData;

        // Update series data
        updateSeries();
    }

    function clearData() {
        timeData = [];
        aData = [];
        bData = [];
        cData = [];
        updateSeries();
    }

    // Update the series with current data
    function updateSeries() {
        // Clear series first
        seriesA.clear();
        seriesB.clear();
        seriesC.clear();

        // Safeguard: Return early if no data
        if (timeData.length === 0) return;

        // Then add all data points
        for (let i = 0; i < timeData.length; i++) {
            if (i < timeData.length && i < aData.length)
                seriesA.append(timeData[i], aData[i]);
            if (i < timeData.length && i < bData.length)
                seriesB.append(timeData[i], bData[i]);
            if (i < timeData.length && i < cData.length)
                seriesC.append(timeData[i], cData[i]);
        }
    }

    // Auto scale axes based on data
    function autoScaleAxes() {
        if (timeData.length === 0) return;

        // Find min/max values
        let minTime = Math.min(...timeData);
        let maxTime = Math.max(...timeData);

        let allValues = [...aData, ...bData, ...cData];
        let minValue = Math.min(...allValues);
        let maxValue = Math.max(...allValues);

        // Add some padding
        let xPadding = (maxTime - minTime) * 0.05;
        let yPadding = (maxValue - minValue) * 0.1;

        // Update axis limits
        minX = minTime - xPadding;
        maxX = maxTime + xPadding;
        minY = Math.max(0, minValue - yPadding); // Ensure we don't go below 0 for typical compartment models
        maxY = maxValue + yPadding;
    }

    ChartView {
        id: chartView
        anchors.fill: parent
        antialiasing: true
        title: root.title
        legend.visible: true
        legend.alignment: Qt.AlignBottom


        ValueAxis {
            id: axisX
            titleText: root.xAxisTitle
            min: root.minX
            max: root.maxX > root.minX ? root.maxX : root.minX + 1 // Prevent invalid axis range
        }

        ValueAxis {
            id: axisY
            titleText: root.yAxisTitle
            min: root.minY
            max: root.maxY > root.minY ? root.maxY : root.minY + 1 // Prevent invalid axis range
        }

        LineSeries {
            id: seriesA
            name: root.seriesAName
            axisX: axisX
            axisY: axisY
            color: root.seriesAColor
        }

        LineSeries {
            id: seriesB
            name: root.seriesBName
            axisX: axisX
            axisY: axisY
            color: root.seriesBColor
        }

        LineSeries {
            id: seriesC
            name: root.seriesCName
            axisX: axisX
            axisY: axisY
            color: root.seriesCColor
        }
    }

}
