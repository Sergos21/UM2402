/*! ***************************************************************************
* Проект: src.pro
*
* @file serialization/json.h
*
* @brief: Заголовочный файл класса сериализатора для XML-формата
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "src/common.h"

namespace netcom {

class Contract;

namespace serialization {
namespace inner {

enum class XMLConvertType {
  Standard,
  Attributes
};

class NETCOM_SHARED_EXPORT XML {
public:
  static QByteArray serialize(const Contract *contract, bool *ok = Q_NULLPTR);
  static Contract *deserialize(const QByteArray &data, bool *ok = Q_NULLPTR);
  static bool tryDeserialize(Contract *contract, const QByteArray &data);
};

} // namespace inner

} // namespace serialization
} // namespace netcom
