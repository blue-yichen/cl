#include "ChatWindow.h"
#include <QSize>
#include <QDebug>
#include "Data.h"

ChatWindow::ChatWindow(QWidget *parent)
	: MainWindow(parent) {
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
	//m_messageBrowser->setFont();
	//m_messageBrowser->append("yichen:hello,world\n");
	chatLayout->addWidget(m_messageBrowser);
	//消息输入区
	m_sendButton = new QPushButton();
	m_selectButton = new QPushButton();
	m_textEdit = new QTextEdit();
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
	m_contactPerson = new ToolBar();
	m_contactPerson->setOrientation(Qt::Vertical);
	m_additionButton->setText("添加好友/聊天室");
	//读取聊天人列表
	readFriendList();
	updateFriendList();
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
	for (auto &it : m_friendButtonList) {
		//connect();
	}
	for (auto &it : m_friendActionList) {

	}
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

void ChatWindow::readFriendList() {
	int buttonHeight = static_cast<int>(m_friendSize.height * m_height);
	int buttonNumber = m_contactPerson->maxButtonNumber(buttonHeight);
	for (int i = 0;i < buttonNumber;++i) {
		m_friendButtonList.push_back(new QToolButton());
		m_friendButtonList[i]->setText(QString("好友").append(static_cast<char>('0' + i)));
	}
	for (int i = 0;i < 5;++i) {
		m_friendActionList.push_back(new QAction());
		m_friendActionList[i]->setText(QString("好友").append(static_cast<char>
		('0' + i)));
	}
}

void ChatWindow::updateFriendList() {
	for (const auto &it : m_friendButtonList) {
		m_contactPerson->addWidget(it);
	}
	for (const auto &it : m_friendActionList) {
		m_contactPerson->addAction(it);
	}
}

void ChatWindow::onSendButtonClicked() {

}
void ChatWindow::onSelectButtonClicked() {

}
void ChatWindow::showMessageOnBrowser() {

}
void ChatWindow::showFileOnBrowser() {

}
void ChatWindow::onAdditionButtonClicked() {

}
void ChatWindow::onFriendButtonClicked(const QString &id) {

}

