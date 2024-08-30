#ifndef CHAT_SRC_LOGINMAINWINDOW_H_
#define CHAT_SRC_LOGINMAINWINDOW_H_
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFont>
#include <QKeyEvent>
#include "MainWindow.h"
#include "ChatWindow.h"
#include "ChatString.h"
#include "ReceiverThread.h"
#include "ControlMessage.h"
#include "ALineInputDialog.h"
#include "FormatMessage.h"
#include "NetworkClient.h"

const int TimeLimits = 5;

class LoginWindow : public MainWindow {
  Q_OBJECT
  typedef unsigned long long u64;
public:
  LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow() override = default;
protected:
  void keyPressEvent(QKeyEvent *event) override;
signals:
	void loginSuccessful(ChatString &message);	//登录成功信号
	void loginFail(ChatString &message);
	void registerSuccessful(); //注册成功信号
	void registerFail();
    void loginFinished();
protected:
    bool eventFilter(QObject *obj,QEvent *event) override;
private slots:
  	void login(); //响应pushbutton的clicked信号
	void registers();
    void onLoginSuccessful(ChatString &message);
    void onLoginFail(ChatString &message);
private:
  static bool isEnoughTimeLimits(u64 prevTime,u64 curTime);
  void updateWidgetSize() override;
  static void labelFontStyle(QLabel* &label, int fontSize, QFlag alignment);
  bool checkPasswordAndAid();
  QLabel *m_caption;
  QLabel *m_aid;
  QLabel *m_password;
  QLineEdit *m_aidEdit;
  QLineEdit *m_passwordEdit;
  QPushButton *m_loginButton;
  QPushButton *m_registerButton;

  SizePercent m_captionSize;
  SizePercent m_inputLabelSize;
  SizePercent m_editSize;
  SizePercent m_loginButtonSize;
};

#endif //CHAT_SRC_LOGINMAINWINDOW_H_
