#ifndef EXTERN_
#define EXTERN_ extern
#endif
#include "NetworkClient.h"
#include "ChatHistory.h"
#include "Account.h"
#include "ReceiverThread.h"
#include "Friend.h"
#include <QHash>
#include <QMutex>
#include <QSystemTrayIcon>


//这个资源不需要线程锁，因为这个资源是用于单线程的
EXTERN_ QHash<QString,ChatHistory*> (*histories);
EXTERN_ NetworkClient *socket;
EXTERN_ Account *user;
EXTERN_ ReceiverThread *receiverThread;
EXTERN_ ChatWindow *chatWindow;
EXTERN_ LoginWindow *loginWindow;
EXTERN_ QSystemTrayIcon *tray;
EXTERN_ QSet<Friend> waitToAgreeTable;
EXTERN_ QSet<Friend> needToAgreeTable;
EXTERN_ QMutex waitMutex;
EXTERN_ QMutex needMutex;
EXTERN_ QMutex userMutex;