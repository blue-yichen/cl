#ifndef CHAT_DATAFILE_H
#define CHAT_DATAFILE_H
#include <QString>

const QString OutlineFileName = "outline.txt";
const QString ChatHistoryDirName = "ChatHistory";
const QString PreviousPath = "../data/users/";

namespace Type {
    const QString type[] = {
            "image","video","sound","text"
    };
    enum Type {
        Image,Video,Sound,Text
    };
}

#endif //CHAT_DATAFILE_H
