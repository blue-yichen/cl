#ifndef CHAT_SRC_CHATHISTORY_H_
#define CHAT_SRC_CHATHISTORY_H_
#include <QFile>
#include <QString>
#include <QHash>
#include <QVector>

namespace Type {
	const QString type[] = {
		"image","video","voice","text"
	};
	enum Type {
	  Image,Video,Voice,Text
	};
}

struct ChatOutline {
  ChatOutline(const QString &aid, const QString &type,
			  const QString &content, const QString &timestamp)
			  : m_aid(aid), m_type(type), m_content(content),
				m_timestamp(timestamp) {}
  QString m_aid;
  QString m_type;
  QString m_content;
  QString m_timestamp;
};

class ChatHistory : public QObject {
  Q_OBJECT
public:
  	explicit ChatHistory(const QString &historyPath,QObject
						   *parent = nullptr);
  	~ChatHistory() override;
	void readHistory();
	void clearHistory();		//删除所有聊天记录
	bool getFile(const QString &fileId,
				 QString &filename, QByteArray &fileData);
	const QVector<ChatOutline*>& getAllOutline();
	QString getAid(const QString &outline);
	QString getType(const QString &outline);
	QString getContent(const QString &outline);
	QString getTimestamp(const QString &outline);
signals:
  void historyRead(const ChatHistory &history);		//聊天记录读取完毕，对于客户端在browser上显示，对于服务器端，...
  void historyCleared();
public slots:
  void appendOutline(const QString &aid, const QString &message);
protected:
  QString m_historyPath;
  QVector<ChatOutline*> m_outline;
private:
  QString getOutlineHelper(const QString &outline,int skip_n);
};

#endif //CHAT_SRC_CHATHISTORY_H_
