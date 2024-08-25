#ifndef EXTERN_
#define EXTERN_ extern
#endif
#include "ChatHistoryClient.h"
#include "NetworkClient.h"
#include "Account.h"
#include "ReceiverThread.h"
#include "Friend.h"
#include <QHash>
#include <QMutex>
#include <QSystemTrayIcon>


EXTERN_ QHash<QString,ChatHistoryClient> (*histories);
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