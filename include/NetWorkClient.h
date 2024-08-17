#ifndef CHAT_SRC_NETWORKCLIENT_H_
#define CHAT_SRC_NETWORKCLIENT_H_
#include <QTcpSocket>
#include <QHostAddress>

class NetWorkClient : public QTcpSocket{
public:
  NetWorkClient() = default;
  NetWorkClient(const QString &serverAddress,int port) {
	connectToServer(QHostAddress(serverAddress),port);
  }
  void connectToServer(const QHostAddress& serverAddress,int port);
  void sendMessage(const QString &message);
protected slots:
  void onReadyRead();
};

#endif //CHAT_SRC_NETWORKCLIENT_H_
