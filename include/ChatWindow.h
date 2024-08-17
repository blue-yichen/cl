#ifndef CHAT_SRC_WIDGET_H_
#define CHAT_SRC_WIDGET_H_
#include <QResizeEvent>
#include <QTextEdit>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVector>
#include "ToolBar.h"
#include "MainWindow.h"

class ChatWindow : public MainWindow {
  Q_OBJECT
public:
  ChatWindow(QWidget *parent = nullptr);
  ~ChatWindow();
private slots:
  void onSendButtonClicked();
  void onSelectButtonClicked();
  void onAdditionButtonClicked();
  void onFriendButtonClicked(const QString &id);
  void showMessageOnBrowser();
  void showFileOnBrowser();
private:
  	void updateWidgetSize() override;
	void readFriendList();
	void updateFriendList();
	QVector<QToolButton*> m_friendButtonList;
	QVector<QAction*> m_friendActionList;
  	QTextBrowser *m_messageBrowser;
	QPushButton *m_sendButton;
	QPushButton *m_selectButton;
	QTextEdit *m_textEdit;
	QPushButton *m_additionButton;
	ToolBar *m_contactPerson;

	SizePercent m_messageBrowserSize;
	SizePercent m_sendButtonSize;
	SizePercent m_selectButtonSize;
	SizePercent m_textEditSize;
	SizePercent m_additionButtonSize;
	SizePercent m_contactPersonSize;
	SizePercent m_friendSize;
};

#endif //CHAT_SRC_WIDGET_H_
