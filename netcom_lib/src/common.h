/*! ***************************************************************************
* Проект: src.pro
*
* @file common.h
*
* @brief: Файл общих данных библиотеки netcom_lib
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QtCore/qglobal.h>

#include "macros.h"

/// Макрос обозначающий метку экспорта/импорта данных из/в библиотеку
#if defined(NETCOM_LIBRARY)
#  define NETCOM_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETCOM_SHARED_EXPORT Q_DECL_IMPORT
#endif

/// Переопределенный пользовательский литерал для преобразования типов вида qint16
inline qint16 operator "" _s(unsigned long long value) {
  return static_cast<qint16>(value);
}

/// Переопределенный пользовательский литерал для преобразования типов вида quint16
inline quint16 operator "" _us(unsigned long long value) {
  return static_cast<quint16>(value);
}

/// Определение псевдонима пространства имен QtPrivate
namespace qtp = QtPrivate;

namespace netcom {

enum class ProtocolType : int {
  UNKNOWN,        // протокол передачи отсутствует либо неизвестен
  TCP,            // обмен данными по сетевому протоколу TCP/IP
  // TODO: необходима реализация протокола UDP
  UDP,            // обмен данными по сетевому протоколу UDP
  // TODO: необходима реализация протокола RS-232/RS-485
  COM,            // обмен данными по сетевому протоколу RS-232/RS-485
  // TODO: возможно в будущем понадобится поддерживать старый протокол обмена с ZeroMQ!
  MES_ZEROMQ      // обмен данными с использованием библиотеки mes на ZeroMQ
};

enum class SerializatorType {
  BINARY,         // сериализация данных в бинарном виде через QDataStream
  XML,            // сериализация данных в виде xml-файла
  JSON,           // сериализация данных в виде json-формата
  ELZOND          // сериализация в унифицированный строковый протокол обмена данными, разработанный А. Шиманом
};

namespace utility {

template <typename T>
Q_CONSTEXPR Q_INLINE_TEMPLATE typename std::underlying_type<T>::type enum_value(T value) {
  return static_cast<typename std::underlying_type<T>::type>(value);
}

template <typename T>
Q_CONSTEXPR Q_INLINE_TEMPLATE T enum_value(typename std::underlying_type<T>::type value) {
  return static_cast<T>(value);
}

Q_DECL_CONSTEXPR std::uint32_t fnv1a_32(const char *s, std::size_t len) {
  return ((len ? fnv1a_32(s, len - 1) : 2166136261u) ^ s[len - 1]) * 16777619u;
}

inline void setBoolPtrValue(bool *ok, bool state) {
  if (ok) {
    *ok = state;
  }
}

} // namespace utility
} // namespace netcom

Q_DECL_CONSTEXPR inline std::uint32_t operator "" _hash(const char *s, std::size_t len) {
  return netcom::utility::fnv1a_32(s, len);
}
