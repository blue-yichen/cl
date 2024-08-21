#include <QApplication>
#include <QString>
#include <QFile>
#include "LoginWindow.h"
#define EXTERN_
#include "Data.h"
#undef EXTERN_


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
	//加载qss
	QFile qss("../source/qss/MacOS.qss");
	if (qss.open(QFile::ReadOnly)) {
		QString styleSheet(qss.readAll());
		a.setStyleSheet(styleSheet);
		qss.close();
	} else {
		qDebug() << "qss open error";
		exit(EXIT_FAILURE);
	}
  	user = new Account();
  	loginWindow = new LoginWindow();
  	receiverThread = new ReceiverThread();
	receiverThread->start();
  	chatWindow = new ChatWindow();
  	socket = new NetworkClient("127.0.0.1", 12345);
	histories = new QHash<QString,ChatHistoryClient>();

	loginWindow->show();

	return QApplication::exec();
}
