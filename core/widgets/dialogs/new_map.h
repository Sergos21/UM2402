#pragma once

#include <QDialog>

namespace Ui {
  class NewMap;
}

namespace widgets {

class NewMapDialog : public QDialog {
  Q_OBJECT
public:
  explicit NewMapDialog(QWidget *parent = Q_NULLPTR);
  ~NewMapDialog();

private:
  Ui::NewMap *m_ui;
};

} // namespace widgets
