//
// Created by yichen on 2024/8/14.
//

#include "ChatString.h"
#include <algorithm>
#include <QDebug>
//as[adf]
//adf
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
  if (number < 0) {
	return "";
  }
  QString result;
  while (number > 0) {
	result = result + QChar(number % 10 + '0');
	number /= 10;
  }
  std::reverse(result.begin(),result.end());
  return result;
}
