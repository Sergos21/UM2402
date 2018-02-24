###############################################################################
# ! Проект: src.pro
#
# @file src/src.pro
#
# @brief Файл проекта библиотеки netcom_lib
#
# @note
#
# @author Лола С.А.
###############################################################################
QT  += network

QT  -= gui

TARGET = netcom11
TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION, 4) {
  CONFIG += c++11
} else {
  error("For library `netcom_c11` needed Qt5 framework")
}

#QMAKE_EXTENSION_SHLIB = so

DEFINES += NETCOM_LIBRARY

INCLUDEPATH += $$PWD/..

HEADERS += \
  common.h \
  protocol/tcp/client.h \
  protocol/tcp/server.h \
  serialization/json.h \
  serialization/xml.h \
  property/abstract_property.h \
  type_manager.h \
  property/p_base.h \
  property/p_contract.h \
  contract.h \
  property/p_variant.h \
  factory.h \
  macros.h \
  property/p_s_container.h \
  protocol/abstract_protocol.h \
  protocol/tcp/socket.h \
  helpers.h \
  property/inner_value.h \
  property/p_enum.h

PRECOMPILED_HEADER +=

SOURCES += \
  serialization/xml.cpp \
  serialization/json.cpp \
  protocol/tcp/client.cpp \
  protocol/tcp/server.cpp \
  contract.cpp \
  factory.cpp \
  protocol/tcp/socket.cpp

unix:win32:macx: DESTDIR = $$PWD/../bin
