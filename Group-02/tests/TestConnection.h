/**
 * @file TestConnection.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H

#include <QTest>
#include <Application/Simulation.h>
#include <Components/Compartment.h>
#include <Components/Connection.h>

class TestConnection : public QObject {
  Q_OBJECT

 private slots:
  void test_constructors();

  void test_getter_setters();

  void test_add_connection();

  void test_update_connection();
};

#endif  //TESTCONNECTION_H
