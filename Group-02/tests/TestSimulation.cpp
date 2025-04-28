/**
 * @file TestSimulation.cpp
 * @author Nitish Maindoliya, Bram Hogg
 */


#include "TestSimulation.h"
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QUrl>


void TestSimulation::test_constructors() {
    // default constructor, name
    auto simulation = std::make_unique<Simulation>();
    QVERIFY(simulation != nullptr);
    QCOMPARE(simulation->property("name").toString(), QString("New Simulation"));

    //no connections, compartments, variables in simulation by default
    QCOMPARE(simulation->get_compartments().size(), 0);
    QCOMPARE(simulation->get_connections().size(), 0);
    QCOMPARE(simulation->get_variables().size(), 0);

    //connections not existent on creation
    QCOMPARE(simulation->property("connectionMode").toBool(), false);
    QCOMPARE(simulation->property("sourceCompartment").value<Compartment*>(), nullptr);
    QCOMPARE(simulation->property("targetCompartment").value<Compartment*>(), nullptr);

    //timer defaults to not running
    QCOMPARE(simulation->property("isRunning").toBool(), false);
    QCOMPARE(simulation->property("currentTime").toInt(), 0);
}

void TestSimulation::test_getter_setters() {

    auto simulation = std::make_unique<Simulation>();
    QSignalSpy compartmentsChangedSpy(simulation.get(), SIGNAL(compartmentsChanged()));
    QSignalSpy connectionsChangedSpy(simulation.get(), SIGNAL(connectionsChanged()));
    QSignalSpy variablesChangedSpy(simulation.get(), SIGNAL(variablesChanged()));
    QSignalSpy connectionModeChangedSpy(simulation.get(), SIGNAL(connectionModeChanged()));
    QSignalSpy sourceCompartmentChangedSpy(simulation.get(), SIGNAL(sourceCompartmentChanged()));
    QSignalSpy targetCompartmentChangedSpy(simulation.get(), SIGNAL(targetCompartmentChanged()));
    QSignalSpy sidebarCompartmentChangedSpy(simulation.get(), SIGNAL(sidebarCompartmentChanged()));
    QSignalSpy sidebarConnectionChangedSpy(simulation.get(), SIGNAL(sidebarConnectionChanged()));

    //test signals are working
    simulation->add_compartment();
    QCOMPARE(compartmentsChangedSpy.count(), 1);
    QCOMPARE(simulation->get_compartments().size(), 1);

    //compartment/connection getters and setters
    simulation->add_compartment();
    QCOMPARE(compartmentsChangedSpy.count(), 2);
    QCOMPARE(simulation->get_compartments().size(), 2);

    auto compartments = simulation->get_compartments();
    QVERIFY(compartments.at(0)->get_symbol() != compartments.at(1)->get_symbol());

    simulation->set_connection_mode(true);
    QCOMPARE(connectionModeChangedSpy.count(), 1);
    QCOMPARE(simulation->property("connectionMode").toBool(), true);

    //adding more connections
    simulation->set_source_compartment(compartments.at(0));
    QCOMPARE(sourceCompartmentChangedSpy.count(), 1);
    QCOMPARE(simulation->property("sourceCompartment").value<Compartment*>(), compartments.at(0));

    simulation->set_target_compartment(compartments.at(1));
    QCOMPARE(targetCompartmentChangedSpy.count(), 1);
    QCOMPARE(simulation->property("targetCompartment").value<Compartment*>(), compartments.at(1));

    simulation->add_connection();
    QCOMPARE(connectionsChangedSpy.count(), 1);
    QCOMPARE(simulation->get_connections().size(), 1);

    //variable change tests
    simulation->add_variable("testVar", 15.0);
    QCOMPARE(variablesChangedSpy.count(), 1);
    QCOMPARE(simulation->get_variables().size(), 1);
    QCOMPARE(simulation->get_variables().value("testVar").toDouble(), 15.0);

    simulation->update_variable("testVar", "updatedVar", 8.0);
    QCOMPARE(variablesChangedSpy.count(), 2);
    QCOMPARE(simulation->get_variables().size(), 1);
    QCOMPARE(simulation->get_variables().value("updatedVar").toDouble(), 8.0);

    //sidebar compartment test
    simulation->set_sidebar_compartment(compartments.at(0));
    QCOMPARE(sidebarCompartmentChangedSpy.count(), 1);
    QCOMPARE(sidebarConnectionChangedSpy.count(), 1);
    QCOMPARE(simulation->property("sidebarCompartment").value<Compartment*>(), compartments.at(0));
}

void TestSimulation::test_xml_loading_saving() {
    //simulation save and load with a temp file
    auto simulation = std::make_unique<Simulation>();
    simulation->add_compartment();
    simulation->add_compartment();
    auto compartments = simulation->get_compartments();

    //add a lot of compartments, variables, and connections
    compartments[0]->set_initial_amount(100);
    compartments[0]->set_current_amount(100);
    compartments[0]->set_x(50);
    compartments[0]->set_y(60);

    compartments[1]->set_initial_amount(0);
    compartments[1]->set_current_amount(0);
    compartments[1]->set_x(150);
    compartments[1]->set_y(160);

    simulation->set_connection_mode(true);
    simulation->set_source_compartment(compartments[0]);
    simulation->set_target_compartment(compartments[1]);
    simulation->add_connection();

    simulation->add_variable("testRate", 0.05);

    auto connections = simulation->get_connections();
    connections[0]->set_rate_expression("testRate");

    //actual saving tests
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(true);
    QVERIFY(tempFile.open());
    QString filePath = QUrl::fromLocalFile(tempFile.fileName()).toString();
    tempFile.close();

    simulation->save_xml(filePath);

    //load from the temp file just saved
    auto loadedSimulation = std::make_unique<Simulation>();
    loadedSimulation->load_xml(filePath);

    //test all components were loaded correctly
    QCOMPARE(loadedSimulation->get_compartments().size(), 2);
    auto loadedCompartments = loadedSimulation->get_compartments();
    QCOMPARE(loadedCompartments[0]->get_initial_amount(), 100.0);
    QCOMPARE(loadedCompartments[0]->get_x(), 50);
    QCOMPARE(loadedCompartments[0]->get_y(), 60);

    QCOMPARE(loadedSimulation->get_connections().size(), 1);
    auto loadedConnections = loadedSimulation->get_connections();
    QCOMPARE(loadedConnections[0]->get_source()->get_symbol(), loadedCompartments[0]->get_symbol());
    QCOMPARE(loadedConnections[0]->get_target()->get_symbol(), loadedCompartments[1]->get_symbol());
    QCOMPARE(loadedConnections[0]->get_rate_expression(), QString("testRate"));

    QCOMPARE(loadedSimulation->get_variables().size(), 1);
    QCOMPARE(loadedSimulation->get_variables().value("testRate").toDouble(), 0.05);

    //invalid save test
    QSignalSpy promptSpy(loadedSimulation.get(), SIGNAL(promptMessage(Simulation::PromptType, const QString&, Simulation::PromptMode)));
    loadedSimulation->save();

    //remove a compartment that's part of a connection
    loadedSimulation->remove_compartment(loadedCompartments[0]->get_symbol());
    QCOMPARE(loadedSimulation->get_compartments().size(), 1);
    QCOMPARE(loadedSimulation->get_connections().size(), 0);

    QFile::remove(tempFile.fileName());
}

void TestSimulation::test_controls() {
    //test all the components in action

    auto simulation = std::make_unique<Simulation>();
    QSignalSpy isRunningSpy(simulation.get(), SIGNAL(isRunningChanged()));
    QSignalSpy currentTimeSpy(simulation.get(), SIGNAL(currentTimeChanged()));
    QSignalSpy compartmentsSpy(simulation.get(), SIGNAL(compartmentsChanged()));
    QSignalSpy connectionsSpy(simulation.get(), SIGNAL(connectionsChanged()));
    QSignalSpy variablesSpy(simulation.get(), SIGNAL(variablesChanged()));

    simulation->add_compartment();
    simulation->add_compartment();
    auto compartments = simulation->get_compartments();
    compartments[0]->set_initial_amount(100);
    compartments[0]->set_current_amount(100);
    compartments[1]->set_initial_amount(0);
    compartments[1]->set_current_amount(0);

    simulation->set_connection_mode(true);
    simulation->set_source_compartment(compartments[0]);
    simulation->set_target_compartment(compartments[1]);
    simulation->add_connection();

    simulation->add_variable("rate", 0.1);
    auto connections = simulation->get_connections();
    connections[0]->set_rate_expression("rate");

    isRunningSpy.clear();
    currentTimeSpy.clear();
    compartmentsSpy.clear();
    connectionsSpy.clear();
    variablesSpy.clear();

    //check running status by repeatedly starting and pausing, then count singals
    simulation->start();
    QCOMPARE(isRunningSpy.count(), 1);
    QCOMPARE(simulation->property("isRunning").toBool(), true);

    simulation->pause();
    QCOMPARE(isRunningSpy.count(), 2);
    QCOMPARE(simulation->property("isRunning").toBool(), false);

    //reset test
    simulation->take_time_step();
    QVERIFY(compartments[0]->get_current_amount() < 100.0);
    QVERIFY(compartments[1]->get_current_amount() > 0.0);

    //should be reset to initial value
    simulation->reset();
    QCOMPARE(compartments[0]->get_current_amount(), 100.0);
    QCOMPARE(compartments[1]->get_current_amount(), 0.0);
    QCOMPARE(simulation->property("currentTime").toInt(), 0);

    //clear test, should be default values
    simulation->clear();
    QCOMPARE(compartmentsSpy.count(), 1);
    QCOMPARE(connectionsSpy.count(), 1);
    QCOMPARE(variablesSpy.count(), 1);
    QCOMPARE(simulation->get_compartments().size(), 0);
    QCOMPARE(simulation->get_connections().size(), 0);
    QCOMPARE(simulation->get_variables().size(), 0);
    QCOMPARE(simulation->property("currentTime").toInt(), 0);
    QCOMPARE(simulation->property("isRunning").toBool(), false);

    simulation->add_compartment();
    QCOMPARE(compartmentsSpy.count(), 2);
    QCOMPARE(simulation->get_compartments().size(), 1);

    simulation->set_connection_mode(true);
    QCOMPARE(simulation->property("connectionMode").toBool(), true);

    simulation->add_connection();
    QCOMPARE(connectionsSpy.count(), 1);
}

void TestSimulation::test_timer() {
    //timer tests by using signals to check running status and time difference

    auto simulation = std::make_unique<Simulation>();
    QSignalSpy isRunningSpy(simulation.get(), SIGNAL(isRunningChanged()));
    QSignalSpy currentTimeSpy(simulation.get(), SIGNAL(currentTimeChanged()));

    //check number of signals based on start/stop status of the timer
    simulation->start();
    QCOMPARE(isRunningSpy.count(), 1);
    QCOMPARE(simulation->property("isRunning").toBool(), true);

    simulation->pause();
    QCOMPARE(isRunningSpy.count(), 2);
    QCOMPARE(simulation->property("isRunning").toBool(), false);

    simulation->start();
    QCOMPARE(isRunningSpy.count(), 3);
    QCOMPARE(simulation->property("isRunning").toBool(), true);

    simulation->reset();
    QCOMPARE(simulation->property("isRunning").toBool(), false);
    QCOMPARE(simulation->property("currentTime").toInt(), 0);

    simulation->start();
    QCOMPARE(isRunningSpy.count(), 5);
    QCOMPARE(simulation->property("isRunning").toBool(), true);

    simulation->clear();
    QCOMPARE(simulation->property("isRunning").toBool(), false);

    //add components to test timer
    simulation->add_compartment();
    simulation->add_compartment();
    auto compartments = simulation->get_compartments();
    compartments[0]->set_initial_amount(100);
    compartments[0]->set_current_amount(100);
    compartments[1]->set_initial_amount(0);
    compartments[1]->set_current_amount(0);

    simulation->set_connection_mode(true);
    simulation->set_source_compartment(compartments[0]);
    simulation->set_target_compartment(compartments[1]);
    simulation->add_connection();

    simulation->add_variable("rate", 0.1);
    auto connections = simulation->get_connections();
    connections[0]->set_rate_expression("rate");

    //need to wait for a bit to allow timer to go
    simulation->start();
    QTest::qWait(200);
    QVERIFY(simulation->property("currentTime").toInt() > 0);
    simulation->pause();
}

void TestSimulation::test_evolution() {
    //test flow of amount between compartments through connections
    //use time steps to check at each second

    auto simulation = std::make_unique<Simulation>();
    QSignalSpy addGraphingValuesSpy(simulation.get(), SIGNAL(addGraphingValues(double, QVariant)));
    QSignalSpy currentTimeSpy(simulation.get(), SIGNAL(currentTimeChanged()));

    //add some components and variables
    simulation->add_compartment();
    simulation->add_compartment();
    auto compartments = simulation->get_compartments();
    compartments[0]->set_initial_amount(100);
    compartments[0]->set_current_amount(100);
    compartments[1]->set_initial_amount(0);
    compartments[1]->set_current_amount(0);

    simulation->set_connection_mode(true);
    simulation->set_source_compartment(compartments[0]);
    simulation->set_target_compartment(compartments[1]);
    simulation->add_connection();

    simulation->add_variable("rate", 0.1);
    auto connections = simulation->get_connections();
    connections[0]->set_rate_expression("rate");

    simulation->take_time_step();
    QCOMPARE(currentTimeSpy.count(), 1);
    QCOMPARE(simulation->property("currentTime").toInt(), 1);
    QCOMPARE(addGraphingValuesSpy.count(), 1);

    //test compartment values after one step
    //this is the calculations
    double expected_A1 = 90.0; //100.0 * (1.0 - 0.1);
    double expected_B1 = 10.0; //100.0 * 0.1;
    QCOMPARE(compartments[0]->get_current_amount(), expected_A1);
    QCOMPARE(compartments[1]->get_current_amount(), expected_B1);

    //some more time has passed, test current time is increasing
    simulation->take_time_step();
    QCOMPARE(currentTimeSpy.count(), 2);
    QCOMPARE(simulation->property("currentTime").toInt(), 2);

    //the calculations after more time has passed
    expected_A1 = expected_A1 * (1.0 - 0.1); //81.0
    expected_B1 = expected_B1 + 90.0 * 0.1;  //10.0 + 9.0 = 19.0
    QCOMPARE(compartments[0]->get_current_amount(), expected_A1);
    QCOMPARE(compartments[1]->get_current_amount(), expected_B1);

    //test case of a connection going both ways between compartments
    simulation->set_source_compartment(compartments[1]);
    simulation->set_target_compartment(compartments[0]);
    simulation->add_connection();
    auto reverse_connection = simulation->get_connections()[1];
    simulation->add_variable("reverse_rate", 0.05);
    reverse_connection->set_rate_expression("reverse_rate");
    simulation->take_time_step();

    //math for expected flow values
    // A1 loses 0.1 to B1
    // B1 loses 0.05 to A1
    double flow_A1_to_B1 = expected_A1 * 0.1;
    double flow_B1_to_A1 = expected_B1 * 0.05;
    expected_A1 = expected_A1 - flow_A1_to_B1 + flow_B1_to_A1;
    expected_B1 = expected_B1 - flow_B1_to_A1 + flow_A1_to_B1;
    QCOMPARE(compartments[0]->get_current_amount(), expected_A1);
    QCOMPARE(compartments[1]->get_current_amount(), expected_B1);

    //final time step
    int timeSteps = simulation->property("timeSteps").toInt();
    int steps_to_take = timeSteps - simulation->property("currentTime").toInt() - 1;
    for (int i = 0; i < steps_to_take; i++) {
        simulation->take_time_step();
    }
    simulation->take_time_step();
    QCOMPARE(simulation->property("currentTime").toInt(), timeSteps);

    //test to check simulation stops running after final step
    simulation->start();
    simulation->take_time_step();
    QCOMPARE(simulation->property("isRunning").toBool(), false);

    simulation->reset();
    compartments[0]->set_current_amount(100);
    compartments[1]->set_current_amount(0);
    connections[0]->set_rate_expression("rate * " + compartments[1]->get_symbol());

    //first step should transfer nothing bc B1 is 0
    simulation->take_time_step();
    QCOMPARE(compartments[0]->get_current_amount(), 100.0);
    QCOMPARE(compartments[1]->get_current_amount(), 0.0);

    //set B1 to a 10.0 and take another step
    compartments[1]->set_current_amount(10.0);
    simulation->take_time_step();

    //test current amount is still being transferred
    //math
    // A1 loses 0.1 * A1 * B1 = 0.1 * 100 * 10 = 100
    QVERIFY(compartments[0]->get_current_amount() < 100.0);
    QVERIFY(compartments[1]->get_current_amount() > 10.0);
}

#include "TestSimulation.moc"