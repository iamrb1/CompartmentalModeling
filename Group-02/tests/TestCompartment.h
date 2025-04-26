/**
 * @file TestCompartment.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef TESTCOMPARTMENT_H
#define TESTCOMPARTMENT_H

#include <QTest>
#include <Application/Simulation.h>
#include <Components/Compartment.h>
#include <Components/Connection.h>

class TestCompartment : public QObject {
  Q_OBJECT

 private slots:
  void test_constructors();

  void test_getter_setters();

  void test_add_compartment();

  void test_update_connection();

  void test_delete_compartment();
};

#endif  //TESTCOMPARTMENT_H
