/*! ***************************************************************************
* Проект: src.pro
*
* @file protocol/tcp/socket.cpp
*
* @brief: Файл исходных данных вспомогательного класса для работы с протоколом
* нижнего уровня Ethernet - TCP/IP
*
* @note
*
* @author Лола С.А.
******************************************************************************/
#include "socket.h"

#include <QtNetwork/QHostAddress>
#include <QtEndian>
//--------------------------------------------------------------------------------------
using namespace netcom;
using namespace netcom::protocols;
using namespace netcom::protocols::inner;
//-------------------------------ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ----------------------------------
const int SOCKET_WRITE_WAIT_MSEC { 1000 };
const int SOCKET_READ_WAIT_MSEC  { 1000 };
const int PACKET_LEN_SIZE        { 4 };
//--------------------------------------------------------------------------------------
TcpSocket::TcpSocket(QObject *parent) :
  QTcpSocket(parent) {
  // регистрация внутреннего идентификатора
  qRegisterMetaType<qptrdiff>("qptrdiff");
  // регистрация QAbstractSocket::SocketError
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

  connect(this, &TcpSocket::readyRead, this, &TcpSocket::slotReadMessage);
}
//--------------------------------------------------------------------------------------
/// <summary> Отправить сообщение </summary>
/// <param name="data"> Массив байт на запись </param>
/// <returns></returns>
void TcpSocket::slotWriteMessage(const QByteArray &data) {
  static QByteArray packet_len_data(PACKET_LEN_SIZE, Qt::Uninitialized);
  // записываем длину передаваемых данных
  qToBigEndian<qint32>(data.size(), reinterpret_cast<uchar*>(packet_len_data.data()));
  const QByteArray full_packet = packet_len_data + data;
  write(full_packet);
  waitForBytesWritten(SOCKET_WRITE_WAIT_MSEC);
}
//--------------------------------------------------------------------------------------
/// <summary> Слот обработки полученного сообщения </summary>
/// <returns> </returns>
void TcpSocket::slotReadMessage() {
  while (bytesAvailable() > PACKET_LEN_SIZE) {
    const QByteArray len_data = read(PACKET_LEN_SIZE);
    const qint32 actual_len = qFromBigEndian<qint32>(
      reinterpret_cast<const quint8*>(len_data.constData()));
    QByteArray buff;
    qint32 fragment_size = 0;
    // принудительно дособираем пакет, в случае если он ожидается большого размера
    while (buff.size() < actual_len) {
      fragment_size = bytesAvailable();
      if ((buff.size() + fragment_size) > actual_len) {
        fragment_size = actual_len - buff.size();
      }
      if (fragment_size == 0) {
        // принудительно дожидаемся прихода полного пакета
        if (!waitForReadyRead(SOCKET_READ_WAIT_MSEC)) {
          buff.clear();
          break;
        }
      }
      else {
        buff += read(fragment_size);
      }
    }

    if (!buff.isEmpty())
      emit sigReadMessage(buff);
  }
}
//--------------------------------------------------------------------------------------
