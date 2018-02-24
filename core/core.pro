#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T12:30:54
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = core
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
  main.cpp \
  widgets/main_window.cpp \
  widgets/dialogs/new_map.cpp \
  widgets/dialogs/new_project.cpp \
  widgets/dialogs/new_property.cpp \
    ../plugins/ext_lineedit/ext_lineedit.cpp

HEADERS += \
  widgets/main_window.h \
  widgets/dialogs/new_map.h \
  widgets/dialogs/new_project.h \
  data/project.h \
  data/map.h \
  widgets/dialogs/new_property.h \
    data/project_element.h \
    ../plugins/ext_lineedit/ext_lineedit.h

FORMS += \
  widgets/main_window.ui \
  widgets/dialogs/new_project.ui \
  widgets/dialogs/new_map.ui \
  widgets/dialogs/new_property.ui

RESOURCES += \
  res.qrc

DISTFILES += \
  resources/default_style.qss

INCLUDEPATH += \
  $$PWD/.. \
  $$PWD/../netcom_c11

unix:!macx|win32: LIBS += -L$$OUT_PWD/../netcom_lib/src/ -lnetcom11
