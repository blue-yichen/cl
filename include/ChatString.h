#ifndef CHATSERVER_SRC_CHATSTRING_H_
#define CHATSERVER_SRC_CHATSTRING_H_
#include <QString>
#include "ControlMessage.h"

class ChatString {
  using ChatStringType = long long;
public:
  ChatString(const QString &data,const QString &bucket = "[]",
			 ChatStringType start = 0,ChatStringType end = -1)
 		: m_data(data), m_bucket(bucket), m_start(start),m_end(end) {
		if (m_end == -1) {
		  m_end = data.size() - 1;
		}
  }
  ~ChatString() {}
  QString getDataInBucket();
  QString getDataAndBucket();
  QString next();
  void skipNBucket(int n);
  //从后面数的第n个括号内的内容
  QString getEndNBucket(int n);
  void setData(const QString &data);
  void setBucket(const QString &bucket);
  QString toQString();
  static bool isNumberStr(const QString &str);
  static QString NumberToStr(int number);
  static QString generateRandomString(uint16_t len);
  ChatString operator+(const QString &str) {
	return str + m_data;
  }
  static QString getALineFormatStr(const QString &arg1,const QString &arg2);
  QString operator=(const QString &data) {
      setData(data);
      return m_data;
  }
private:
  int getBucketSum() const;
  QString m_data;
  QString m_bucket;
  ChatStringType m_start;
  ChatStringType m_end;
};

#endif //CHATSERVER_SRC_CHATSTRING_H_
