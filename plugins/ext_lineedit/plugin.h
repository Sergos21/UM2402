#pragma once

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class ExtLineEditPlugin : public QObject, public QDesignerCustomWidgetInterface {
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
#endif // QT_VERSION >= 0x050000

public:
  ExtLineEditPlugin(QObject *parent = Q_NULLPTR);

  bool isContainer() const Q_DECL_OVERRIDE;
  bool isInitialized() const Q_DECL_OVERRIDE;
  QIcon icon() const Q_DECL_OVERRIDE;
  QString domXml() const Q_DECL_OVERRIDE;
  QString group() const Q_DECL_OVERRIDE;
  QString includeFile() const Q_DECL_OVERRIDE;
  QString name() const Q_DECL_OVERRIDE;
  QString toolTip() const Q_DECL_OVERRIDE;
  QString whatsThis() const Q_DECL_OVERRIDE;
  QWidget *createWidget(QWidget *parent) Q_DECL_OVERRIDE;
  void initialize(QDesignerFormEditorInterface *core) Q_DECL_OVERRIDE;

private:
  bool m_initialized;
};
