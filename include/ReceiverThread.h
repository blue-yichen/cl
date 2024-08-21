#ifndef CHAT_SRC_RECEIVERTHREAD_H_
#define CHAT_SRC_RECEIVERTHREAD_H_
#include <QThread>
#include "LoginWindow.h"

class ReceiverThread : public QThread {
  Q_OBJECT
public:
  ReceiverThread(QThread *parent = nullptr);
  ~ReceiverThread();
public slots:
  void startReceive();
protected:
  void run() override {
	this->exec();
  }
};

#endif //CHAT_SRC_RECEIVERTHREAD_H_
