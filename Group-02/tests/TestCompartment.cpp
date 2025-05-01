/**
 * @file TestCompartment.cpp
 * @author Nitish Maindoliya, Dominik Leisinger
 */

#include "TestCompartment.h"

void TestCompartment::test_constructors() {
  Compartment defaultComp;
  QCOMPARE(defaultComp.get_name(), QString("Test"));
  QCOMPARE(defaultComp.get_symbol(), QString("X"));
  QCOMPARE(defaultComp.get_initial_amount(), 0.0);
  QCOMPARE(defaultComp.get_current_amount(), 0.0);
  QCOMPARE(defaultComp.get_x(), 100);
  QCOMPARE(defaultComp.get_y(), 100);

  QString name = "ParamComp";
  QString symbol = "PC";
  double initamount = 20.0;

  Compartment ParamComp(name, symbol, initamount);
  QCOMPARE(ParamComp.get_name(), name);
  QCOMPARE(ParamComp.get_symbol(), symbol);
  QCOMPARE(ParamComp.get_initial_amount(), initamount);
  QCOMPARE(ParamComp.get_current_amount(), 20.0);
  QCOMPARE(ParamComp.get_x(), 100);
  QCOMPARE(ParamComp.get_y(), 100);


}
void TestCompartment::test_getter_setters() {
  Compartment compartment;

  //Name Section

  QCOMPARE(compartment.get_name(), QString("Test"));

  compartment.set_name("Test Name");
  QCOMPARE(compartment.get_name(), QString("Test Name"));


  // Name with invalid characters - no change
  compartment.set_name("InvalidName!!");
  QCOMPARE(compartment.get_name(), QString("Test Name"));


  // Empty name - no change
  compartment.set_name("");
  QCOMPARE(compartment.get_name(), QString("Test Name"));

  //Symbol Section

  Simulation simulation;
  //Need valid simulation for set_symbol()
  compartment.set_simulation(&simulation);

  QCOMPARE(compartment.get_symbol(), QString("X"));

  compartment.set_symbol("AB");
  QCOMPARE(compartment.get_symbol(), QString("AB"));


  // Symbol with invalid characters - no change
  compartment.set_symbol("A$");
  QCOMPARE(compartment.get_symbol(), QString("AB"));


  // Empty symbol - no change
  compartment.set_symbol("");
  QCOMPARE(compartment.get_symbol(), QString("AB"));

  //Coordinate section

  QCOMPARE(compartment.get_x(), 100);
  QCOMPARE(compartment.get_x(), 100);

  compartment.set_x(200);
  compartment.set_y(200);
  QCOMPARE(compartment.get_x(), 200);
  QCOMPARE(compartment.get_y(), 200);

  //Amount Section

  QCOMPARE(compartment.get_initial_amount(), 0.0);
  QCOMPARE(compartment.get_current_amount(), 0.0);

  compartment.set_initial_amount(40.0);
  compartment.set_current_amount(45.0);

  QCOMPARE(compartment.get_initial_amount(), 40.0);
  QCOMPARE(compartment.get_current_amount(), 45.0);

}
void TestCompartment::test_add_compartment() {
  Simulation sim;

  QCOMPARE(sim.get_compartments().count(), 0);
  sim.add_compartment();
  QCOMPARE(sim.get_compartments().count(), 1);

  auto compartments = sim.get_compartments();
  QVERIFY(!compartments.empty());

  auto compartment = compartments.last();
  QString symbol = compartment->get_symbol();
  sim.remove_compartment(symbol);
  QCOMPARE(sim.get_compartments().count(), 0);

}
void TestCompartment::test_update_connection() {
  Simulation sim;

  sim.add_compartment();
  sim.add_compartment();

  auto compartments = sim.get_compartments();
  QCOMPARE(compartments.size(), 2);

  sim.set_connection_mode(true);

  auto sourceComp = compartments[0];
  auto targetComp = compartments[1];

  QString connectionName = QString("Connection %1 %2").arg(sourceComp->get_symbol(), targetComp->get_symbol());
  Connection conn(connectionName, sourceComp, targetComp, "0.01", &sim);

  QCOMPARE(conn.get_name(), connectionName);
  QCOMPARE(conn.get_source(), sourceComp);
  QCOMPARE(conn.get_target(), targetComp);
  QCOMPARE(conn.get_rate_expression(), QString("0.01"));

}
void TestCompartment::test_delete_compartment() {
  Simulation sim;

  sim.add_compartment();
  int initialCount = sim.get_compartments().count();

  auto compartments = sim.get_compartments();
  QVERIFY(!compartments.empty());

  QString symbol = compartments[0]->get_symbol();

  sim.remove_compartment(symbol);

  QCOMPARE(sim.get_compartments().count(), initialCount - 1);

  bool found = false;
  for (const auto& comp : sim.get_compartments()) {
    if (comp->get_symbol() == symbol) {
      found = true;
      break;
    }
  }
  QVERIFY(!found);

}

#include "TestCompartment.moc"
