#ifndef EXTERN_
#define EXTERN_ extern
#endif
#include "ChatHistoryClient.h"
#include "NetworkClient.h"
#include "Account.h"
#include "ReceiverThread.h"
#include <QHash>

EXTERN_ QHash<QString,ChatHistoryClient> (*histories);
EXTERN_ NetworkClient *socket;
EXTERN_ Account *user;
EXTERN_ ReceiverThread *receiverThread;
EXTERN_ ChatWindow *chatWindow;
EXTERN_ LoginWindow *loginWindow;
