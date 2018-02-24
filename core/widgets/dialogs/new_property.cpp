#include "new_property.h"
#include "ui_new_property.h"

using namespace widgets;

NewPropertyDialog::NewPropertyDialog(QWidget *parent) :
  QDialog(parent),
  m_ui(new Ui::NewProperty) {
  m_ui->setupUi(this);
}

NewPropertyDialog::~NewPropertyDialog() {
  delete m_ui;
}
