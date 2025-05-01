/**
 * @file TestCompartment.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef TESTCOMPARTMENT_H
#define TESTCOMPARTMENT_H

#include <Application/Simulation.h>
#include <Components/Compartment.h>
#include <Components/Connection.h>
#include <QTest>

class TestCompartment : public QObject {
  Q_OBJECT

 private:
  Simulation m_simulation;
  Compartment m_compartment = Compartment("Test", "X", 0.0, &m_simulation);

 private slots:
  void test_constructors();

  void test_getter_setters();

  void test_add_compartment();

  void test_update_compartment();

  void test_delete_compartment();
};

#endif  //TESTCOMPARTMENT_H
