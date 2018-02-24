#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;

QT_END_NAMESPACE
// !!!!!!

class ExtLineEdit : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QString label READ label WRITE setLabel DESIGNABLE true)
public:
  explicit ExtLineEdit(QWidget *parent = Q_NULLPTR);
  virtual ~ExtLineEdit();

  QString label() const;
  void setLabel(const QString &text);

protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
  QLabel *m_label         { Q_NULLPTR };
  QLineEdit *m_line_edit  { Q_NULLPTR };
};

