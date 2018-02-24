/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/tcp/client.h
*
* @brief: Заголовочный файл класса клиента реализующего протокол нижнего уровня
* Ethernet - TCP/IP
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QThread>

#include "src/protocol/abstract_protocol.h"
#include "src/protocol/tcp/socket.h"

namespace netcom {
namespace protocols {

class TcpServer;

namespace inner {
class TcpSocket;
}

class NETCOM_SHARED_EXPORT TcpClient : public QThread, public AbstractProtocol {
  Q_OBJECT
  friend class TcpServer;
  enum InitState {
    InitState_Default,
    InitState_Descriptor
  };
public:
  explicit TcpClient(const QString &peer_ip,
                     int peer_port,
                     QObject *parent = Q_NULLPTR);
  explicit TcpClient(const QString &peer_ip,
                     int peer_port,
                     const QString &local_ip,
                     int local_port,
                     QObject *parent = Q_NULLPTR);
  explicit TcpClient(qptrdiff descriptor,
                     const QString &id,
                     QObject *parent = Q_NULLPTR);
  virtual ~TcpClient();

  virtual ProtocolType getProtocolType() const Q_DECL_OVERRIDE;

  virtual bool openProtocol() Q_DECL_OVERRIDE;
  virtual bool isOpenProtocol() const Q_DECL_OVERRIDE;
  virtual bool closeProtocol() Q_DECL_OVERRIDE;

  virtual qint64 writeMessage(const QByteArray &data, const QString &id = NULL_ID) Q_DECL_OVERRIDE;
  virtual qint64 readMessage(QByteArray &data, QString &id = NULL_ID) Q_DECL_OVERRIDE;

signals:
  void sigClientConnected();
  void sigClientDisconnected();
  void sigClientWriteMessage(const QByteArray &data);
  void sigClientBytesWritten(qint64 bytes);
  void sigClientReadMessage(const QByteArray &data);
  void sigThreadInitialize(bool result);
  void sigSocketError(QAbstractSocket::SocketError error);

protected:
  virtual void run() Q_DECL_OVERRIDE;

private slots:
  void slotOnClientConnected();
  void slotOnClientDisconected();
  void slotOnSocketError(QAbstractSocket::SocketError error);

private:
  QString peer_ip_;
  int peer_port_              { 0 };
  QString local_ip_;
  int local_port_             { 0 };
  qptrdiff socket_descriptor_ { 0 };
  QString id_;

  const InitState init_state_;
  bool is_connected_          { false };
  inner::TcpSocket *socket_   { Q_NULLPTR };
};

//--------------------------ОПИСАНИЕ ВСТРАИВАЕМЫХ МЕТОДОВ-------------------------------
inline ProtocolType TcpClient::getProtocolType() const {
  return ProtocolType::TCP;
}
//--------------------------------------------------------------------------------------

} // namespace protocol
} // namespace netcom
