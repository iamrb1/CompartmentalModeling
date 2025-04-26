/**
 * @file main.cpp
 * @author Nitish Maindoliya
 */

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QScreen>


int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QGuiApplication::setOrganizationName("cseg2");
  QGuiApplication::setOrganizationDomain("https://github.com/CSE498/CSE498-Spring2025/");
  QGuiApplication::setApplicationName("Compartmental Modeling Simulator");

  QQmlApplicationEngine engine;

  engine.loadFromModule("Application", "SimulationUI");

  if (engine.rootObjects().isEmpty())
    return -1;

  const auto& root = engine.rootObjects();

  if (const QScreen* screen = QGuiApplication::primaryScreen()) {
    const auto screen_geometry = screen->geometry();
    const int w = static_cast<int>(screen_geometry.width() * 0.7);
    const int h = static_cast<int>(screen_geometry.height() * 0.7);
    const int x = (screen_geometry.width() - w) / 2;
    const int y = (screen_geometry.height() - h) / 2;
    const auto rootObj = root.first();
    rootObj->setProperty("width", w);
    rootObj->setProperty("height", h);
    rootObj->setProperty("x", x);
    rootObj->setProperty("y", y);
  }

  return QApplication::exec();
}
