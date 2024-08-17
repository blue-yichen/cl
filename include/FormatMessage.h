//
// Created by yichen on 2024/8/15.
//

#ifndef CHATSERVER_SRC_FORMATMESSAGE_H_
#define CHATSERVER_SRC_FORMATMESSAGE_H_
#include <QString>

class FormatMessage {
public:
  FormatMessage(const QString &fillData = "")
  	: m_data("{\n}") {
	addData(fillData);
  }
  ~FormatMessage() = default;
  const QString& toQString() const {
	return m_data;
  }
  void setData(const QString &data) {
	m_data = data;
  }
  void addData(const QString &fillData);
private:
  void add(const QString &data,long long dataIndex = 0);
  QString m_data;
};

#endif //CHATSERVER_SRC_FORMATMESSAGE_H_
