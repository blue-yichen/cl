#include "FormatMessage.h"
void FormatMessage::add(const QString &data,long long dataIndex) {
	//找到最后一个]
	long long prev = 0,next;
	while ((next = m_data.indexOf("]",prev)) != -1) {
		prev = next + 1;
	}
	if (!prev) {
	  prev += 1;
	}
	m_data.insert(prev,'\n');
	prev += 1;
	m_data.insert(prev,'\t');
	prev += 1;
	m_data.insert(prev,"[" + data + "]");
	prev = dataIndex;
	while ((next = m_data.indexOf(":",prev)) != -1) {
	  m_data.insert(next,']');
	  m_data.insert(next + 2,'[');
	  prev = next + 3;
	}
}

void FormatMessage::addData(const QString &fillData) {
  long long start = 0,end;
  while ((end = fillData.indexOf("\n",start)) != -1) {
	QString data = fillData.mid(start,end - start);
	add(data,m_data.size() - 1);
	start = end + 1;
  }
}
