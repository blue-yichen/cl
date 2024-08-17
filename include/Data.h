#ifndef EXTERN_
#define EXTERN_ extern
#endif
#include "ChatHistoryClient.h"
#include "NetWorkClient.h"
#include "User.h"
#include <QHash>

EXTERN_ QHash<QString,ChatHistoryClient> (*histories);
EXTERN_ NetWorkClient *socket;
EXTERN_ User *user;