/**
 * @file main.cpp
 * @author Nitish Maindoliya
 */

#include <QQmlContext>
#include <QScreen>
#include "MainFrame.h"

int main(int argc, char *argv[]) {
    MainFrame frame;
    return frame.Initialize(argc, argv);

}
