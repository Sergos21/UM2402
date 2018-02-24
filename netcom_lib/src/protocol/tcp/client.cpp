/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/tcp/client.cpp
*
* @brief: Файл исходных данных класса клиента реализующего протокол нижнего
* уровня Ethernet - TCP/IP
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "client.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QEventLoop>
#include <QtEndian>

using namespace netcom;
using namespace netcom::protocols;
//-------------------------------ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ----------------------------------
const int THREAD_WAIT_MSEC { 10000 };
//--------------------------------------------------------------------------------------
TcpClient::TcpClient(const QString &peer_ip,
                     int peer_port,
                     QObject *parent) :
  QThread(parent),
  peer_ip_(peer_ip),
  peer_port_(peer_port),
  init_state_(InitState_Default) {
}
//--------------------------------------------------------------------------------------
TcpClient::TcpClient(const QString &peer_ip,
                     int peer_port,
                     const QString &local_ip,
                     int local_port,
                     QObject *parent) :
  QThread(parent),
  peer_ip_(peer_ip),
  peer_port_(peer_port),
  local_ip_(local_ip),
  local_port_(local_port),
  init_state_(InitState_Default) {
}
//--------------------------------------------------------------------------------------
TcpClient::TcpClient(qptrdiff descriptor,
                     const QString &id,
                     QObject *parent) :
  QThread(parent),
  socket_descriptor_(descriptor),
  id_(id),
  init_state_(InitState_Descriptor) {
}
//--------------------------------------------------------------------------------------
TcpClient::~TcpClient() {
  quit();
  wait(THREAD_WAIT_MSEC);
}
//--------------------------------------------------------------------------------------
/// <summary> Открыть протокол </summary>
/// <returns> true/false </returns>
bool TcpClient::openProtocol() {
  if (isRunning()) {
    return false;
  }
  start();
  return true;
}
//--------------------------------------------------------------------------------------
/// <summary> Проверка на открытие протокола </summary>
/// <returns> true/false </returns>
bool TcpClient::isOpenProtocol() const {
  return is_connected_;
}
//--------------------------------------------------------------------------------------
/// <summary> Закрыть протокол </summary>
/// <returns> true/false </returns>
bool TcpClient::closeProtocol() {
  if (is_connected_) {
    quit();
    wait(THREAD_WAIT_MSEC);
    is_connected_ = false;
  }
  return true;
}
//--------------------------------------------------------------------------------------
/// <summary> Отправить сообщение </summary>
/// <param name="data"> Массив байт на запись </param>
/// <param name="id"> Идентификатор получателя (для tcp-клиента не актуален) </param>
/// <returns> Количество записанных байт </returns>
qint64 TcpClient::writeMessage(const QByteArray &data, const QString &id) {
  Q_UNUSED(id);
  if (is_connected_) {
    emit sigClientWriteMessage(data);
    return data.size();
  }
  return -1ll;
}
//--------------------------------------------------------------------------------------
/// <summary> Получить сообщение </summary>
/// <param name="data"> Массив байт на чтение </param>
/// <param name="id"> Идентификатор получателя (для tcp-клиента не актуален) </param>
/// <returns> Количество прочитанных байт </returns>
qint64 TcpClient::readMessage(QByteArray &data, QString &id) {
  Q_UNUSED(data)
  Q_UNUSED(id)
  return -1ll;
}
//--------------------------------------------------------------------------------------
/// <summary> Функция обработки потока клиента TCP/IP </summary>
/// <returns> </returns>
void TcpClient::run() {  
  socket_ = new inner::TcpSocket();
  // запись вызывается в том же потоке, в котором создан сокет
  connect(this, &TcpClient::sigClientWriteMessage, socket_, &inner::TcpSocket::slotWriteMessage);
  connect(socket_, &inner::TcpSocket::sigReadMessage, this, &TcpClient::sigClientReadMessage);
  connect(socket_, &inner::TcpSocket::connected, this, &TcpClient::slotOnClientConnected);
  connect(socket_, &inner::TcpSocket::disconnected, this, &TcpClient::slotOnClientDisconected);
  connect(socket_, &inner::TcpSocket::bytesWritten, this, &TcpClient::sigClientBytesWritten);
  //connect(socket_, &inner::TcpSocket::error, this, &TcpClient::slotOnSocketError);
  connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotOnSocketError(QAbstractSocket::SocketError)));

  if (init_state_ == InitState_Descriptor) {
    if (!socket_->setSocketDescriptor(socket_descriptor_)) {
      emit sigThreadInitialize(false);
      delete socket_;
      socket_ = Q_NULLPTR;
      return;
    }
    peer_ip_     = socket_->peerAddress().toString();
    peer_port_   = socket_->peerPort();
    local_ip_    = socket_->localAddress().toString();
    local_port_  = socket_->localPort();
  }
  else {
    //socket_->bind(QHostAddress(local_ip_), local_port_);
    socket_->connectToHost(peer_ip_, peer_port_, QIODevice::ReadWrite);
    socket_descriptor_ = socket_->socketDescriptor();
  }

  emit sigThreadInitialize(true);
  exec();

  // процедура завершения потока
  socket_->close();
  delete socket_;
  socket_ = Q_NULLPTR;
  if (init_state_ == InitState_Descriptor) {
    peer_ip_ = local_ip_ = QString::null;
    peer_port_ = local_port_ = -1;
  }
  else {
    socket_descriptor_ = -1;
  }
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки установления соединения с сервером </summary>
/// <returns> </returns>
void TcpClient::slotOnClientConnected() {
  if (isRunning()) {
    is_connected_ = true;
    emit sigClientConnected();
  }
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки отключения соединения с сервером </summary>
/// <returns> </returns>
void TcpClient::slotOnClientDisconected() {
  closeProtocol();
  emit sigClientDisconnected();
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки отключения соединения с сервером </summary>
/// <returns> </returns>
void TcpClient::slotOnSocketError(QAbstractSocket::SocketError error) {
  closeProtocol();
  emit sigSocketError(error);
}
//--------------------------------------------------------------------------------------

