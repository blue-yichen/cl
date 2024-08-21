#include "ChatWindow.h"
#include <QSize>
#include <QDebug>
#include "Data.h"

ChatWindow::ChatWindow(QWidget *parent)
	: MainWindow(parent), m_currentChatFriend(NoChat) {
	//初始化m_height和m_width
	setMaximumSize(800,600);
	setMinimumSize(640,480);
	//禁止窗口最大化
	setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setSize(800,600);
	m_textEditSize.setSize(0.1, 0.55);
	m_sendButtonSize.setSize(0.1, 0.1);
	m_selectButtonSize = m_sendButtonSize;
	m_messageBrowserSize.setSize(0.85, 0.765);
	m_contactPersonSize.setSize(0.85, 0.2);
	m_additionButtonSize.setSize(0.1, 0.2);
	m_friendSize.setSize(m_contactPersonSize.height * 0.078,
						 m_contactPersonSize.width * 0.95);
	//创建主布局
	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);
	//消息显示区
	QVBoxLayout *chatLayout = new QVBoxLayout();
	m_messageBrowser = new QTextBrowser();
	//m_messageBrowser->setStyleSheet("p { line-height=1px; }");
	//m_messageBrowser->setFont();
	//m_messageBrowser->append("yichen:hello,world\n");
	chatLayout->addWidget(m_messageBrowser);
	//消息输入区
	m_sendButton = new QPushButton();
	m_selectButton = new QPushButton();
	m_textEdit = new TextEdit();
	QHBoxLayout *inputLayout = new QHBoxLayout();
	m_sendButton->setText("发送");
	m_selectButton->setText("选择文件");
	chatLayout->addLayout(inputLayout);
	inputLayout->addWidget(m_selectButton);
	inputLayout->addWidget(m_textEdit);
	inputLayout->addWidget(m_sendButton);
	//联系人选择区
	QVBoxLayout *contactLayout = new QVBoxLayout();
	m_additionButton = new QPushButton();
	m_contactPerson = new QToolBar();
	m_contactPerson->setOrientation(Qt::Vertical);
	m_additionButton->setText("添加好友/聊天室");
	contactLayout->addWidget(m_contactPerson);
	contactLayout->addWidget(m_additionButton);
	//更新控件大小
	updateWidgetSize();
	mainLayout->addLayout(contactLayout);
	mainLayout->addLayout(chatLayout);
	//连接信号和槽函数
	connect(m_sendButton,&QPushButton::clicked,this,&ChatWindow::onSendButtonClicked);
	connect(m_selectButton,&QPushButton::clicked,this,&ChatWindow::onSelectButtonClicked);
	connect(m_additionButton,&QPushButton::clicked,this,&ChatWindow::onAdditionButtonClicked);
	connect(this,&ChatWindow::messageSentByButton,this,&ChatWindow::addMessageOnBrowser);
	connect(m_textEdit,&TextEdit::returnPressed,m_sendButton,&QPushButton::click);
}

ChatWindow::~ChatWindow() = default;

void ChatWindow::updateWidgetSize() {
	m_textEdit->setMaximumSize(static_cast<int>
							(m_width * m_textEditSize.width), static_cast<int>
							(m_height * m_textEditSize.height));

	m_textEdit->setMinimumSize(static_cast<int>
							   (m_width * m_textEditSize.width), static_cast<int>
							   (m_height * m_textEditSize.height));
	m_sendButton->setMaximumSize(static_cast<int>
								(m_width * m_sendButtonSize.width), static_cast<int>
								(m_height * m_sendButtonSize.height));

	m_sendButton->setMinimumSize(static_cast<int>
								 (m_width * m_sendButtonSize.width), static_cast<int>
								 (m_height * m_sendButtonSize.height));
	m_selectButton->setMaximumSize(static_cast<int>
								(m_width * m_selectButtonSize.width), static_cast<int>
								(m_height * m_selectButtonSize.height));
	m_selectButton->setMinimumSize(static_cast<int>
								   (m_width * m_selectButtonSize.width), static_cast<int>
								   (m_height * m_selectButtonSize.height));
	m_messageBrowser->setMaximumSize(static_cast<int>
									(m_width * m_messageBrowserSize.width), static_cast<int>
									(m_height * m_messageBrowserSize.height));
	m_messageBrowser->setMinimumSize(static_cast<int>
									 (m_width * m_messageBrowserSize.width), static_cast<int>
									 (m_height * m_messageBrowserSize.height));
	m_additionButton->setMaximumSize(static_cast<int>
								   (m_width * m_additionButtonSize.width), static_cast<int>
									(m_height * m_additionButtonSize.height));
	m_additionButton->setMinimumSize(static_cast<int>
									 (m_width * m_additionButtonSize.width), static_cast<int>
									 (m_height * m_additionButtonSize.height));
	m_contactPerson->setMaximumSize(static_cast<int>
								  (m_width * m_contactPersonSize.width), static_cast<int>
									(m_height * m_contactPersonSize.height));
	m_contactPerson->setMinimumSize(static_cast<int>
									(m_width * m_contactPersonSize.width), static_cast<int>
									(m_height * m_contactPersonSize.height));
	for (auto &it : m_friendButtonList) {
		it->setMaximumSize(static_cast<int>
							(m_width * m_friendSize.width), static_cast<int>
							(m_height * m_friendSize.height));
		it->setMinimumSize(static_cast<int>
						   (m_width * m_friendSize.width), static_cast<int>
						   (m_height * m_friendSize.height));
	}
}

void ChatWindow::showFriendList() {
	for (const auto &it : m_friendButtonList) {
		m_contactPerson->addWidget(it);
	}
	for (const auto &it : m_friendActionList) {
		m_contactPerson->addAction(it);
	}
}

void ChatWindow::onSendButtonClicked() {
  	if (m_currentChatFriend == NoChat) {
		QMessageBox::information(this,"提醒","请在右侧好友列表中选择一个好友后再发送消息哦");
		return;
	  }
  	//得到之前显示的文本
	QString message = m_textEdit->toPlainText();
	qDebug() << message;
	if (message.isEmpty()) {
	  QMessageBox::information(this,"提醒","消息不能为空哦");
	  return;
	}
	FormatMessage formatToSendMessage;
	QString toSendMessage =
		ControlMessage::Mes[ControlMessage::Forward] +
		ControlMessage::Mes[ControlMessage::Delimiter] +
		ControlMessage::Mes[ControlMessage::Null] +
		ControlMessage::Mes[ControlMessage::EndFlag];
	toSendMessage +=
		ControlMessage::Mes[ControlMessage::ForwardMessage] +
		ControlMessage::Mes[ControlMessage::Delimiter] +
		message +
		ControlMessage::Mes[ControlMessage::EndFlag];
	toSendMessage +=
		ControlMessage::Mes[ControlMessage::ReceiverAid] +
		ControlMessage::Mes[ControlMessage::Delimiter] +
		m_friendChatAid[m_currentChatFriend] +
		ControlMessage::Mes[ControlMessage::EndFlag];
	formatToSendMessage.addData(toSendMessage);
	emit messageSent(formatToSendMessage.toQString());
	//socket->sendMessage(formatToSendMessage.toQString());
	emit messageSentByButton(MySelf,user->getAid(),message);
}
void ChatWindow::onSelectButtonClicked() {
  QFileDialog fileDialog(this);
  fileDialog.setMinimumSize(480, 320); // 设置最小大小
  fileDialog.setMaximumSize(640, 480); // 设置最大大小
  QString filePath = fileDialog.getOpenFileName(this,"选择文件","","All Files (*)");
  qDebug() << filePath;
}

void ChatWindow::addFileOnBrowser(const QString &fileType) {

}
void ChatWindow::onAdditionButtonClicked() {
	ALineInputDialog dialog("输入好友账号","账号");
	QString aid;
	int count = 0;
	do {
	  if (count != 0) {
		QMessageBox::information(this,"提醒","aid至少" + ChatString::NumberToStr
		(AidMinLength) + "位,且aid必须为数字");
	  }
	  dialog.exec();
	  aid = dialog.getInput();
	  count++;
	} while(!Account::isQualifiedForAid(aid));
	QMessageBox::information(this,"提醒","已向aid为" + aid + "的对象发送添加申请");
    QString forwardMessage =
            ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::FriendFunc],
             ControlMessage::Mes[ControlMessage::Null]);
    forwardMessage +=
            ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::AddFriend],
             ControlMessage::Mes[ControlMessage::Null]);
    forwardMessage +=
            ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::AnotherSideAid],
             aid);
    FormatMessage formatForwardMessage(forwardMessage);

    qDebug() << "add friend:" + aid;
    qDebug() << formatForwardMessage.toQString();
    //主动添加好友
    emit messageSent(formatForwardMessage.toQString());
}
void ChatWindow::onFriendButtonClicked(const QString &id) {

}
void ChatWindow::showNewFriendOnList(const QString &aid) {
  	//已经添加了新的好友在user中
	//user原本20个好友
	//这里就是21
	//而这里给出的是ChatId，一个索引
	QString showText = user->getNameFromFriend(aid) + "("
		+ aid + ")";
	if (isToolButton((int)user->getFriendsNumber() - 2)) {
		QToolButton *button = new QToolButton();
		button->setText(showText);
		button->resize(m_contactPerson->width(),
					   m_contactPerson->height() /
					   ToolButtonNumber);
		m_friendButtonList.emplace_back(button);
		m_contactPerson->addWidget(button);
	} else {
	  	QAction *button = new QAction();
	  	button->setText(showText);
		m_friendActionList.emplace_back(button);
		m_contactPerson->addAction(button);
	}
}
void ChatWindow::playVideoOnBrowser(const QUrl &url) {

}
void ChatWindow::friendButtonClicked(const QToolButton *button) {
  m_currentChatFriend = findWhichButton(button);
  qDebug() << QString("this is No.") + QChar(m_currentChatFriend + '0') + "button";
}
void ChatWindow::friendButtonClicked(const QAction *button) {
  m_currentChatFriend = findWhichButton(button);
  qDebug() << QString("this is No.") + QChar(m_currentChatFriend + '0') + "button";
}
int ChatWindow::findWhichButton(const QToolButton *button) const {
  for (int i = 0;i < m_friendButtonList.size();++i) {
	if (button == m_friendButtonList[i]) {
	  return i;
	}
  }
  return NoChat;
}
int ChatWindow::findWhichButton(const QAction *button) const {
  for (int i = 0;i < m_friendActionList.size();++i) {
	if (button == m_friendActionList[i]) {
	  return i + ToolButtonNumber;
	}
  }
  return NoChat;
}

void ChatWindow::addMessageOnBrowser(SenderType type,const QString &senderAid, const QString &messageContent) {
	QString message;
	QString showName;
	qDebug() << QString("this is in ") + __FUNCTION__;
	qDebug() << "show message：" + messageContent;
	if (senderAid != user->getAid()) {
	  showName = user->getNameFromFriend(senderAid);
	  showName += "(" + senderAid + ")";
	} else {
	  showName = "你自己";
	}
  	int chatId;
	if (type == Another) {
	  //判断好友是在哪类按钮上，QAction或QToolButton
	  chatId = m_friendChatId[senderAid];
	  if (isToolButton(chatId)) {
		//QAction
		QAction *friendButton = m_friendActionList[chatId - ToolButtonNumber];
	  } else {
		QToolButton *friendButton = m_friendButtonList[chatId];
	  }
	} else {
	  chatId = m_currentChatFriend;
	}
  	message = m_perFriendContents[chatId];
	message.append(QString("<style>"
						   "p { line-height: 1; }"
						   "</style>"
						   "<p><b>%1: </b> %2</p>").arg(showName,
														messageContent));
	qDebug() << "currentChatFriend:" + ChatString::NumberToStr
	(m_currentChatFriend);
	qDebug() << "Message Chat Id:" + ChatString::NumberToStr(chatId);
	m_perFriendContents[chatId] = message;
	if (m_currentChatFriend == chatId) {
	  m_messageBrowser->setHtml(message);
	  //自动向下滚动
	  m_messageBrowser->verticalScrollBar()->
	  setValue(m_messageBrowser->verticalScrollBar()->maximum());
	}
	m_textEdit->setText("");
}
void ChatWindow::updateBrowser() {
  m_messageBrowser->setHtml(m_perFriendContents[m_currentChatFriend]);
  m_messageBrowser->verticalScrollBar()->setValue(m_messageBrowser->verticalScrollBar()->maximum());
}
//如果是button返回false，action返回true
bool ChatWindow::isToolButton(int chatId) const {
  return chatId >= ToolButtonNumber;
}

void ChatWindow::readFriendList() {
 QList<QString> aids = user->getAllAidOfFriend();
 QList<QString> usernames = user->getAllUsernameOfFriend();
 QString showText;
 for (int i = 0;i < aids.size();++i) {
   showText = usernames[i] + "(" + aids[i] + ")";
   //建立双向映射
   m_friendChatId[aids[i]] = i;
   m_friendChatAid[i] = aids[i];
   if (!isToolButton(i)) {
	 //QToolButton
	 m_friendButtonList.emplace_back(new QToolButton(this));
	 m_friendButtonList[i]->setText(showText);
	 m_friendButtonList[i]->resize(m_contactPerson->width(),m_contactPerson->height() / ToolButtonNumber);
   } else {
	 //QAction
	 m_friendActionList.emplace_back(new QAction(this));
	 m_friendActionList[i - ToolButtonNumber]->setText(showText);
   }
   m_perFriendContents.push_back("");
 }
  for (auto &it : m_friendButtonList) {
	connect(it,&QToolButton::clicked,[=]() {
	  friendButtonClicked(it);
	  updateBrowser();
	  chatWindow->setWindowTitle("与" + it->text() + "的聊天");

	});
  }
  for (auto &it : m_friendActionList) {
	connect(it,&QAction::triggered,[=]() {
	  friendButtonClicked(it);
	  updateBrowser();
	  chatWindow->setWindowTitle("与" + it->text() + "的聊天");
	});
  }
}



