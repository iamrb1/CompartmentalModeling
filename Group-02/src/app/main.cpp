/**
 * @file main.cpp
 * @author Nitish Maindoliya
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>

int main(int argc, char *argv[]) {
  const QGuiApplication app(argc, argv);

  QGuiApplication::setOrganizationName("cseg2");
  QGuiApplication::setOrganizationDomain("https://github.com/CSE498/CSE498-Spring2025/");
  QGuiApplication::setApplicationName("Compartmental Modeling Simulator");

  QQmlApplicationEngine engine;
  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection
  );

  engine.loadFromModule("cseg2", "Main");

  if (const QScreen* screen = QGuiApplication::primaryScreen()) {
    const QRect screen_geometry = screen->geometry();
    const int window_width = static_cast<int>(screen_geometry.width() * 0.7);
    const int window_height = static_cast<int>(screen_geometry.height() * 0.7);
    const int x = (screen_geometry.width() - window_width) / 2;
    const int y = (screen_geometry.height() - window_height) / 2;

    const auto& root_objects = engine.rootObjects();
    root_objects.first()->setProperty("width", window_width);
    root_objects.first()->setProperty("height", window_height);
    root_objects.first()->setProperty("x", x);
    root_objects.first()->setProperty("y", y);
  }

  return QGuiApplication::exec();
}
