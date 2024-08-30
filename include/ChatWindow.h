#ifndef CHAT_SRC_WIDGET_H_
#define CHAT_SRC_WIDGET_H_
#include <QResizeEvent>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
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
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QSoundEffect>
#include "ScrollApplyWindow.h"
#include "DataFile.h"
#include "TextEdit.h"
#include "MainWindow.h"
#include "ALineInputDialog.h"
#include "Account.h"
#include "Friend.h"
#include "FloatingWindow.h"

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
  void readFriendsRelation();
  void quit();
  void showWaitButton();
  void showNeedButton();
  void showAtPrevPos();
  void saveCurPos();
signals:
  void showMessageOnBrowser(
          SenderType type,
          const QString &senderAid,
          const QString &messageContent);
  void sendMessage(const QString &messageContent);
  void appendOutline(const QString &chatAid,
                         SenderType senderType,
                         const QString &messageOrFileName,
                         Type::Type messaegType,
                         const QString &timestamp);
public slots:
  void onSendButtonClicked();
  void onSelectButtonClicked();
  void onAdditionButtonClicked();
  //接收到消息就显示在textBrowser上
  void addMessageOnBrowser(SenderType type,const QString &senderAid,
                           const QString &messageContent);
  void addFileOnBrowser(const QString &fileType);
  void playVideoOnBrowser(const QUrl &url);
  void friendButtonClicked(const QToolButton *button);
  void friendButtonClicked(const QAction *button);
  void onWaitingAgreeAdded(const Friend &aFriend);
  void onNeedingAgreeAdded(const Friend &aFriend);
  void onWaitingAgreeRemoved(const Friend &aFriend);
  void onNeedingAgreeRemoved(const Friend &aFriend);
  void onFriendAdded(const Friend &aFriend);
  void onShowMessage(const QString &title,const QString &message);
  void onLoadedHistory();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
  	void updateWidgetSize() override;
	int findWhichButton(const QToolButton *button) const ;
	int findWhichButton(const QAction *button) const ;
	void updateBrowser();
	bool isToolButton(int chatId) const;
    void loadAFriendHistoryToBrowser(void *history,const QString &aid);
    void handlerMagicMessage(const QString &message);
    void handlerNormalMessage(const QString &message);
    bool checkMagicMessage(const QString &message);
    bool isSameFriend(const QString &label);
	int m_currentChatFriendId;
    bool m_isQuit;
    bool m_isStartMessageTone;
    QPoint m_savePos;
	QVector<QToolButton*> m_friendButtonList;
	QVector<QAction*> m_friendActionList;
	//QToolButton在前，QAction在后
	QVector<QString> m_perFriendContents;
	//aid和按钮编号的映射
	QHash<QString,int> m_friendChatId;
	QHash<int,QString> m_friendChatAid;
    //消息提示音
    QSoundEffect *m_messageTone;
    //控件
  	QTextBrowser *m_messageBrowser;
	QPushButton *m_sendButton;
	QPushButton *m_selectButton;
	TextEdit *m_textEdit;
	QPushButton *m_additionButton;
	QToolBar *m_contactPerson;
    FloatingWindow *m_waitAgreeButton;
    FloatingWindow *m_needAgreeButton;
    ScrollApplyWindow *m_waitAgreeList;
    ScrollApplyWindow *m_needAgreeList;

	SizePercent m_messageBrowserSize;
	SizePercent m_sendButtonSize;
	SizePercent m_selectButtonSize;
	SizePercent m_textEditSize;
	SizePercent m_additionButtonSize;
	SizePercent m_contactPersonSize;
	SizePercent m_friendSize;
};

#endif //CHAT_SRC_WIDGET_H_
