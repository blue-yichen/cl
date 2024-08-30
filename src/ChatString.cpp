#include "ChatString.h"
#include <algorithm>
#include <QDebug>
#include <limits>
#include <random>

const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

QString ChatString::getDataInBucket() {
  ChatString::ChatStringType leftBucketIndex = m_data.indexOf(m_bucket[0],m_start);
  if (leftBucketIndex == -1) {
	return "";
  }
  ChatString::ChatStringType rightBucketIndex = m_data.indexOf(m_bucket[1],leftBucketIndex);
  if (rightBucketIndex == -1) {
	return "";
  }
  m_start = rightBucketIndex + 1;
  return m_data.mid(leftBucketIndex + 1,rightBucketIndex - leftBucketIndex - 1);
}
QString ChatString::next() {
  return getDataInBucket();
}
void ChatString::setData(const QString &data) {
	m_data = data;
    m_start = 0;
    m_end = data.size() - 1;
}
void ChatString::setBucket(const QString &bucket) {
	m_bucket = bucket;
}

void ChatString::skipNBucket(int n) {
  for (int i = 0;i < n;++i) {
	getDataInBucket();
  }
}
QString ChatString::toQString() {
  return m_data;
}
bool ChatString::isNumberStr(const QString &str) {
  for (const auto &it : str) {
	 if (!it.isDigit()) {
	   return false;
	 }
  }
  return true;
}
QString ChatString::NumberToStr(int number) {
    if (number == 0) {
        return "0";
    }
    QString result;
    bool flag = false;
    bool isMin = false;
    //思路是这样的
    //负数最小值当做正数最大值处理。将最后一位+1即可
    if (number < 0) {
        flag = true;
        if (number == std::numeric_limits<int>::min()) {
            number = std::numeric_limits<int>::max();
            isMin = true;
        } else {
            number = -number;
        }
    }
    while (number > 0) {
        result += (QChar('0' + number % 10));
        number /= 10;
    }
    //负数最小值要把最后一位替换掉
    std::reverse(result.begin(),result.end());
    if (flag) {
        result.insert(0,'-');
        //负数最小值
        if (isMin) {
            result[result.size() - 1] =
                    static_cast<QChar>(result[result.size() - 1].toLatin1() +
                    1);
        }
    }
    return result;
}
QString ChatString::getEndNBucket(int n) {
	//第n个字符串的内容 = skip(count - n)个括号后的内容
	int sum = getBucketSum();
	if (!(n <= sum && n >= 1)) {
	  return {};
	}
  	skipNBucket(sum - n);

	return getDataInBucket();
}
int ChatString::getBucketSum() const {
  int count = 0;
  ChatString str(m_data);
  while (!str.getDataInBucket().isEmpty()) {
	count++;
  }
  return count;
}

QString ChatString::getALineFormatStr(const QString &arg1, const QString &arg2) {
    return
            arg1 +
            ControlMessage::Mes[ControlMessage::Delimiter] +
            arg2 +
            ControlMessage::Mes[ControlMessage::EndFlag];
}

//得到括号中的数据(包括括号)
QString ChatString::getDataAndBucket() {
    ChatString::ChatStringType leftBucketIndex = m_data.indexOf(m_bucket[0],m_start);
    if (leftBucketIndex == -1) {
        return "";
    }
    ChatString::ChatStringType rightBucketIndex = m_data.indexOf(m_bucket[1],leftBucketIndex);
    if (rightBucketIndex == -1) {
        return "";
    }
    m_start = rightBucketIndex + 1;
    return m_data.mid(leftBucketIndex,rightBucketIndex - leftBucketIndex + 1);
}

QString ChatString::generateRandomString(uint16_t len) {
    QString result;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, (int)characters.size() - 1);
    for (uint16_t i = 0;i < len;++i) {
        result += characters[distribution(generator)];
    }

    return result;
}
