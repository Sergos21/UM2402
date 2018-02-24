#include "widgets/main_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  widgets::MainWindow win;
  win.showMaximized();

  return app.exec();
}
