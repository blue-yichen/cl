//
// Created by yichen on 2024/8/17.
//

#include "NetWorkClient.h"
void NetWorkClient::connectToServer(const QHostAddress& serverAddress, int port) {
  connectToHost(serverAddress,port);
  connect(this,&QTcpSocket::readyRead,this,&NetWorkClient::onReadyRead);
  connect(this,&QTcpSocket::errorOccurred,[](){
	qDebug() << "connect error";
  });
}
void NetWorkClient::onReadyRead() {
  //处理接收到的数据
}
void NetWorkClient::sendMessage(const QString &message) {
	if (this->state() == QTcpSocket::ConnectedState) {
	  this->write(message.toUtf8());
	}
}
