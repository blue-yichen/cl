#include "Base32.h"

static QHash<QChar,int> Base32CharToNumber;

void Base32::initBase32() {
    for (int i = 0;i < 32;++i) {
        Base32CharToNumber[Base32Alphabet[i]] = i;
    }
}

QString Base32::encode(const QByteArray &data) {
    QString result;
    uint32_t bits = 0;
    uint32_t accumulator = 0;
    for (uint8_t byte : data) {
        //累加
        accumulator = accumulator << 8 | byte;
        bits += 8;
        //编码
        while (bits >= 5) {
            //将高五位编码
            bits -= 5;
            result += Base32Alphabet[accumulator >> bits & 0x1F];
        }
    }
    //剩下不满5位，则补0
    if (bits > 0) {
        result += Base32Alphabet[accumulator << (5 - bits) & 0x1F];
    }
    while (result.size() % 8 != 0) {
        result += '=';
    }

    return result;
}

QByteArray Base32::decode(const QString &encode) {
    QByteArray result;
    uint32_t bits = 0;
    uint32_t accumulator = 0;
    for (auto byte : encode) {
        if (byte == '=') {
            break;
        }
        //每个字节有5位有效数据,且这五位都在低位
        accumulator = accumulator << 5 | Base32CharToNumber[byte];
        bits += 5;
        //解码
        while (bits >= 8) {
            //从高位开始解码
            bits -= 8;
            result += static_cast<char>(accumulator >> bits & 0xFF);
        }
    }

    return result;
}