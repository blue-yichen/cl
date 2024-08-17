//
// Created by yichen on 2024/8/14.
//

#ifndef CHATSERVER_SRC_CHATSTRING_H_
#define CHATSERVER_SRC_CHATSTRING_H_
#include <QString>

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
  QString next();
  void skipNBucket(int n);
  void setData(const QString &data);
  void setBucket(const QString &bucket);
  QString toQString();
  static bool isNumberStr(const QString &str);
  static QString NumberToStr(int number);
private:
  QString m_data;
  QString m_bucket;
  ChatStringType m_start;
  ChatStringType m_end;
};

#endif //CHATSERVER_SRC_CHATSTRING_H_
