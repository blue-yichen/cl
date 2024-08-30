#include <QCoreApplication>
#include <QQueue>
#include <unistd.h>
#include "Base32.h"
#include "ChatString.h"
#include "NetworkClient.h"
#include "ControlMessage.h"
#include "Data.h"
#include "Account.h"

void NetworkClient::connectToServer(const QHostAddress& serverAddress, int port) {
  connectToHost(serverAddress,port);
  connect(loginWindow,&LoginWindow::loginFinished,[this]() {
	//登录成功之后，此线程就开启，并且接管我的消息接收
	connect(this,&QTcpSocket::readyRead,this,&NetworkClient::onReadyRead);
	connect(chatWindow, &ChatWindow::sendMessage, this,
            &NetworkClient::onSendMessage);
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
  connect(this,&NetworkClient::needingAgreeAdded,chatWindow,
          &ChatWindow::onNeedingAgreeAdded);
  connect(this,&NetworkClient::waitingAgreeAdded,chatWindow,&ChatWindow::onWaitingAgreeAdded);
  connect(this,&NetworkClient::needingAgreeRemoved,chatWindow,
          &ChatWindow::onNeedingAgreeRemoved);
  connect(this,&NetworkClient::waitingAgreeRemoved,chatWindow,
          &ChatWindow::onWaitingAgreeRemoved);
  connect(this,&NetworkClient::friendAddSccessful,user,&Account::friendAdded);
  connect(this,&NetworkClient::showMessageBox,
          chatWindow,&ChatWindow::onShowMessage);
  connect(this,&NetworkClient::connectServerError,[]() {
      QMessageBox::information(nullptr,"提醒","与服务器的连接异常.......");
  });

}
void NetworkClient::onReadyRead() {
  //处理接收到的数据
  QString controlMessage = this->readAll();
  qDebug() << "receive messages:" + controlMessage;
  handleMessage(controlMessage);
}
void NetworkClient::sendMessage(const QString &message) {
	if (this->state() != QTcpSocket::ConnectedState) {
        emit showMessageBox("提醒", "与服务器断开连接......");
        //重连机制
    }
    this->write(message.toUtf8());
}

void NetworkClient::moveToMainThread() {
  // 获取主线程
  QThread* mainThread = QCoreApplication::instance()->thread();
  // 移动到主线程
  this->moveToThread(mainThread);
}

void NetworkClient::receiveAllFriends(Account &account) {
	//接收账号好友
    QString toReceiveMessage;
    if (socket->waitMessage(MaxWaitTime)) {
        toReceiveMessage = socket->readAll();
    } else {
        emit socket->connectServerError();
        return;
    }
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
  this->sendImmediatelyMessage("reply");
}
NetworkClient::NetworkClient(const QString &serverAddress, int port) {
  connectToServer(QHostAddress(serverAddress),port);
}
void NetworkClient::onSendMessage(const QString &messageContent) {
 	this->sendMessage(messageContent);
	qDebug() << "the message of to send:" + messageContent;
}

void NetworkClient::receiveNoReadMessage() {
  	//是否有未读消息
	//如果有，将一次性发送过来。
    qDebug() << "in receiveNoReadMessage Function";
    QString noReadMessage;
    noReadMessage = socket->readAll();
    qDebug() << "handler:";
    qDebug() << noReadMessage;
	ChatString message(noReadMessage);
	if (message.getDataInBucket() == ControlMessage::Mes[ControlMessage::NoReadMessage]) {
		return;
	}
    message.setData(noReadMessage);
    message.setBucket("{}");
    qDebug() << "receive no read message:" + message.toQString();
    QQueue<QString> messageQueue;
    //解析noReadMessage,将所有控制信息存入queue
    ChatString messageTmp(message.getDataAndBucket());
    qDebug() << messageTmp.toQString();
    messageQueue.enqueue(messageTmp.toQString());
    while (messageTmp.getDataInBucket() !=
    ControlMessage::Mes[ControlMessage::EndMessage])
    {
        messageTmp.setData(message.getDataAndBucket());
        qDebug() << "a line message:" + messageTmp.toQString();
        messageQueue.enqueue(messageTmp.toQString());
    }
    //最后一个是结束消息，不用处理
    while (messageQueue.size() > 1) {
        handleMessage(messageQueue.dequeue());
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
    message.skipNBucket(1);
    QString timestamp = message.getDataInBucket();
    //接收好友的消息，时间戳从消息中提取
    emit appendOutline(senderAid,SenderType::Another,
                       messageContent,Type::Text,timestamp);
    qDebug() << "before base32 decode:" + messageContent;
    messageContent = Base32::decode(messageContent);
    qDebug() << "after base32 decode:" + messageContent;
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
    QString aid;
    QString username;
    message.skipNBucket(2);
    QString replyCommand = message.getDataInBucket();
    qDebug() << QString("in") + __FUNCTION__;
    qDebug() << replyCommand;
    //主动方处理的消息
    if (replyCommand ==
        ControlMessage::Mes[ControlMessage::AidNoExist]) {
        emit showMessageBox("提醒","aid为" + message.getDataInBucket() +"的账号不存在");
    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::BeAdding]) {
       message.skipNBucket(2);
       aid = message.getDataInBucket();
       qDebug() << QString("in") + "be adding" + " aid " + aid;
       message.skipNBucket(1);
       username = message.getDataInBucket();
       qDebug() << "username:" + username;
       emit waitingAgreeAdded(Friend(aid,username));
    } else if (replyCommand ==
               ControlMessage::Mes[ControlMessage::RepeatAdd]) {
        aid = message.getDataInBucket();
        emit showMessageBox("提醒",aid + "已经是你的好友了");
    } else if (replyCommand ==
              ControlMessage::Mes[ControlMessage::RepeatApply]) {
        aid = message.getDataInBucket();
        emit showMessageBox("提醒","你已经申请过添加" + aid + "为好友了,请耐心等待对方回复");
    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::ToAddFriendBeRefuse]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();
        emit waitingAgreeRemoved(Friend(aid,username));
    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::HostAddFriendSuccess]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();
        userMutex.lock();
        user->addFriend(username,aid);
        userMutex.unlock();
        emit friendAddSccessful(Friend(aid,username));
        emit waitingAgreeRemoved(Friend(aid,username));
    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::TheOtherAddsFriendToo]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();

        userMutex.lock();
        user->addFriend(username,aid);
        userMutex.unlock();
        emit friendAddSccessful(Friend(aid,username));
        emit waitingAgreeRemoved(Friend(aid,username));
    } else if (replyCommand ==
    ControlMessage::Mes[ControlMessage::FriendIsSelf]) {
        emit showMessageBox("提醒","不能添加自己为好友哦");
    }
    //被动方处理的消息
    if (replyCommand ==
        ControlMessage::Mes[ControlMessage::FriendApply]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();
        emit needingAgreeAdded(Friend(aid,username));
    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::PassiveAddFriendSuccess]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();

        userMutex.lock();
        user->addFriend(username,aid);
        userMutex.unlock();
        emit friendAddSccessful(Friend(aid,username));
        emit needingAgreeRemoved(Friend(aid,username));
    } else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::BeFriendFail]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();
        emit needingAgreeRemoved(Friend(aid,username));
    }  else if (replyCommand ==
                ControlMessage::Mes[ControlMessage::TheOtherAppliedToBeFriend
                ]) {
        qDebug() << "this is friend message:" + message.toQString();
        message.skipNBucket(2);
        aid = message.getDataInBucket();
        message.skipNBucket(1);
        username = message.getDataInBucket();
        userMutex.lock();
        user->addFriend(username,aid);
        userMutex.unlock();
        emit needingAgreeRemoved(Friend(aid,username));
        emit friendAddSccessful(Friend(aid,username));
    }
}

void NetworkClient::onKeyTalkingFinished() {

}

void NetworkClient::refusedFriendApply(const Friend &aFriend) {
    replyFriendApply(ControlMessage::RefuseFriend,aFriend.m_aid);
}

void NetworkClient::agreedFriendApply(const Friend &aFriend) {
    replyFriendApply(ControlMessage::AgreeFriend,aFriend.m_aid);
}

void NetworkClient::replyFriendApply(ControlMessage::ControlMessageEnum messageEnum,const QString &aid) {
    QString forwardMessage = ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::FriendFunc],
             ControlMessage::Mes[ControlMessage::Null]);
    forwardMessage += ChatString::getALineFormatStr
            (ControlMessage::Mes[messageEnum],
             ControlMessage::Mes[ControlMessage::Null]);
    forwardMessage += ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::AnotherSideAid],
             aid);
    FormatMessage formatMessage(forwardMessage);
    sendMessage(formatMessage.toQString());
}

void NetworkClient::sendImmediatelyMessage(const QString &message) {
    if (this->state() == QTcpSocket::ConnectedState) {
        this->write(message.toUtf8());
        this->flush();
    } else {
        emit showMessageBox("提醒", "与服务器断开连接......");
    }
}
