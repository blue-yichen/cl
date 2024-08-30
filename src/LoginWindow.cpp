#include <QVBoxLayout>
#include <QMessageBox>
#include "SHA256.h"
#include "LoginWindow.h"
#include "NetworkClient.h"
#include "Data.h"
#include <ctime>


LoginWindow::LoginWindow(QWidget *parent)
: MainWindow(parent) {
	//初始化m_height和m_width
	setMinimumSize(480,320);
	setMaximumSize(640,480);
	setSize(480,320);
	setWindowTitle("登录");
	//禁止窗口最大化
	setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	m_editSize.setSize(0.1,0.6);
	m_inputLabelSize.setSize(0.1,0.1);
	m_captionSize.setSize(0.1,1);
	m_loginButtonSize.setSize(0.15,0.35);
	//创建主布局
	auto *mainLayout = new QVBoxLayout();
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->setSpacing(static_cast<int>(m_height * 0.1));
	setLayout(mainLayout);
	//登录标签
	m_caption = new QLabel("登录Chat");
	labelFontStyle(m_caption, 20, Qt::AlignCenter);
	//账号密码输入Widget
	m_aid = new QLabel("账号");
	labelFontStyle(m_aid, 14, Qt::AlignCenter);
  m_aidEdit = new QLineEdit();
	m_password = new QLabel("密码");
	labelFontStyle(m_password, 14, Qt::AlignCenter);
	m_passwordEdit = new QLineEdit();
	m_passwordEdit->setEchoMode(QLineEdit::Password);
	auto *passwordLayout = new QHBoxLayout();
	passwordLayout->addWidget(m_password);
	passwordLayout->addWidget(m_passwordEdit);
	passwordLayout->setSpacing(0);
	auto *accountLayout = new QHBoxLayout();
	accountLayout->addWidget(m_aid);
	accountLayout->addWidget(m_aidEdit);
	accountLayout->setSpacing(0);
	auto *inputLayout = new QVBoxLayout();
	inputLayout->addLayout(accountLayout);
	inputLayout->addLayout(passwordLayout);
	inputLayout->setSpacing(static_cast<int>(m_height * 0.05));
	m_loginButton = new QPushButton("登录");
	m_registerButton = new QPushButton("注册");
	auto *hLayout = new QHBoxLayout();
	hLayout->addWidget(m_registerButton);
	hLayout->addWidget(m_loginButton);
	//updateWidgetSize();
	mainLayout->addWidget(m_caption,0,Qt::AlignCenter);
	mainLayout->addLayout(inputLayout,0);
	mainLayout->addLayout(hLayout);
	//连接信号和槽函数
	if (!connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::login)) {
        qDebug() << "connect  clicked signal of m_loginButton and login function fail";
    }
	if (!connect(this, &LoginWindow::loginSuccessful,this,&LoginWindow::onLoginSuccessful)) {

    }
	if (!connect(m_registerButton,&QPushButton::clicked,this,&LoginWindow::registers)) {

    }
	if(!connect(this, &LoginWindow::registerSuccessful, [this](){
	  QMessageBox::information(this,"消息","注册成功");
	  login();
	})) {

    }

	if (!connect(this,&LoginWindow::registerFail,[this]() {
	  QMessageBox::information(this,"提醒","注册失败,此aid已被注册");
	})) {

    }
	if(!connect(this,&LoginWindow::loginFail,this,&LoginWindow::onLoginFail)) {

    }
}

void LoginWindow::updateWidgetSize() {
	m_caption->setMinimumSize(static_cast<int>
							(m_width * m_captionSize.width),
	  						static_cast<int>
							  (m_height * m_captionSize.height));
	m_password->setMinimumSize(static_cast<int>
								(m_width * m_inputLabelSize.width),
								static_cast<int>
								(m_height * m_inputLabelSize.height));
	m_aid->setMinimumSize(static_cast<int>
							   (m_width * m_inputLabelSize.width),
						  static_cast<int>
							   (m_height * m_inputLabelSize.height));
	m_aidEdit->setMinimumSize(static_cast<int>
								   (m_width * m_editSize.width),
							  static_cast<int>
								   (m_height * m_editSize.height));

	m_passwordEdit->setMinimumSize(static_cast<int>
								   (m_width * m_editSize.width),
								   static_cast<int>
								   (m_height * m_editSize.height));
	m_loginButton->setMinimumSize(static_cast<int>
								  (m_width * m_loginButtonSize.width),
								  static_cast<int>
								  (m_height * m_loginButtonSize.height));
	m_registerButton->setMinimumSize(static_cast<int>
								  (m_width * m_loginButtonSize.width),
								  static_cast<int>
								  (m_height * m_loginButtonSize.height));
	m_caption->setMaximumSize(static_cast<int>
							  (m_width * m_captionSize.width),
							  static_cast<int>
							  (m_height * m_captionSize.height));
	m_password->setMaximumSize(static_cast<int>
							   (m_width * m_inputLabelSize.width),
							   static_cast<int>
							   (m_height * m_inputLabelSize.height));
	m_aid->setMaximumSize(static_cast<int>
							   (m_width * m_inputLabelSize.width),
						  static_cast<int>
							   (m_height * m_inputLabelSize.height));
	m_aidEdit->setMaximumSize(static_cast<int>
								   (m_width * m_editSize.width),
							  static_cast<int>
								   (m_height * m_editSize.height));

	m_passwordEdit->setMaximumSize(static_cast<int>
								   (m_width * m_editSize.width),
								   static_cast<int>
								   (m_height * m_editSize.height));
	m_loginButton->setMaximumSize(static_cast<int>
								  (m_width * m_loginButtonSize.width),
								  static_cast<int>
								  (m_height * m_loginButtonSize.height));
	m_registerButton->setMaximumSize(static_cast<int>
									 (m_width * m_loginButtonSize.width),
									 static_cast<int>
									 (m_height * m_loginButtonSize.height));
}
void LoginWindow::login() {
  	if (!checkPasswordAndAid()) {
		return;
	}
    //登录间隔限制
    static u64 prevTime;
    u64 curTime = std::time(nullptr);
    if (!LoginWindow::isEnoughTimeLimits(prevTime,curTime)) {
        QMessageBox::information(this,"提醒","登录过于频繁，请等待" +
        ChatString::NumberToStr(TimeLimits) + "秒后重试");
        return;
    }
    prevTime = curTime;
	//生成控制信息
	FormatMessage formatMessage;
	QString controlMessage =
		ControlMessage::Mes[ControlMessage::Login] + ":"
		+ ControlMessage::Mes[ControlMessage::Null] + "\n";
	controlMessage += ControlMessage::Mes[ControlMessage::AidData] + ":"
		+ m_aidEdit->text() + "\n";
	controlMessage +=
            ChatString::getALineFormatStr
            (ControlMessage::Mes
            [ControlMessage::PasswordData],
            SHA256::calculateSha256
            (m_passwordEdit->text().toUtf8()));
	qDebug() << controlMessage;
	formatMessage.addData(controlMessage);
	qDebug() << formatMessage.toQString();
	socket->sendImmediatelyMessage(formatMessage.toQString());
	//服务器断开连接
	if (!socket->waitMessage(5000)) {
	  qDebug() << "与服务器断开连接";
	  return;
	}
  	qDebug() << "this is a message" + controlMessage;
	controlMessage = socket->readAll();
	qDebug() << "this is a reply" + controlMessage;
	ChatString str(controlMessage);
	if (str.getDataInBucket() ==
	ControlMessage::Mes[ControlMessage::LoginSuccessful]) {
	  //同步接收发送方的数据
      qDebug() << "login successful";
	  socket->reply();
	  emit loginSuccessful(str);
	} else {
	  emit loginFail(str);
	}
}

void LoginWindow::registers() {
  if (!checkPasswordAndAid()) {
	return;
  }
  //模态对话框
  ALineInputDialog dialog("设置用户名","用户名");
  QMessageBox::information(&dialog,"注意","接下来将进入用户名输入阶段，按回车键即可确认输入");
  dialog.exec();
  QString username = dialog.getInput();
  QString aid = m_aidEdit->text();
  QString password = m_passwordEdit->text();
  if (username.isEmpty()) {
      QMessageBox::information(this,"提醒","用户名不能为空哦");
      return;
  }
  password = SHA256::calculateSha256(password.toUtf8());
  QString controlMessage;
  FormatMessage formatMessage;
  controlMessage = ControlMessage::Mes[ControlMessage::Register] + ":" + aid + ControlMessage::Mes[ControlMessage::EndFlag];
  controlMessage += ControlMessage::Mes[ControlMessage::UsernameData] + ":" + username + ControlMessage::Mes[ControlMessage::EndFlag];
  controlMessage += ControlMessage::Mes[ControlMessage::PasswordData] + ":" + password + ControlMessage::Mes[ControlMessage::EndFlag];
  //qDebug() << controlMessage;
  formatMessage.addData(controlMessage);
  //qDebug() << formatMessage.toQString();
  socket->sendImmediatelyMessage(formatMessage.toQString());
  //阻塞最多5秒等待消息
  if(!socket->waitMessage(5000)) {
//	QMessageBox::information(loginWindow,"提醒","登录请求超时，服务器端出现问题，请稍后重试");
	qDebug() << "与服务器断开连接";
	return;
  }
  controlMessage = socket->readAll();
  ChatString str(controlMessage);
  if (str.getDataInBucket() == ControlMessage::Mes[ControlMessage::RegisterSuccessful]) {
	emit registerSuccessful();
  } else {
	emit registerFail();
  }
}

void LoginWindow::labelFontStyle(QLabel* &label, int fontSize, QFlag alignment) {
	QFont font = label->font();
	font.setBold(true);
	font.setPointSize(fontSize);
	label->setFont(font);
	label->setAlignment(alignment);
}

void LoginWindow::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		login();
	}
}
bool LoginWindow::checkPasswordAndAid() {
  if (!Account::isQualifiedForPassword(m_passwordEdit->text())
	  || !Account::isQualifiedForAid(m_aidEdit->text())) {
	QMessageBox::information(this,"提醒",QString("账号或密码不符合要求") + "(密码至少" + ChatString::NumberToStr(PasswordMinlength) + "位字符，账号至少为" + ChatString::NumberToStr(AidMinLength) + "位数字)");
	return false;
  }
  return true;
}

bool LoginWindow::isEnoughTimeLimits(LoginWindow::u64 prevTime, LoginWindow::u64 curTime) {
    u64 timeDiff = curTime - prevTime;
    if (timeDiff < TimeLimits) {
        return false;
    }
    return true;
}

void LoginWindow::onLoginSuccessful(ChatString &message) {
    message.skipNBucket(2);
    //切换账号之前，将之前追加本地历史记录的槽和函数断开连接
    //如果上一个账号没有aid，则说明这是第一次登录
    //若不是第一次登录，则断开之前的信号槽连接
    if (!user->getAid().isEmpty()) {
        (*histories)[user->getAid()]->disconnect();
    }
    //设置账号信息
    user->setUser(message.getDataInBucket(),m_aidEdit->text(),
                  m_passwordEdit->text());
    NetworkClient::receiveAllFriends(*user);
    QMessageBox::information(this,"登录成功","欢迎回来," + user->getName());
    this->hide();
    //只有登录成功才能开始读取好友，否则读不到好友
    chatWindow->readFriendList();
    chatWindow->showFriendList();
    socket->reply();
    //接收服务器发送的好友关系并进行处理,只是用处理未读消息的方式处理好友关系。并没有使用存储未读消息的变量
    if (socket->waitMessage(MaxWaitTime)) {
        socket->receiveNoReadMessage();
    }
    socket->reply();
    //加载历史记录
    QString filePath = PreviousPath + user->getAid() +
                       + "/" + ChatHistoryDirName;
    //此账号未在本次程序中登录过，需要创建一个存储此账号历史记录的容器
    if (!(*histories).contains(user->getAid())) {
        (*histories)[user->getAid()] = new ChatHistory(filePath);
    } else { //此账号在本次程序中登录过，只需要恢复信号槽连接即可
        //只需要恢复信号槽连接
        (*histories)[user->getAid()]->connectSignals();
    }
    //加载聊天记录
    (*histories)[user->getAid()]->
            loadAllChatHistories();
    socket->reply();
    //等待服务器发送未读消息的处理信息
    if (socket->waitMessage(MaxWaitTime)) {
        socket->receiveNoReadMessage();
    }
    chatWindow->show();
    chatWindow->saveCurPos();
    chatWindow->setWindowTitle(QString("聊天") + "(当前登录账号为" + user->getAid() +")");
    tray->setVisible(true);
    //何时释放,通知thread可以启用了
    emit loginFinished();
}

void LoginWindow::onLoginFail(ChatString &message) {
    message.skipNBucket(1);
    QString cmd = message.getDataInBucket();
    if (cmd == ControlMessage::Mes[ControlMessage::RepeatLogin]) {
        QMessageBox::information(this,"提醒","账号已经登录");
    } else if (cmd == ControlMessage::Mes
    [ControlMessage::AccountOrPasswordError]) {
        QMessageBox::information(this,"提醒","账号或密码错误");
    }
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_BracketLeft ||
            keyEvent->key() == Qt::Key_BracketRight||
            keyEvent->key() == Qt::Key_Colon ||
            keyEvent->key() == Qt::Key_BraceLeft ||
            keyEvent->key() == Qt::Key_BraceRight ) {
            QMessageBox::information(this,"提醒","不能输入{}[]:");
            return true;
        }
    }
    return MainWindow::eventFilter(obj,event);
}
