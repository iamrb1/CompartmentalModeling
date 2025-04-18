/**
* @file Application.h
 * @author Nitish Maindoliya, Matthew Hawkins, Bram Hogg
 *
 *
 */
#pragma once

#include "Simulation.h"

class MainFrame : public QObject {
  Q_OBJECT
public:
  explicit MainFrame(QObject* parent = nullptr) : QObject(parent) {}

  int Initialize(int argc, char* argv[]);
  Simulation* m_simulation;
public slots:
  void save_simulation(const QUrl& fileUrl);
  void load_simulation(const QUrl& fileUrl);
};
