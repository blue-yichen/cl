#ifndef CHAT_SRC_WIDGET_H_
#define CHAT_SRC_WIDGET_H_
#include <QResizeEvent>
#include <QFileDialog>
#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVector>
#include <QToolBar>
#include <QUrl>
#include <QScrollBar>
#include "TextEdit.h"
#include "MainWindow.h"
#include "ALineInputDialog.h"
#include "Account.h"

const int NoChat = -1;
const int ToolButtonNumber = 10;

enum SenderType {
  MySelf,Another
};

class ChatWindow : public MainWindow {
  Q_OBJECT
public:
  ChatWindow(QWidget *parent = nullptr);
  ~ChatWindow();
  void showFriendList();
  void readFriendList();
  void showNewFriendOnList(const QString &aid);
signals:
  void messageSentByButton(SenderType type,const QString &senderAid,const QString &messageContent);
  void messageSent(const QString &messageContent);
public slots:
  void onSendButtonClicked();
  void onSelectButtonClicked();
  void onAdditionButtonClicked();
  void onFriendButtonClicked(const QString &id);
  //接收到消息就显示在textBrowser上
  void addMessageOnBrowser(SenderType type,const QString &senderAid, const QString &messageContent);
  void addFileOnBrowser(const QString &fileType);
  void playVideoOnBrowser(const QUrl &url);
  void friendButtonClicked(const QToolButton *button);
  void friendButtonClicked(const QAction *button);
private:
  	void updateWidgetSize() override;
	int findWhichButton(const QToolButton *button) const ;
	int findWhichButton(const QAction *button) const ;
	void updateBrowser();
	bool isToolButton(int chatId) const;
	int m_currentChatFriend;
	QVector<QToolButton*> m_friendButtonList;
	QVector<QAction*> m_friendActionList;
	//QToolButton在前，QAction在后
	QVector<QString> m_perFriendContents;
	//aid和按钮编号的映射
	QHash<QString,int> m_friendChatId;
	QHash<int,QString> m_friendChatAid;
  	QTextBrowser *m_messageBrowser;
	QPushButton *m_sendButton;
	QPushButton *m_selectButton;
	TextEdit *m_textEdit;
	QPushButton *m_additionButton;
	QToolBar *m_contactPerson;
	SizePercent m_messageBrowserSize;
	SizePercent m_sendButtonSize;
	SizePercent m_selectButtonSize;
	SizePercent m_textEditSize;
	SizePercent m_additionButtonSize;
	SizePercent m_contactPersonSize;
	SizePercent m_friendSize;
};

#endif //CHAT_SRC_WIDGET_H_
