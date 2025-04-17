#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QObject>
#include <QtQml>

class Simulation;

class Sidebar : public QObject {
  Q_OBJECT
  QML_ELEMENT
 private:
  Simulation* m_simulation = nullptr;
  //enum m_mode;


 public:
  explicit Sidebar(QObject* parent = nullptr);

  void set_simulation(Simulation* simulation){m_simulation = simulation;};
  Q_INVOKABLE void add_variable(const std::string& id, const float &var);
  Q_INVOKABLE void remove_variable(const std::string& id);



 signals:
  void variable_emmited(const std::string& id, const float &var);

  void variable_remove_emmited(const std::string& id);


};

#endif  // SIDEBAR_H
