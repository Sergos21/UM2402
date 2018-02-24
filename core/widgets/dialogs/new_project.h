#pragma once

#include <QDialog>

namespace Ui {
  class NewProject;
}

namespace widgets {

class NewProjectDialog : public QDialog {
  Q_OBJECT
public:
  explicit NewProjectDialog(QWidget *parent = Q_NULLPTR);
  ~NewProjectDialog();

private:
  Ui::NewProject *m_ui;
};

} // namespace widgets
