#pragma once

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

namespace widgets {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = Q_NULLPTR);
  ~MainWindow();

private slots:
  void on_action_NewMap_triggered();
  void on_actionNewProject_triggered();

  void on_actionExit_triggered();

private:
  Ui::MainWindow *m_ui;
};

} // namespace widgets
