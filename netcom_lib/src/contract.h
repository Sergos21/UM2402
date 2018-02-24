/*! ***************************************************************************
* Проект: src.pro
*
* @file contract.h
*
* @brief: Заголовочный файл класса контракта
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QMetaObject>
#include <QVariant>

#include "common.h"

namespace netcom {

class Contract;

bool NETCOM_SHARED_EXPORT contractClone(const Contract *source, Contract *dest);

class NETCOM_SHARED_EXPORT Contract {
public:
  Contract() Q_DECL_EQ_DEFAULT;
  virtual ~Contract() Q_DECL_EQ_DEFAULT;
  Contract(const Contract &other);
  Contract(Contract &&other) Q_DECL_EQ_DEFAULT;
  Contract &operator=(const Contract &other);
  Contract &operator=(Contract &&other) Q_DECL_EQ_DEFAULT;

  virtual const char *netcomName() const = 0;
  virtual const QMetaObject *metaObject() const = 0;

  bool netcomClone(const Contract *source);
  void netcomClear();

  static const int netcom_variant_type;
};

} // namespace netcom

Q_DECLARE_METATYPE(::netcom::Contract*)
