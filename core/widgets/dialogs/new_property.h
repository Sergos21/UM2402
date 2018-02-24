#pragma once

#include <QDialog>

namespace Ui {
  class NewProperty;
}

namespace widgets {

class NewPropertyDialog : public QDialog {
  Q_OBJECT
public:
  explicit NewPropertyDialog(QWidget *parent = Q_NULLPTR);
  ~NewPropertyDialog();

private:
  Ui::NewProperty *m_ui;
};

} // namespace widgets

