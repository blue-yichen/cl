#ifndef CHAT_SRC_NETWORKCLIENT_H_
#define CHAT_SRC_NETWORKCLIENT_H_
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include "Account.h"
#include "ChatWindow.h"

const int MaxWaitTime = 100000;

class NetworkClient : public QTcpSocket{
  Q_OBJECT
public:
  NetworkClient(const QString &serverAddress, int port);
  void connectToServer(const QHostAddress& serverAddress,int port);
  void sendMessage(const QString &message);
  static void receiveAllFriends(Account &account);
  void receiveNoReadMessage();
  void moveToMainThread();
  void connectSignals();
  bool waitMessage(int time);
  void reply();
signals:
  void messageReceived(SenderType type, const QString &senderAid, const QString &messageContent);
  void keyTalkingFinished();
protected slots:
  void onReadyRead();
  void onMessageSent(const QString &messageContent);
  void onKeyTalkingFinished();
private:
  void handleMessage(const QString &controlMessage);
  void handleFriend(const QString &controlMessage);
  void handleForwardMessage(const QString &controlMessage);
  void dealWithForwardMessage(ChatString &message);
  void dealWithForwardFile(ChatString &message);
};

#endif //CHAT_SRC_NETWORKCLIENT_H_
