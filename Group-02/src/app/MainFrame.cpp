/**
* @file Application.cpp
 * @author Nitish Maindoliya, Matthew Hawkins, Bram Hogg
 */
#include "MainFrame.h"
#include "Simulation.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QFile>
#include <QDebug>

int MainFrame::Initialize(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  QGuiApplication::setOrganizationName("cseg2");
  QGuiApplication::setOrganizationDomain("https://github.com/CSE498/CSE498-Spring2025/");
  QGuiApplication::setApplicationName("Compartmental Modeling Simulator");

  QQmlApplicationEngine engine;
  m_simulation = new Simulation();

  engine.rootContext()->setContextProperty(QStringLiteral("mainFrame"), this);

  engine.loadFromModule("cseg2", "Main");
  if (engine.rootObjects().isEmpty())
    return -1;

  QObject* root = engine.rootObjects().first();
  if (auto* saveDlg = root->findChild<QObject*>("saveFileDialog")) {
    QObject::connect(saveDlg, SIGNAL(accepted(QUrl)), this, SLOT(save_simulation(QUrl)));
  }
  if (auto* openDlg = root->findChild<QObject*>("openFileDialog")) {
    QObject::connect(openDlg, SIGNAL(accepted(QUrl)), this, SLOT(load_simulation(QUrl)));
  }

  if (const QScreen* screen = QGuiApplication::primaryScreen()) {
    const QRect g = screen->geometry();
    int w = int(g.width() * 0.7);
    int h = int(g.height() * 0.7);
    int x = (g.width() - w) / 2;
    int y = (g.height() - h) / 2;
    auto rootObj = engine.rootObjects().first();
    rootObj->setProperty("width", w);
    rootObj->setProperty("height", h);
    rootObj->setProperty("x", x);
    rootObj->setProperty("y", y);
  }

  return app.exec();
}

void MainFrame::save_simulation(const QUrl& fileUrl) {
  const QString local_path = fileUrl.toLocalFile();
  m_simulation->save_xml(local_path.toStdString());
}

void MainFrame::load_simulation(const QUrl& fileUrl) {
  const QString local_path = fileUrl.toLocalFile();
  m_simulation->load_xml(local_path.toStdString());
}
