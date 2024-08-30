#ifndef CHAT_BASE32_H
#define CHAT_BASE32_H
#include <QString>
#include <QHash>
#include <QByteArray>

namespace Base32 {
    QString encode(const QByteArray &data);
    QByteArray decode(const QString &encode);
    const QString Base32Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    void initBase32();
};

#endif //CHAT_BASE32_H
