#include "ChatWindow.h"
#include "ScrollApplyWindow.h"
#include "Base32.h"
#include "ChatHistory.h"
#include <QSize>
#include <QMenu>
#include "Data.h"

ChatWindow::ChatWindow(QWidget *parent)
	: MainWindow(parent), m_currentChatFriendId(NoChat),
      m_isQuit(false), m_isStartMessageTone(false) {
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
    //好友浮窗
    m_waitAgreeButton = new FloatingWindow("主");
    m_needAgreeButton = new FloatingWindow("被");
    m_waitAgreeList = new ScrollApplyWindow(LabelLine,m_waitAgreeButton);
    m_needAgreeList = new ScrollApplyWindow(ButtonLine,m_needAgreeButton);
    //消息提示音
    m_messageTone = new QSoundEffect(this);
    m_messageTone->setSource(QUrl::fromLocalFile("."
                                                 ""
                                                 "./source/sounds/MessageTone"
                                                 ".wav"));
    //播放一次
    m_messageTone->setLoopCount(1);
    m_messageTone->setVolume(0.5f);
	mainLayout->addLayout(contactLayout);
	mainLayout->addLayout(chatLayout);
	//连接信号和槽函数
	connect(m_sendButton,&QPushButton::clicked,this,&ChatWindow::onSendButtonClicked);
	connect(m_selectButton,&QPushButton::clicked,this,&ChatWindow::onSelectButtonClicked);
	connect(m_additionButton,&QPushButton::clicked,this,&ChatWindow::onAdditionButtonClicked);
	connect(this, &ChatWindow::showMessageOnBrowser, this, &ChatWindow::addMessageOnBrowser);
	connect(m_textEdit,&TextEdit::returnPressed,m_sendButton,&QPushButton::click);
    connect(user,&Account::friendAdded,this,&ChatWindow::onFriendAdded);
    connect(m_waitAgreeButton,&FloatingWindow::clicked,[this]() {
       m_waitAgreeList->show();
       m_waitAgreeButton->hide();
    });
    connect(m_needAgreeButton,&FloatingWindow::clicked,[this]() {
        m_needAgreeList->show();
        m_needAgreeButton->hide();
    });
    connect(loginWindow,&LoginWindow::loginSuccessful,[this]() {
        connect(m_needAgreeList,&ScrollApplyWindow::agreeButtonClicked,socket,&NetworkClient::agreedFriendApply);
        connect(m_needAgreeList,&ScrollApplyWindow::refuseButtonClicked,socket,&NetworkClient::refusedFriendApply);
    });
    //m_textEdit->installEventFilter(this);
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
  	if (m_currentChatFriendId == NoChat) {
		QMessageBox::information(this,"提醒","请在右侧好友列表中选择一个好友后再发送消息哦");
		return;
	  }
  	//得到之前显示的文本
	QString message = m_textEdit->toPlainText();
	qDebug() << "before base32 encode: " + message;
    message = Base32::encode(message.toUtf8());
    qDebug() << "after base32 encode: " + message;
	if (message.isEmpty()) {
	  QMessageBox::information(this,"提醒","消息不能为空哦");
	  return;
	}
    if (!checkMagicMessage(message)) {
        handlerNormalMessage(message);
    } else {
        handlerMagicMessage(Base32::decode(message));
    }
}
void ChatWindow::onSelectButtonClicked() {
  QFileDialog fileDialog(this);
  fileDialog.setMinimumSize(480, 320); // 设置最小大小
  fileDialog.setMaximumSize(640, 480); // 设置最大大小
  QString filePath = fileDialog.getOpenFileName(this,"选择文件","","All Files (*)");
  qDebug() << filePath;
  //为生成文件uuid
}

void ChatWindow::addFileOnBrowser(const QString &fileType) {

}
void ChatWindow::onAdditionButtonClicked() {
	ALineInputDialog dialog("输入好友账号","账号");
	QString aid;
	int count = 0;
    int ret;
	do {
	  if (count != 0) {
		QMessageBox::information(this,"提醒","aid至少" + ChatString::NumberToStr
		(AidMinLength) + "位,且aid必须为数字");
	  }
	  ret = dialog.exec();
      if (ret == QDialog::Rejected) {
          return;
      }
	  aid = dialog.getInput();
	  count++;
	} while(!Account::isQualifiedForAid(aid));
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
    emit sendMessage(formatForwardMessage.toQString());
}

//void ChatWindow::showNewFriendOnList(const QString &aid) {
//  	//已经添加了新的好友在user中
//	//user原本20个好友
//	//这里就是21
//	//而这里给出的是ChatId，一个索引
//	QString showText = user->getNameFromFriend(aid) + "("
//		+ aid + ")";
//	if (isToolButton((int)user->getFriendsNumber() - 2)) {
//		QToolButton *button = new QToolButton();
//		button->setText(showText);
//		button->resize(m_contactPerson->width(),
//					   m_contactPerson->height() /
//					   ToolButtonNumber);
//		m_friendButtonList.emplace_back(button);
//		m_contactPerson->addWidget(button);
//	} else {
//	  	QAction *button = new QAction();
//	  	button->setText(showText);
//		m_friendActionList.emplace_back(button);
//		m_contactPerson->addAction(button);
//	}
//}
void ChatWindow::playVideoOnBrowser(const QUrl &url) {

}
void ChatWindow::friendButtonClicked(const QToolButton *button) {
    m_currentChatFriendId = findWhichButton(button);
  qDebug() << QString("this is No.") + QChar(m_currentChatFriendId + '0') + "button";
}
void ChatWindow::friendButtonClicked(const QAction *button) {
    m_currentChatFriendId = findWhichButton(button);
  qDebug() << QString("this is No.") + QChar(m_currentChatFriendId + '0') + "button";
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

void ChatWindow::addMessageOnBrowser(SenderType type,const QString
&senderAid,const QString &messageContent) {
	QString message;
	QString showName;
	qDebug() << QString("this is in ") + __FUNCTION__;
	qDebug() << "show message：" + messageContent;
	if (senderAid != user->getAid()) {
	  showName = user->getNameFromFriend(senderAid);
	  showName += "(" + senderAid + ")";
	} else {
	  showName = "我";
	}
  	int chatId;
    //其他人的消息显示在browser上
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
      //自己发送的消息
      chatId = m_currentChatFriendId;
	}
    //响起消息提示音
    //响铃的条件
    //1.消息来源不是当前聊天窗口
    //2.不是加载聊天记录
    if (m_isStartMessageTone && m_currentChatFriendId != chatId) {
        m_messageTone->play();
    }
  	message = m_perFriendContents[chatId];
	message.append(QString("<style>"
						   "p { line-height: 1; }"
						   "</style>"
						   "<p><b>%1: </b> %2</p>").arg(showName,messageContent));
	m_perFriendContents[chatId] = message;
    //当前聊天窗口的消息
	if (m_currentChatFriendId == chatId) {
	  m_messageBrowser->setHtml(message);
	  //自动向下滚动
	  m_messageBrowser->verticalScrollBar()->
	  setValue(m_messageBrowser->verticalScrollBar()->maximum());
    }
}
void ChatWindow::updateBrowser() {
  m_messageBrowser->setHtml(m_perFriendContents[m_currentChatFriendId]);
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
 QString chatTitle;
 for (int i = 0;i < aids.size();++i) {
   showText = usernames[i] + "(" + aids[i] + ")";
   chatTitle = "与" + showText + "的聊天";
   //建立双向映射
   m_friendChatId[aids[i]] = i;
   m_friendChatAid[i] = aids[i];
   if (!isToolButton(i)) {
	 //QToolButton
	 m_friendButtonList.emplace_back(new QToolButton(this));
	 m_friendButtonList[i]->setText(showText);
//	 m_friendButtonList[i]->resize(m_contactPerson->width(),
//                              m_contactPerson->height() / ToolButtonNumber);
     m_friendButtonList[i]->setFixedSize(m_contactPerson->width() - 10,
                            m_contactPerson->height() /
                            ToolButtonNumber);
     qDebug() << "friend height:" + ChatString::NumberToStr
     (m_contactPerson->height() / ToolButtonNumber);
   } else {
	 //QAction
	 m_friendActionList.emplace_back(new QAction(this));
	 m_friendActionList[i - ToolButtonNumber]->setText(showText);
   }
   m_perFriendContents.emplace_back("");
 }
  for (auto &it : m_friendButtonList) {
	connect(it,&QToolButton::clicked,[=]() {
	  friendButtonClicked(it);
	  updateBrowser();
	  chatWindow->setWindowTitle(chatTitle);
	});
  }
  for (auto &it : m_friendActionList) {
	connect(it,&QAction::triggered,[=]() {
	  friendButtonClicked(it);
	  updateBrowser();
	  chatWindow->setWindowTitle(chatTitle);
	});
  }
}

//添加好友
void ChatWindow::onFriendAdded(const Friend &aFriend) {
    //在好友列表中扩展
    //判断是action还是ToolButton
    //如果是同一个好友，则不处理
    QString showName = aFriend.m_username +
            "(" + aFriend.m_aid + ")";
    QString chatTitle = "与" + showName + "的聊天";
    if (isSameFriend(showName)) {
        return;
    }
    if (m_friendButtonList.size() < ToolButtonNumber) {
        //ToolButton
        QToolButton *button = new QToolButton(this);
        button->setText(showName);
        button->setFixedSize(m_contactPerson->width() - 8,
                             m_contactPerson->height() /
                                     (ToolButtonNumber + 3) + 5);
//        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        //连接信号槽
        connect(button,&QToolButton::clicked,[=]() {
            friendButtonClicked(button);
            updateBrowser();
            chatWindow->setWindowTitle(chatTitle);
        });
        m_friendButtonList.emplace_back(button);
        m_contactPerson->addWidget(button);

        qDebug() << "friend height:" + ChatString::NumberToStr
                (m_contactPerson->height() / ToolButtonNumber);
    } else {
        //action
        QAction *button = new QAction();
        button->setText(showName);
        //连接信号槽
        connect(button,&QAction::triggered,[=]() {
            friendButtonClicked(button);
            updateBrowser();
            chatWindow->setWindowTitle(chatTitle);
        });
        m_friendActionList.emplace_back(button);
        m_contactPerson->addAction(button);
    }
    //得到对应的chatId
    //和对应的信息框内容
    int chatId = m_friendChatAid.size();
    m_friendChatAid[chatId] = aFriend.m_aid;
    m_friendChatId[aFriend.m_aid] = chatId;
    m_perFriendContents.emplace_back("");
    qDebug() << "add chatId:" + ChatString::NumberToStr(chatId);
}


void ChatWindow::onWaitingAgreeAdded(const Friend &aFriend) {
    qDebug() << QString("in ") + __FUNCTION__;
    //有一个申请就显示按钮
    waitMutex.lock();
    if (waitToAgreeTable.empty()) {
        m_waitAgreeButton->show();
    }
    waitToAgreeTable.insert(aFriend);
    m_waitAgreeList->addRaw(aFriend.m_username + "(" +
                            aFriend.m_aid + ")");
    waitMutex.unlock();

}

void ChatWindow::onNeedingAgreeAdded(const Friend &aFriend) {
    qDebug() << QString("in ") + __FUNCTION__;
    needMutex.lock();
    if (needToAgreeTable.empty()) {
        m_needAgreeButton->show();
    }
    needToAgreeTable.insert(aFriend);
    m_needAgreeList->addRaw(aFriend.m_username + "(" +
                            aFriend.m_aid + ")");
    needMutex.unlock();
}

void ChatWindow::onWaitingAgreeRemoved(const Friend &aFriend) {
    qDebug() << QString("in ") + __FUNCTION__;
    waitMutex.lock();
    waitToAgreeTable.remove(aFriend);
    m_waitAgreeList->removeRaw(aFriend.m_username + "(" +
                               aFriend.m_aid + ")");
    if (waitToAgreeTable.empty()) {
        m_waitAgreeButton->hide();
        m_waitAgreeList->hide();
    }
    waitMutex.unlock();
}

void ChatWindow::onNeedingAgreeRemoved(const Friend &aFriend) {
    qDebug() << QString("in ") + __FUNCTION__;

    needMutex.lock();
    needToAgreeTable.remove(aFriend);
    m_needAgreeList->removeRaw(aFriend.m_username + "(" +
                               aFriend.m_aid + ")");
    if (needToAgreeTable.empty()) {
        m_needAgreeButton->hide();
        m_needAgreeList->hide();
    }
    needMutex.unlock();
}

void ChatWindow::onShowMessage(const QString &title, const QString &message) {
    QMessageBox::information(chatWindow,title,message);
}

void ChatWindow::closeEvent(QCloseEvent *event) {
    if (!m_isQuit) {
        this->hide();
        event->ignore();
    } else {
        MainWindow::closeEvent(event);
    }
    m_waitAgreeList->hide();
    m_waitAgreeButton->hide();
    m_needAgreeButton->hide();
    m_needAgreeList->hide();
}



void ChatWindow::quit() {
    m_isQuit = true;
}

void ChatWindow::showWaitButton() {
    if (m_waitAgreeButton) {
        m_waitAgreeButton->showAtPrevPos();
    }
}

void ChatWindow::showNeedButton() {
    if (m_needAgreeButton) {
        m_needAgreeButton->showAtPrevPos();
    }
}

void ChatWindow::showAtPrevPos() {
    this->show();
    this->move(m_savePos);
}

void ChatWindow::saveCurPos() {
    m_savePos = this->pos();
}

void ChatWindow::onLoadedHistory() {
    //从history中读取消息并加载到每个好友的browser缓冲区
    ChatHistory *history = (*histories)[user->getAid()];
    QList<QString> aidsOfTheFriend =
            user->getAllAidOfFriend();
    for (const auto &aid : aidsOfTheFriend) {
        qDebug() << "load " + aid + " history on browser";
        //加载一个好友的聊天信息到browser
        loadAFriendHistoryToBrowser(history,aid);
    }
    m_isStartMessageTone = true;
}

void ChatWindow::loadAFriendHistoryToBrowser(
        void *historyTemp,const QString &aid) {
    ChatHistory *history =
            static_cast<ChatHistory*>(historyTemp);
    QList<ChatOutline> outlines =
            history->getAFriendHistory(aid);
    QString message;
    SenderType type;
    qDebug() << outlines.isEmpty();
    //要将当前聊天框设置为这个好友的chatId，因为如果是自己发送的消息。默认就是在跟当前chatid聊天
    m_currentChatFriendId = m_friendChatId[aid];
    qDebug() << "chatId :" + ChatString::NumberToStr(m_currentChatFriendId);
    for (const auto &outline : outlines) {
        //处理每一条信息
        if (outline.m_aid == user->getAid()) {
            type = SenderType::MySelf;
        } else {
            type = SenderType::Another;
        }
        //文本
        qDebug() << "outline type : " + outline.m_type;
        qDebug() << "outline content: " + outline.m_content;
        if (outline.m_type == Type::type[Type::Text]) {
            emit showMessageOnBrowser(type,outline.m_aid,outline.m_content);
        } else { //文件

        }
        //完成一条消息的处理
    }
}

bool ChatWindow::checkMagicMessage(const QString &message) {
    return false;
}

void ChatWindow::handlerMagicMessage(const QString &message) {

}

void ChatWindow::handlerNormalMessage(const QString &message) {
    FormatMessage formatToSendMessage;
    QString toSendMessage =
            ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::Forward],
             ControlMessage::Mes[ControlMessage::Null]);
    toSendMessage +=
            ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::ForwardMessage],
             message);
    toSendMessage +=
            ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::ReceiverAid],
             m_friendChatAid[m_currentChatFriendId]);
    QString timestamp = QDateTime::currentDateTime().toString();
    //发送消息时，带上时间戳
    toSendMessage += ChatString::getALineFormatStr
            (ControlMessage::Mes[ControlMessage::Timestamp],
             timestamp);
    formatToSendMessage.addData(toSendMessage);
    qDebug() << "send message:" + formatToSendMessage.toQString();
    m_textEdit->setText("");
    emit sendMessage(formatToSendMessage.toQString());
    emit showMessageOnBrowser(MySelf, user->getAid(), Base32::decode(message));
    //时间戳由客户端发送时加上
    emit appendOutline(m_friendChatAid[m_currentChatFriendId],
                       SenderType::MySelf,message,
                       Type::Text,
                       timestamp);
}

void ChatWindow::readFriendsRelation() {
    QString friends;
    if (socket->waitMessage(MaxWaitTime)) {
        friends = socket->readAll();
    }
    if (friends == ControlMessage::Mes[ControlMessage::NoFriendRelation]) {
        qDebug() << ControlMessage::Mes[ControlMessage::NoFriendRelation];
    }
}

bool ChatWindow::isSameFriend(const QString &label) {
    //遍历两个好友表的label
    for (const auto &it : m_friendButtonList) {
        if (it->text() == label) {
            return true;
        }
    }
    for (const auto &it : m_friendActionList) {
        if (it->text() == label) {
            return true;
        }
    }
    return false;
}
