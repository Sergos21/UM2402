###############################################################################
# ! Проект: netcom_lib.pro
#
# @file netcom_lib.pro
#
# @brief Файл подпроектов библиотеки netcom_lib
#
# @note
#
# @author Лола С.А.
###############################################################################

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
  src

HEADERS += \
  netcom_core.h \
  netcom_xml.h \
  netcom_json.h \
  netcom_tcp.h
