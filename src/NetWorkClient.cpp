#include <QCoreApplication>
#include "ChatString.h"
#include "NetworkClient.h"
#include "ControlMessage.h"
#include "Data.h"
#include <QQueue>

void NetworkClient::connectToServer(const QHostAddress& serverAddress, int port) {
  connectToHost(serverAddress,port);
  connect(loginWindow,&LoginWindow::loginSuccessful,[this]() {
	//登录成功之后，此线程就开启，并且接管我的消息接收
	connect(this,&QTcpSocket::readyRead,this,&NetworkClient::onReadyRead);
	connect(chatWindow,&ChatWindow::messageSent,this,
			&NetworkClient::onMessageSent);
  });
  connect(this,&QTcpSocket::errorOccurred,[](){
	qDebug() << "connect error";
  });
  connect(this,&QTcpSocket::disconnected,[]() {
	qDebug() << "disconnect";
  });
  connect(this,&QTcpSocket::connected,[]() {
	qDebug() << "connect";
  });
  connect(this, &NetworkClient::messageReceived, chatWindow, &ChatWindow::addMessageOnBrowser);
}
void NetworkClient::onReadyRead() {
  //处理接收到的数据
  QString controlMessage = this->readAll();
  qDebug() << "receive messages:" + controlMessage;
  handleMessage(controlMessage);
}
void NetworkClient::sendMessage(const QString &message) {
	if (this->state() == QTcpSocket::ConnectedState) {
	  this->write(message.toUtf8());
	}
}

void NetworkClient::moveToMainThread() {
  // 获取主线程
  QThread* mainThread = QCoreApplication::instance()->thread();
  // 移动到主线程
  this->moveToThread(mainThread);
}

void NetworkClient::receiveAllFriends(Account &account) {
	//接收账号好友
	QString toReceiveMessage = socket->readAll();
  	qDebug() << "好友信息:" + toReceiveMessage;
	ChatString message(toReceiveMessage);
	QString aid;
	QString username;
	while (message.getDataInBucket() != ControlMessage::Mes[ControlMessage::EndMessage]) {
	  aid = message.getDataInBucket();
	  message.skipNBucket(1);
	  username = message.getDataInBucket();
	  account.addFriend(username,aid);
	}
}
bool NetworkClient::waitMessage(int time) {
  if (!this->waitForReadyRead(time)) {
	return false;
  }
  return true;
}
void NetworkClient::reply() {
  this->sendMessage("reply");
}
NetworkClient::NetworkClient(const QString &serverAddress, int port) {
  connectToServer(QHostAddress(serverAddress),port);
}
void NetworkClient::onMessageSent(const QString &messageContent) {
 	this->sendMessage(messageContent);
	qDebug() << "the message of to send:" + messageContent;
}
void NetworkClient::connectSignals() {

}
void NetworkClient::receiveNoReadMessage() {
  	//是否有未读消息
	//如果有，会一次性发送过来。
	QString noReadMessage = socket->readAll();
	ChatString message(noReadMessage);
	QString senderAid;
	QString receiveMessage;
	QString controlMessage;
	if (message.getDataInBucket() == ControlMessage::Mes[ControlMessage::NoReadMessage]) {
		return;
	}
	message.setData(noReadMessage);
	while (true) {
	  controlMessage = message.getDataInBucket();
	  if (controlMessage ==
	  ControlMessage::Mes
	  [ControlMessage::Forward]) {
		qDebug() << "this is start of a message";
		message.skipNBucket(1);
	  } else if (controlMessage ==
	  ControlMessage::Mes
	  [ControlMessage
	  ::ForwardMessage]) {
		receiveMessage = message.getDataInBucket();
	  } else if (controlMessage ==
	  ControlMessage::Mes
	  [ControlMessage::SendAid]) {
		senderAid = message.getDataInBucket();
		//接收了一条完整的消息
		emit messageReceived(Another,senderAid,receiveMessage);
	  } else {
		break;
	  }
	}
}

void NetworkClient::handleMessage(const QString &controlMessage) {
    ChatString message(controlMessage);
    QString command = message.getDataInBucket();
    if (command == ControlMessage::Mes[ControlMessage::Forward]) {
        qDebug() << "forward message";
        handleForwardMessage(controlMessage);
    } else if (command ==
                ControlMessage::Mes[ControlMessage::FriendFunc]) {
        handleFriend(controlMessage);
    }
}

void NetworkClient::dealWithForwardMessage(ChatString &message) {
    QString messageContent = message.getDataInBucket();
    message.skipNBucket(1);
    QString senderAid = message.getDataInBucket();
    emit messageReceived(Another,senderAid,messageContent);
}

void NetworkClient::dealWithForwardFile(ChatString &message) {

}

void NetworkClient::handleForwardMessage(const QString &controlMessage) {
    ChatString message(controlMessage);
    //得到第二条控制命令
    message.skipNBucket(2);
    if (message.getDataInBucket() == ControlMessage::Mes[ControlMessage::ForwardMessage]) {
        dealWithForwardMessage(message);
    } else {
        dealWithForwardFile(message);
    }
}

void NetworkClient::handleFriend(const QString &controlMessage) {
    ChatString message(controlMessage);
    QQueue<QString> messageQueue;
    message.skipNBucket(2);
    QString replyCommand = message.getDataInBucket();
    //关键通信时，要判断得到的回复是否为期望的回复。如果是其他消息。暂存到消息队列。等处理了真正的回复后再一一处理消息队列
    //主动方处理的消息
    if (replyCommand ==
        ControlMessage::Mes[ControlMessage::AidNoExist]) {

    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::BeAdding]) {

    } else if (replyCommand ==
               ControlMessage::Mes[ControlMessage::RepeatAdd]) {

    } else if (replyCommand ==
              ControlMessage::Mes[ControlMessage::RepeatApply]) {

    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::ToAddFriendBeRefuse]) {

    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::HostAddFriendSuccess]) {


    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::TheOtherAddsFriendToo]) {

    }
    //被动方处理的消息
    if (replyCommand ==
        ControlMessage::Mes[ControlMessage::FriendApply]) {

    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::PassiveAddFriendSuccess]) {

    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::BeFriendFail]) {

    }  else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::TheOtherAppliedToBeFriend
                ]) {


    }
}

void NetworkClient::onKeyTalkingFinished() {

}
