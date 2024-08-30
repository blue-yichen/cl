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
  bool waitMessage(int time);
  void sendImmediatelyMessage(const QString &message);
  void reply();
signals:
  void connectServerError();
  void messageReceived(SenderType type, const QString &senderAid, const QString &messageContent);
  void keyTalkingFinished();
  void playMessageTone();
  void waitingAgreeAdded(const Friend &aFriend);
  void needingAgreeAdded(const Friend &aFriend);
  void waitingAgreeRemoved(const Friend &aFriend);
  void needingAgreeRemoved(const Friend &aFriend);
  void friendAddSccessful(const Friend &aFriend);
  void showMessageBox(const QString &title,const QString &message);
  void appendOutline(const QString &chatAid,
                       SenderType senderType,
                       const QString &messageOrFileName,
                       Type::Type messaegType,
                       const QString &timestamp);
protected slots:
  void onReadyRead();
  void onSendMessage(const QString &messageContent);
  void onKeyTalkingFinished();
public slots:
    void refusedFriendApply(const Friend &aFriend);
    void agreedFriendApply(const Friend &aFriend);
private:
  void handleMessage(const QString &controlMessage);
  void handleFriend(const QString &controlMessage);
  void handleForwardMessage(const QString &controlMessage);
  void dealWithForwardMessage(ChatString &message);
  void dealWithForwardFile(ChatString &message);
  void replyFriendApply(ControlMessage::ControlMessageEnum messageEnum,
                        const QString &aid);
};

#endif //CHAT_SRC_NETWORKCLIENT_H_
