/**
 * @file TestConnection.cpp
 * @author Nitish Maindoliya, Dominik Leisinger, Bram Hogg
 */

#include "TestConnection.h"

void TestConnection::test_constructors() {
  //default constructor
  Connection defaultConn;
  QCOMPARE(defaultConn.get_source(), nullptr);
  QCOMPARE(defaultConn.get_target(), nullptr);
  QCOMPARE(defaultConn.get_simulation(), nullptr);
  QCOMPARE(defaultConn.get_name(), QString("Test"));
  QCOMPARE(defaultConn.get_rate_expression(), QString("0"));

  //variable constructor
  Compartment sourcecomp;
  Compartment targetcomp;
  QString name = "TestName";
  QString rateexp = "0.01";

  Connection paramconn(name, &sourcecomp, &targetcomp, rateexp);
  QCOMPARE(paramconn.get_source(), &sourcecomp);
  QCOMPARE(paramconn.get_target(), &targetcomp);
  QCOMPARE(paramconn.get_name(), name);
  QCOMPARE(paramconn.get_rate_expression(), rateexp);
}

void TestConnection::test_getter_setters() {

  Connection defaultconn;

  //Name Section
  QCOMPARE(defaultconn.get_name(), QString("Test"));

  defaultconn.set_name("Test Name");
  QCOMPARE(defaultconn.get_name(), QString("Test Name"));

  // Name with invalid characters - no change
  defaultconn.set_name("InvalidName!!");
  QCOMPARE(defaultconn.get_name(), QString("Test Name"));

  // Empty name - no change
  defaultconn.set_name("");
  QCOMPARE(defaultconn.get_name(), QString("Test Name"));

  //more name set/get
  Connection connection;

  connection.set_name("tested");
  QCOMPARE(connection.get_name(), QString("tested"));

  // Empty name - no change
  connection.set_name("");
  QCOMPARE(connection.get_name(), QString("tested"));

  //Source/Target Section

  Compartment SourceComp;
  Compartment TargetComp;

  QCOMPARE(defaultconn.get_source(), nullptr);
  QCOMPARE(defaultconn.get_target(), nullptr);

  defaultconn.set_source(&SourceComp);
  defaultconn.set_target(&TargetComp);

  QCOMPARE(defaultconn.get_source(), &SourceComp);
  QCOMPARE(defaultconn.get_target(), &TargetComp);

  defaultconn.set_target(&SourceComp);
  QCOMPARE(defaultconn.get_target(), &SourceComp);

  // more set/get targets and sources
  Compartment compartment1;
  Compartment compartment2;

  connection.set_source(&compartment1);
  connection.set_target(&compartment2);
  QCOMPARE(connection.get_source(), &compartment1);
  QCOMPARE(connection.get_target(), &compartment2);

  Compartment compartment3;
  connection.set_source(&compartment3);
  connection.set_target(&compartment1);
  QCOMPARE(connection.get_source(), &compartment3);
  QCOMPARE(connection.get_target(), &compartment1);

  //Rate expression section
  QCOMPARE(defaultconn.get_rate_expression(), "0");
  defaultconn.set_rate_expression("0.001");
  QCOMPARE(defaultconn.get_rate_expression(), "0.001");

  QString rate_exp = "HHHlskk";
  QString rate_exp2 = "(09..,/sdf/2";

  connection.set_rate_expression(rate_exp);
  QCOMPARE(connection.get_rate_expression(), "HHHlskk");
  connection.set_rate_expression(rate_exp2);
  QCOMPARE(connection.get_rate_expression(), "(09..,/sdf/2");

  rate_exp = "LJKMNB99s.";
  connection.set_rate_expression(rate_exp);
  QCOMPARE(connection.get_rate_expression(), "LJKMNB99s.");

  QString rate_exp3 = " ";
  connection.set_rate_expression(rate_exp3);
  QCOMPARE(connection.get_rate_expression(), " ");

  //Simulation Section
  //    QCOMPARE(defaultconn.get_simulation(), nullptr);
  //    Simulation sim;
  //    defaultconn.set_simulation(&sim);
  //    QCOMPARE(defaultconn.get_simulation(), &sim);
  //
  //    // more set/get simulation
  //    Simulation* simulation1;
  //    Simulation* simulation2;
  //    connection.set_simulation(simulation1);
  //    QCOMPARE(connection.get_simulation(), simulation1);
  //
  //    connection.set_simulation(simulation2);
  //    QCOMPARE(connection.get_simulation(), simulation2);
}
void TestConnection::test_add_connection() {
  Simulation sim;

  sim.add_compartment();
  sim.add_compartment();

  QVector<Compartment*> compartments = sim.get_compartments();
  QCOMPARE(compartments.size(), 2);

  int initialConnCount = sim.get_connections().count();

  sim.set_connection_mode(true);

  Compartment* source = compartments[0];
  Compartment* target = compartments[1];

  sim.set_source_compartment(source);
  sim.set_target_compartment(target);
  sim.add_connection();

  ///Test if new connection created
  QCOMPARE(sim.get_connections().count(), initialConnCount + 1);

  QVector<Connection*> connections = sim.get_connections();
  QVERIFY(!connections.empty());
  Connection* connection = connections[0];

  QCOMPARE(connection->get_source(), source);
  QCOMPARE(connection->get_target(), target);

  ///Test Connection name + base values
  QString expectedName = QString("Connection %1 %2").arg(source->get_symbol(), target->get_symbol());
  QCOMPARE(connection->get_name(), expectedName);
  QCOMPARE(connection->get_rate_expression(), QString("0"));

  ///Test cannot make duplicate connections
  sim.set_connection_mode(source);
  sim.set_target_compartment(target);
  sim.add_connection();
  QCOMPARE(sim.get_connections().count(), initialConnCount + 1);

  ///Test can make a reverse connection
  sim.set_source_compartment(target);
  sim.set_target_compartment(source);
  sim.add_connection();
  QCOMPARE(sim.get_connections().count(), initialConnCount + 2);
}
void TestConnection::test_update_connection() {

  Simulation simulation;
  simulation.add_compartment();
  simulation.add_compartment();
  //compartment getter
  QVector<Compartment*> compartments = simulation.get_compartments();
  QCOMPARE(compartments.size(), 2);

  simulation.set_connection_mode(true);

  Compartment* source = compartments[0];
  Compartment* target = compartments[1];
  simulation.set_source_compartment(source);

  QString connection_name = QString("Connection %1 %2").arg(source->get_symbol(), target->get_symbol());
  Connection connection = Connection(connection_name, source, target, "0.1", &simulation);

  //test properties of the connection
  QCOMPARE(connection.get_source(), source);
  QCOMPARE(connection.get_target(), target);
  QCOMPARE(connection.get_rate_expression(), QString("0.1"));

  //symbol updates
  simulation.set_target_compartment(target);
  simulation.add_connection();
  QString new_connection_name = connection.get_name();
  QCOMPARE(new_connection_name, "Connection F1 E1");

  source->set_symbol("G4");
  target->set_symbol("D9");
  QString news_name = QString("Connection %1 %2").arg(source->get_symbol(), target->get_symbol());
  connection.set_name(news_name);
  QString connection_name_G4 = connection.get_name();
  QCOMPARE(connection_name_G4, "Connection G4 D9");

  //add new target after set rate expression
  connection.set_rate_expression("2.3");
  simulation.add_compartment();
  compartments = simulation.get_compartments();
  Compartment* new_target = compartments[2];
  simulation.set_target_compartment(new_target);
  simulation.add_connection();

  target->set_symbol("D2");
  QString another_name = QString("Connection %1 %2").arg(source->get_symbol(), target->get_symbol());
  connection.set_name(another_name);
  QString connection_name_D2 = connection.get_name();
  QCOMPARE(connection_name_D2, "Connection G4 D2");
  QCOMPARE(connection.get_rate_expression(), "2.3");

  simulation.set_target_compartment(source);
  simulation.add_connection();

  target->set_symbol("G3");
  simulation.set_source_compartment(target);
  QString name3 = QString("Connection %1 %2").arg(source->get_symbol(), target->get_symbol());
  connection.set_name(name3);
  QString new_connection_name_D2 = connection.get_name();
  QCOMPARE(new_connection_name_D2, "Connection G4 G3");
  QCOMPARE(connection.get_rate_expression(), "2.3");
}

#include "TestConnection.moc"
