CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(ext_lineedit)
TEMPLATE    = lib

HEADERS     = plugin.h
SOURCES     = plugin.cpp
RESOURCES   = res.qrc

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += designer
} else {
  CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(ext_lineedit.pri)
