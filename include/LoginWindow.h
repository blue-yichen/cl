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

class LoginWindow : public MainWindow {
  Q_OBJECT
public:
  LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow() override;
protected:
  void keyPressEvent(QKeyEvent *event) override;
signals:
	void loginSuccessful();	//登录成功信号
	void loginFail();
	void registerSuccessful(); //注册成功信号
	void registerFail();
private slots:
  	void login(); //响应pushbutton的clicked信号
	void registers();
private:
  void updateWidgetSize() override;
  void labelFontStyle(QLabel* &label, int fontSize, QFlag alignment);
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
