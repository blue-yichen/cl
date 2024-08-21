#include "ReceiverThread.h"
#include "LoginWindow.h"
#include "Data.h"

ReceiverThread::ReceiverThread(QThread *parent)
	: QThread(parent) {
  connect(loginWindow,&LoginWindow::loginSuccessful,this,&ReceiverThread::startReceive);
}

ReceiverThread::~ReceiverThread() {
  socket->moveToMainThread();
}
void ReceiverThread::startReceive() {
  //把与服务器通信的套接字移动到此线程
  //移动到当前线程
  socket->moveToThread(this);
  qDebug() << "socket move to child thread";
}

