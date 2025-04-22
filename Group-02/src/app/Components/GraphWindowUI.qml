/**
 @file GraphWindowUI.qml
 @author Rahul Baragur, Dominik Leisinger
 **/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import cseg2
import Components
import Utilities

ApplicationWindow {
    id: graphWindow
    title: "Simulation Graph"
    width: 800
    height: 600
    visible: true
    palette: ThemeManager.palette

    property Simulation simulation: null
    property Timer simulationTimer: Timer {
        id: internalTimer
        interval: 50  /// 20 FPS
        repeat: true
        running: false

        onTriggered: {
            if (simulation) {
                simulation.stepSimulation(0.1);
            }
        }
    }

    /// Simulation handler for updating the series (timeplot)
    onSimulationChanged: {
        if (simulation) {
            timePlot.updateSeriesFromSimulation();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        TimePlotUI {
            id: timePlot
            Layout.fillWidth: true
            Layout.fillHeight: true

            function updateSeriesFromSimulation() {
                timePlot.clearData();
                timePlot.dataSeries = [];

                /// Add a series for each compartment in the series
                if (simulation && simulation.compartments) {
                    for (let i = 0; i < simulation.compartments.length; i++) {
                        let compartment = simulation.compartments[i];
                        timePlot.addDataSeries(
                            compartment.symbol,
                            compartment.name + " (" + compartment.symbol + ")",
                            null  /// this for random colors, there is a map that has a bunch of colors check below
                        );
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                text: internalTimer.running ? "Pause" : "Start"
                Component.onCompleted: {
                           palette.buttonText = "black"
                       }
                onClicked: {
                    if (!internalTimer.running) {
                        if (simulation) {
                            simulation.startSimulation();
                        }
                        internalTimer.start();
                    } else {
                        internalTimer.stop();
                    }
                }
            }

            Button {
                text: "Reset"
                Component.onCompleted: {
                           palette.buttonText = "black"
                       }
                onClicked: {
                    internalTimer.stop();
                    timePlot.clearData();
                    if (simulation) {
                        simulation.startSimulation();
                        timePlot.updateSeriesFromSimulation();
                    }
                }
            }

            Button {
                text: "Step"
                Component.onCompleted: {
                           palette.buttonText = "black"
                       }
                enabled: !internalTimer.running
                onClicked: {
                    if (simulation) {
                        simulation.stepSimulation(0.1);
                    }
                }
            }
        }
    }

    /// connect simulation data updates to plot
    Connections {
        target: simulation
        enabled: simulation !== null

        function onSimulationDataUpdated(time, values) {
            let dataArray = [];

            /// claude generated data update
            for (let series of timePlot.dataSeries) {
                if (values.hasOwnProperty(series.id)) {
                    dataArray.push(values[series.id]);
                } else {
                    dataArray.push(NaN);
                }
            }

            timePlot.addDataPoint(time, dataArray);
        }

        function onCompartmentsChanged() {
            timePlot.updateSeriesFromSimulation();
        }
    }

    onClosing: {
        internalTimer.stop();
    }
}
