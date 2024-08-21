#include <QVBoxLayout>
#include <QMessageBox>
#include "LoginWindow.h"
#include "NetworkClient.h"
#include "Data.h"


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
	QVBoxLayout *mainLayout = new QVBoxLayout();
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
	QHBoxLayout *passwordLayout = new QHBoxLayout();
	passwordLayout->addWidget(m_password);
	passwordLayout->addWidget(m_passwordEdit);
	passwordLayout->setSpacing(0);
	QHBoxLayout *accountLayout = new QHBoxLayout();
	accountLayout->addWidget(m_aid);
	accountLayout->addWidget(m_aidEdit);
	accountLayout->setSpacing(0);
	QVBoxLayout *inputLayout = new QVBoxLayout();
	inputLayout->addLayout(accountLayout);
	inputLayout->addLayout(passwordLayout);
	inputLayout->setSpacing(static_cast<int>(m_height * 0.05));
	m_loginButton = new QPushButton("登录");
	m_registerButton = new QPushButton("注册");
	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(m_registerButton);
	hLayout->addWidget(m_loginButton);
	updateWidgetSize();
	mainLayout->addWidget(m_caption,0,Qt::AlignCenter);
	mainLayout->addLayout(inputLayout,0);
	mainLayout->addLayout(hLayout);
	//连接信号和槽函数
	connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::login);
	connect(this, &LoginWindow::loginSuccessful, [this]
	() {
	  QMessageBox::information(this,"登录成功","欢迎回来," + user->getName());
	  this->hide();
	  //只有登录成功才能开始读取好友，否则读不到好友
	  chatWindow->readFriendList();
	  chatWindow->showFriendList();
	  socket->reply();
	  //通过socket读取未读消息
	  socket->receiveNoReadMessage();
	  chatWindow->show();
	  chatWindow->setWindowTitle(QString("聊天") + "(当前登录账号为" + user->getAid() +
	  ")");
	});
	connect(m_registerButton,&QPushButton::clicked,this,&LoginWindow::registers);
	connect(this, &LoginWindow::registerSuccessful, [this](){
	  QMessageBox::information(this,"消息","注册成功");
	  login();
	});
	connect(this,&LoginWindow::registerFail,[this]() {
	  QMessageBox::information(this,"提醒","注册失败,此aid已被注册");
	});
	connect(this,&LoginWindow::loginFail,[this]() {
	  QMessageBox::information(this,"提醒","登录失败，请检查账号和密码是否正确");
	});
}

LoginWindow::~LoginWindow()  = default;

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
	//生成控制信息
	FormatMessage formatMessage;
	QString controlMessage =
		ControlMessage::Mes[ControlMessage::Login] + ":"
		+ ControlMessage::Mes[ControlMessage::Null] + "\n";
	controlMessage += ControlMessage::Mes[ControlMessage::AidData] + ":"
		+ m_aidEdit->text() + "\n";
	controlMessage += ControlMessage::Mes[ControlMessage::PasswordData] + ":"
		+ m_passwordEdit->text() + "\n";
	qDebug() << controlMessage;
	formatMessage.addData(controlMessage);
	qDebug() << formatMessage.toQString();
	socket->sendMessage(formatMessage.toQString());
	//服务器断开连接
	if (!socket->waitMessage(5000)) {
	  QMessageBox::information(loginWindow,"提醒","登录请求超时，服务器端出现问题，请稍后重试");
	  qDebug() << "登录请求超时!\n";
	  return;
	}
  	qDebug() << "this is a message" + controlMessage;
	controlMessage = socket->readAll();
	qDebug() << "this is a replay" + controlMessage;
	ChatString str(controlMessage);
	if (str.getDataInBucket() ==
	ControlMessage::Mes[ControlMessage::LoginSuccessful]) {
	  //同步接收发送方的数据
	  socket->reply();
	  socket->waitMessage(MaxWaitTime);
	  str.skipNBucket(2);
	  //设置账号信息
	  user->setUser(str.getDataInBucket(),m_aidEdit->text(),m_passwordEdit->text());
	  NetworkClient::receiveAllFriends(*user);
	  emit loginSuccessful();
	} else {
	  emit loginFail();
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

  QString controlMessage;
  FormatMessage formatMessage;
  controlMessage = ControlMessage::Mes[ControlMessage::Register] + ":" + aid + ControlMessage::Mes[ControlMessage::EndFlag];
  controlMessage += ControlMessage::Mes[ControlMessage::UsernameData] + ":" + username + ControlMessage::Mes[ControlMessage::EndFlag];
  controlMessage += ControlMessage::Mes[ControlMessage::PasswordData] + ":" + password + ControlMessage::Mes[ControlMessage::EndFlag];
  //qDebug() << controlMessage;
  formatMessage.addData(controlMessage);
  //qDebug() << formatMessage.toQString();
  socket->sendMessage(formatMessage.toQString());
  //阻塞最多5秒等待消息
  if(!socket->waitMessage(5000)) {
	QMessageBox::information(loginWindow,"提醒","登录请求超时，服务器端出现问题，请稍后重试");
	qDebug() << "登录请求超时!\n";
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
