/**
 * @file main.cpp
 * @author Nitish Maindoliya
 */

#include <QtTest>
#include "TestCompartment.h"
#include "TestConnection.h"
#include "TestSimulation.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  int status = 0;

  // Register and run all tests
  {
    TestCompartment compartment_test;
    status |= QTest::qExec(&compartment_test, argc, argv);
  }

  {
    TestConnection connection_test;
    status |= QTest::qExec(&connection_test, argc, argv);
  }

  {
    TestSimulation simulation_test;
    status |= QTest::qExec(&simulation_test, argc, argv);
  }

  return status;
}