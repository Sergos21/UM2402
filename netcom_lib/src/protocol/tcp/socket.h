/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/tcp/socket.h
*
* @brief: Заголовочный файл вспомогательного класса для работы с протоколом
* нижнего уровня Ethernet - TCP/IP
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QtNetwork/QTcpSocket>

namespace netcom {
namespace protocols {
namespace inner {

class TcpSocket : public QTcpSocket {
  Q_OBJECT
public:
  explicit TcpSocket(QObject *parent = Q_NULLPTR);
  virtual ~TcpSocket() Q_DECL_EQ_DEFAULT;

signals:
  void sigReadMessage(QByteArray data);

public slots:
  void slotWriteMessage(const QByteArray &data);
  void slotReadMessage();
};

} // namespace inner
} // namespace protocols
} // namespace netcom

