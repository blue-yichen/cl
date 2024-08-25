#ifndef CHAT_FRIEND_H
#define CHAT_FRIEND_H
#include <QString>

struct Friend {
    QString m_username;
    QString m_aid;
    Friend() = default;
    Friend(const QString &aid,const QString &username)
        : m_aid(aid), m_username(username) { }
    // 重载==运算符，用于比较两个Friend对象是否相等
    bool operator==(const Friend &other) const {
        return m_username == other.m_username && m_aid == other.m_aid;
    }
};

inline uint qHash(const Friend &key, uint seed = 0) {
    return qHash(key.m_username, seed) ^ qHash(key.m_aid, seed);
}

#endif //CHAT_FRIEND_H
