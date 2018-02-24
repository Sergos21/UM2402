/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/tcp/server.cpp
*
* @brief: Файл исходных данных класса сервера реализующего протокол нижнего
* уровня Ethernet - TCP/IP
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "server.h"
#include "client.h"
//--------------------------------------------------------------------------------------
using namespace netcom;
using namespace netcom::protocols;
//--------------------------------------------------------------------------------------
TcpServer::TcpServer(const QString &ip,
                     int port,
                     int max_connections,
                     QObject *parent) :
  QTcpServer(parent),
  ip_(ip),
  port_(port),
  max_connections_(max_connections) {
}
//--------------------------------------------------------------------------------------
TcpServer::~TcpServer() {
  closeProtocol();
}
//--------------------------------------------------------------------------------------
/// <summary> Открыть протокол </summary>
/// <returns> true/false </returns>
bool TcpServer::openProtocol() {
  if (isOpenProtocol()) {
    closeProtocol();
  }
  setMaxPendingConnections(max_connections_);
  return listen(QHostAddress(ip_), port_);
}
//--------------------------------------------------------------------------------------
/// <summary> Проверка состояния протокола </summary>
/// <returns> true/false </returns>
bool TcpServer::isOpenProtocol() const {
  return isListening();
}
//--------------------------------------------------------------------------------------
/// <summary> Закрыть протокол </summary>
/// <returns> true/false </returns>
bool TcpServer::closeProtocol() {
  if (isOpenProtocol()) {
    close();
    qDeleteAll(clients_);
    clients_.clear();
  }
  return true;
}
//--------------------------------------------------------------------------------------
/// <summary> Отправить сообщение </summary>
/// <param name="data"> Массив байт для записи </param>
/// <param name="id"> Идентификатор получателя (если пустая строка, то отправляется всем
/// доступным клиентам) </param>
/// <returns> Количество записанных байт </returns>
qint64 TcpServer::writeMessage(const QByteArray &data, const QString &id) {
  if (!id.isEmpty()) {
    if (TcpClient *client = clients_.value(id, Q_NULLPTR)) {
      emit client->writeMessage(data);
    }
    else {
      return -1ll;
    }
  }
  else {
    Q_FOREACH (TcpClient *client, clients_) {
      emit client->writeMessage(data);
    }
  }
  return data.size();
}
//--------------------------------------------------------------------------------------
/// <summary> Получить сообщение </summary>
/// <param name="data"> Массив байт на чтение </param>
/// <param name="id"> Идентификатор получателя </param>
/// <returns> Количество прочитанных байт </returns>
qint64 TcpServer::readMessage(QByteArray &data, QString &id) {
  Q_UNUSED(data)
  Q_UNUSED(id)
  return -1ll;
}
//--------------------------------------------------------------------------------------
/// <summary> Получить удаленный адрес клиента </summary>
/// <param name="id"> Идентификатор клиента </param>
/// <returns> QString </returns>
QString TcpServer::getClientAddress(const QString &id) const {
  if (const TcpClient *client = clients_.value(id, Q_NULLPTR)) {
    return client->peer_ip_;
  }
  return QString();
}
//--------------------------------------------------------------------------------------
/// <summary> Получить удаленный порт клиента </summary>
/// <param name="id"> Идентификатор клиента </param>
/// <returns> int </returns>
int TcpServer::getClientPort(const QString &id) const {
  if (const TcpClient *client = clients_.value(id, Q_NULLPTR)) {
    return client->peer_port_;
  }
  return -1;
}
//--------------------------------------------------------------------------------------
/// <summary> Переназначить идентификатор клиента на другой </summary>
/// <param name="id"> Идентификатор клиента </param>
/// <param name="new_id"> Новый идентификатор для данного клиента </param>
/// <returns> true/false (возвращает 0 в случае, если такое имя уже есть в
/// списке внутренних имен у другого клиента) </returns>
bool TcpServer::assignClientID(const QString &id, const QString &new_id) {
  if (!clients_.contains(new_id)) {
    if (TcpClient *client = clients_.take(id)) {
      client->id_ = new_id;
      clients_.insert(new_id, client);
      return true;
    }
  }
  return false;
}
//--------------------------------------------------------------------------------------
/// <summary> Получить список всех имен клиентов на сервере </summary>
/// <returns> QStringList </returns>
QStringList TcpServer::getClientsID() const {
  return clients_.keys();
}
//--------------------------------------------------------------------------------------
/// <summary> Получить количество рабочих потоков клиентов </summary>
/// <returns> int </returns>
int TcpServer::getClientsCount() const {
  return clients_.size();
}
//--------------------------------------------------------------------------------------
/// <summary> Обработка входящего соединения на стороне сервера </summary>
/// <param name="descriptor"> Дескриптор подсоединившегося клиента </param>
/// <returns> </returns>
void TcpServer::incomingConnection(qptrdiff descriptor) {
  // создание идентификатора для вновь подсоединившегося клиента
  const QString id = QString("client_%1").arg(descriptor);
  TcpClient *client = new TcpClient(descriptor, id, this);
  clients_[id] = client;

  connect(client, &TcpClient::sigClientDisconnected, this, &TcpServer::slotOnClientDisconnected);
  connect(client, &TcpClient::sigClientReadMessage, this, &TcpServer::slotOnClientReadMessage);
  connect(client, &TcpClient::sigSocketError, this, &TcpServer::slotOnErrorSocket);
  client->openProtocol();
  emit sigClientConnected(id);
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки на стороне сервера при разъединении клиента </summary>
/// <returns> </returns>
void TcpServer::slotOnClientDisconnected() {
  if (TcpClient *client = qobject_cast<TcpClient*>(sender())) {
    const QString client_id = client->id_;
    delete client;
    clients_.remove(client_id);
    emit sigClientDisconnected(client_id);
  }
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки на стороне сервера при получении сообщения от клиента </summary>
/// <returns> </returns>
void TcpServer::slotOnClientReadMessage(const QByteArray &data) {
  if (const TcpClient *client = qobject_cast<TcpClient*>(sender())) {
    emit sigClientReadMessage(data, client->id_);
  }
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки на стороне сервера при получении сообщения от клиента </summary>
/// <returns> </returns>
void TcpServer::slotOnErrorSocket(QAbstractSocket::SocketError error) {
  if (const TcpClient *client = qobject_cast<TcpClient*>(sender())) {
    emit sigSocketError(error, client->id_);
  }
}
//--------------------------------------------------------------------------------------


