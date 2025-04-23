/**
 * @file TestSimulation.h
 * @author Nitish Maindoliya
 *
 *
 */

#ifndef TESTSIMULATION_H
#define TESTSIMULATION_H

#include <QTest>
#include <app/Simulation.h>
#include <app/Components/Compartment.h>
#include <app/Components/Connection.h>

class TestSimulation : public QObject {
  Q_OBJECT

 private slots:
  void test_constructors();

  void test_getter_setters();

  void test_xml_loading_saving();

  void test_controls();

  void test_timer();

  void test_evolution();
};

#endif  //TESTSIMULATION_H
