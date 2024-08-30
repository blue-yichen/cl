#ifndef CHAT_SRC_CHATHISTORY_H_
#define CHAT_SRC_CHATHISTORY_H_
#include <QFile>
#include <QString>
#include <QHash>
#include <QVector>
#include "ChatWindow.h"
#include "DataFile.h"

struct ChatOutline {
  ChatOutline() = default;
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
  	~ChatHistory() override = default;
	void clearAllChatHistories();		//删除所有聊天记录
    QVector<ChatOutline> getAFriendHistory(const QString &aid);
	bool getFile(const QString &fileId,
				 QString &filename, QByteArray &fileData);
    void loadAllChatHistories();
    static void printAOutline(const ChatOutline &outline);
    bool removeDir(QString dirPath);
    void connectSignals() const;
    const ChatHistory& operator==(const ChatHistory &data) {
        m_historyPath = data.m_historyPath;
        m_outlines = data.m_outlines;
        return *this;
    }
    static void checkAndRepairPath(QString path);
signals:
  void gotAllHistories();		//聊天记录读取完毕，对于客户端在browser上显示，对于服务器端，...
  void allHistoriesCleared();           //清除browser上的内容
public slots:
    //只要是新消息，就追加到聊天记录中
    //新消息：发送的，接收到的
    /*
     * @param chaterAid 当前好友的aid
     * @param senderType 发送者是好友还是自己
     * @param messageOrFileId 消息内容
     * @param messageType 消息类型
     * */
  void onAppendOutline(const QString &chaterAid,
                       SenderType senderType,
                       const QString &messageOrFileId,
                       Type::Type messageType,const QString &timestamp);
private:
  static void checkAndRepairPathHelper(QString &checkPath,QString &srcPath);
  static void deleteUntillChar(QString &str,const QChar &ch);
  void decodeAllChatHistoriesContents();
  //接收服务器发送来聊天记录并将其存储到m_outlines中
  void receiveAllChatHistories();
  //将m_outlines中的聊天记录存储到本地文件中
  void storeAllChatHistories();
  //从本地目录加载聊天记录到m_outlines中
  void getAllChatHistoriesFromLocal();
  void getAFriendChatHistoryFromLocal(const QString &aid);
  void getAllChatHistoriesFromServer();
  void loadChatHistoriesWhileLocalExist(const QString &hashValue);
  void loadChatHistoriesWhileLocalNoExist();
  bool removeDirHelper(const QString &path);
  void storeChatOutlineOnLocal(const QString &chaterAid,
                               const ChatOutline &outline);
  //返回所有聊天记录的哈希值,若本地不存在聊天记录，则返回空字符串
  QString isExistChatHistories();
  QString m_historyPath;
  //每个好友都有聊天记录
  QHash<QString,QVector<ChatOutline>> m_outlines;
};

#endif //CHAT_SRC_CHATHISTORY_H_
