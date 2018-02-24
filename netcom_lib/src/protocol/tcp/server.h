/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/tcp/server.h
*
* @brief: Заголовочный файл класса сервера реализующего протокол нижнего уровня
* Ethernet - TCP/IP
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#pragma once

#include <QtNetwork/QTcpServer>

#include "src/protocol/abstract_protocol.h"

namespace netcom {
namespace protocols {

class TcpClient;

class NETCOM_SHARED_EXPORT TcpServer : public QTcpServer, public AbstractProtocol {
  Q_OBJECT
public:
  explicit TcpServer(const QString &ip,
                     int port,
                     int max_connections,
                     QObject *parent = Q_NULLPTR);
  virtual ~TcpServer();

  virtual ProtocolType getProtocolType() const Q_DECL_OVERRIDE;

  virtual bool openProtocol() Q_DECL_OVERRIDE;
  virtual bool isOpenProtocol() const Q_DECL_OVERRIDE;
  virtual bool closeProtocol() Q_DECL_OVERRIDE;

  virtual qint64 writeMessage(const QByteArray &data, const QString &id = NULL_ID) Q_DECL_OVERRIDE;
  virtual qint64 readMessage(QByteArray &data, QString &id = NULL_ID) Q_DECL_OVERRIDE;

  QString getClientAddress(const QString &id) const;
  int getClientPort(const QString &id) const;

  bool assignClientID(const QString &id, const QString &new_id);
  QStringList getClientsID() const;
  int getClientsCount() const;

signals:
  void sigClientConnected(const QString &id);
  void sigClientDisconnected(const QString &id);
  void sigClientReadMessage(const QByteArray &data, const QString &id);
  void sigSocketError(QAbstractSocket::SocketError error, const QString &id);

protected:
  void incomingConnection(qptrdiff descriptor) Q_DECL_OVERRIDE;

private slots:
  void slotOnClientDisconnected();
  void slotOnClientReadMessage(const QByteArray &data);
  void slotOnErrorSocket(QAbstractSocket::SocketError error);

private:
  const QString ip_;
  const int port_             { -1 };
  const int max_connections_  { 0 };

  QHash<QString, TcpClient*> clients_;
};

//--------------------------ОПИСАНИЕ ВСТРАИВАЕМЫХ МЕТОДОВ-------------------------------
inline ProtocolType TcpServer::getProtocolType() const {
  return ProtocolType::TCP;
}
//--------------------------------------------------------------------------------------

} // namespace protocols
} // namespace netcom

