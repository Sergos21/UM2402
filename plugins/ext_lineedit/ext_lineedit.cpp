#include "ext_lineedit.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
//--------------------------------------------------------------------------------------
ExtLineEdit::ExtLineEdit(QWidget *parent) :
  QWidget(parent) {
  QLayout *layout = new QHBoxLayout(this);
  m_label         = new QLabel(this);
  m_line_edit     = new QLineEdit(this);

  layout->setMargin(0);
  layout->addWidget(m_label);
  layout->addWidget(m_line_edit);
  this->setLayout(layout);
}
//--------------------------------------------------------------------------------------
ExtLineEdit::~ExtLineEdit() {

}
//--------------------------------------------------------------------------------------
QString ExtLineEdit::label() const {
  return m_label->text();
}
//--------------------------------------------------------------------------------------
void ExtLineEdit::setLabel(const QString &text) {
  m_label->setText(text);
}
//--------------------------------------------------------------------------------------
void ExtLineEdit::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
}
//--------------------------------------------------------------------------------------
