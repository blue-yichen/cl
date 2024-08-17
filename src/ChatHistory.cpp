#include "ChatHistory.h"
#include "ChatString.h"
#include "Data.h"
#include <QDataStream>
#include <QDebug>

ChatHistory::ChatHistory(const QString &historyPath, QObject *parent)
		: m_historyPath(historyPath), QObject(parent) {

}

ChatHistory::~ChatHistory() {
	for (const auto &it : m_outline) {
	  delete it;
	}
	m_outline.clear();
}

QString ChatHistory::getAid(const QString &outline) {
  return getOutlineHelper(outline,0);
}

QString ChatHistory::getType(const QString &outline) {
  return getOutlineHelper(outline,1);
}

QString ChatHistory::getContent(const QString &outline) {
  return getOutlineHelper(outline,2);
}

QString ChatHistory::getTimestamp(const QString &outline) {
  return getOutlineHelper(outline,3);
}
QString ChatHistory::getOutlineHelper(const QString &outline, int skip_n) {
  ChatString data(outline);
  data.skipNBucket(skip_n);
  return data.getDataInBucket();
}
void ChatHistory::appendOutline(const QString &aid, const QString &message) {
	QString filePath = m_historyPath + aid + "outline.txt";
	QFile file(filePath);
	QString data;
	if (file.open(QFile::Append)) {
	  file.write(message.toUtf8() + "\n");
	  file.close();
	  //打开文件失败
	} else {

	}
}

const QVector<ChatOutline *> &ChatHistory::getAllOutline() {
  return m_outline;
}
bool ChatHistory::getFile(const QString &fileId,QString &filename,
						  QByteArray &fileData) {
  ChatString data(fileId);
  QString filePath = m_historyPath + "/" + data.getDataInBucket()
	  + "/" + "source" + "/" + data.getDataInBucket() + "/" + fileId;
  if (QFile::exists(filePath)) {
	data.next();
	filename = data.getDataInBucket();
	QFile file(filePath);
	if (file.open(QFile::ReadOnly)) {
	  fileData = file.readAll();
	  file.close();
	  return true;
	} else {
	  return false;
	}
  }
  return false;
}
void ChatHistory::clearHistory() {
	for (const auto &it : m_outline) {
	  delete it;
	}
	m_outline.clear();
	emit historyCleared();
}
void ChatHistory::readHistory() {

  emit historyRead(*this);
}

