#include <QApplication>
#include <QString>
#include <QMenu>
#include <QAction>
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
    QAction showAction("显示");
    QAction quitAction("退出");
    QMenu trayMenu;
    trayMenu.addAction(&showAction);
    trayMenu.addAction(&quitAction);
    tray = new QSystemTrayIcon();
    tray->setIcon(QIcon("../source/icons/chat.svg"));
    tray->setContextMenu(&trayMenu);
  	user = new Account();
  	loginWindow = new LoginWindow();
  	receiverThread = new ReceiverThread();
	receiverThread->start();
  	chatWindow = new ChatWindow();
  	socket = new NetworkClient("127.0.0.1", 12345);
	histories = new QHash<QString,ChatHistoryClient>();
	loginWindow->show();

    QObject::connect(&showAction,&QAction::triggered,[]() {
       chatWindow->showAtPrevPos();
       waitMutex.lock();
       if (!waitToAgreeTable.isEmpty()) {
           chatWindow->showWaitButton();
       }
       waitMutex.unlock();

       needMutex.lock();
       if (!needToAgreeTable.isEmpty()) {
           chatWindow->showNeedButton();
       }
       needMutex.unlock();
       qDebug() << "chatWindow shown";
    });
    QObject::connect(&quitAction,&QAction::triggered,[]() {
       chatWindow->quit();
       chatWindow->close();
       qDebug() << "chatWindow closed";
    });


	return QApplication::exec();
}
