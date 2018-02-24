#include "main_window.h"
#include "ui_main_window.h"

#include "dialogs/new_project.h"
#include "dialogs/new_map.h"

using namespace widgets;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete m_ui;
}

void widgets::MainWindow::on_action_NewMap_triggered() {
  NewMapDialog dialog(m_ui->centralWidget);
  dialog.exec();
}

void widgets::MainWindow::on_actionNewProject_triggered() {
  NewProjectDialog dialog(m_ui->centralWidget);
  dialog.exec();
}

void widgets::MainWindow::on_actionExit_triggered() {
  close();
}
