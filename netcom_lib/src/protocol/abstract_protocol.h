/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/abstract_protocol.h
*
* @brief: Заголовочный файл общего интерфейса для описания всех протоколов
* нижнего уровня
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include "src/common.h"

namespace netcom {
namespace protocols {

static QString NULL_ID { };

class NETCOM_SHARED_EXPORT AbstractProtocol {
public:
  AbstractProtocol() Q_DECL_EQ_DEFAULT;
  AbstractProtocol(const AbstractProtocol&) Q_DECL_EQ_DELETE;
  virtual ~AbstractProtocol() Q_DECL_EQ_DEFAULT;
  AbstractProtocol &operator=(const AbstractProtocol&) Q_DECL_EQ_DELETE;

  virtual ProtocolType getProtocolType() const = 0;

  virtual bool openProtocol()         = 0;
  virtual bool isOpenProtocol() const = 0;
  virtual bool closeProtocol()        = 0;

  virtual qint64 writeMessage(const QByteArray &data, const QString &id = NULL_ID) = 0;
  virtual qint64 readMessage(QByteArray &data, QString &id = NULL_ID) = 0;
};

} // namespace protocols
} // namespace netcom

