#include "ext_lineedit.h"
#include "plugin.h"

#include <QtPlugin>

ExtLineEditPlugin::ExtLineEditPlugin(QObject *parent) :
  QObject(parent),
  m_initialized(false) {
}

void ExtLineEditPlugin::initialize(QDesignerFormEditorInterface * /* core */) {
  if (m_initialized) {
    return;
  }
  // Add extension registrations, etc. here
  m_initialized = true;
}

bool ExtLineEditPlugin::isInitialized() const {
  return m_initialized;
}

QWidget *ExtLineEditPlugin::createWidget(QWidget *parent) {
  return new ExtLineEdit(parent);
}

QString ExtLineEditPlugin::name() const {
  return QLatin1String("ExtLineEdit");
}

QString ExtLineEditPlugin::group() const {
  return QLatin1String("Input Widgets");
}

QIcon ExtLineEditPlugin::icon() const {
  return QIcon();
}

QString ExtLineEditPlugin::toolTip() const {
  return QLatin1String("");
}

QString ExtLineEditPlugin::whatsThis() const {
  return QLatin1String("");
}

bool ExtLineEditPlugin::isContainer() const {
  return false;
}

QString ExtLineEditPlugin::domXml() const {
  return QLatin1String(
    "<widget class=\"ExtLineEdit\" name=\"extLineEdit\">\n"
    " <property name=\\\"geometry\\\">\n"
    "  <rect>\n"
    "   <x>0</x>\n"
    "   <y>0</y>\n"
    "   <width>250</width>\n"
    "   <height>30</height>\n"
    "  </rect>\n"
    " </property>\n"
    "</widget>\n");
}

QString ExtLineEditPlugin::includeFile() const {
  return QLatin1String("ext_lineedit.h");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ext_lineedit, ExtLineEditPlugin)
#endif // QT_VERSION < 0x050000
