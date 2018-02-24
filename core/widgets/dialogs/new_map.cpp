#include "new_map.h"
#include "ui_new_map.h"

using namespace widgets;

NewMapDialog::NewMapDialog(QWidget *parent) :
  QDialog(parent),
  m_ui(new Ui::NewMap) {
  m_ui->setupUi(this);
}

NewMapDialog::~NewMapDialog() {
  delete m_ui;
}
