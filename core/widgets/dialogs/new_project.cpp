#include "new_project.h"
#include "ui_new_project.h"

using namespace widgets;

NewProjectDialog::NewProjectDialog(QWidget *parent) :
  QDialog(parent),
  m_ui(new Ui::NewProject) {
  m_ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog() {
  delete m_ui;
}
