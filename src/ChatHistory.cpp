#include "ChatHistory.h"
#include "ChatString.h"
#include "SHA256.h"
#include "Data.h"
#include "Base32.h"
#include <unistd.h>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>

ChatHistory::ChatHistory(const QString &historyPath, QObject *parent)
		: m_historyPath(historyPath), QObject(parent) {
    //检查并修复此目录
    checkAndRepairPath(historyPath);
    connectSignals();
}

static inline QString getDataWithBucket(const QString &data,const QString
&bucket) {
    return bucket[0] + data + bucket[1];
}

void ChatHistory::onAppendOutline(const QString &chaterAid,
                                  SenderType senderType,
                                  const QString &messageOrFileId,
                                  Type::Type messageType,
                                  const QString &timestamp) {
    ChatOutline outline;
    if (senderType == SenderType::MySelf) {
        outline.m_aid = user->getAid();
    } else {
        outline.m_aid = chaterAid;
    }
    outline.m_type = Type::type[messageType];
    outline.m_content = messageOrFileId;
    outline.m_timestamp = timestamp;
    storeChatOutlineOnLocal(chaterAid,outline);
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



void ChatHistory::checkAndRepairPath(QString path) {
    QString checkDir;
    qDebug() << path;
    checkAndRepairPathHelper(checkDir,path);
}

void ChatHistory::checkAndRepairPathHelper(QString &checkPath, QString &srcPath) {
    //得到下一个/之前的字符串
    QString dirStr = checkPath + srcPath.section('/',0,0) + '/';
    deleteUntillChar(srcPath,'/');
    static QDir dir;
    //如果不存在则创建此目录
    if (!dir.exists(dirStr)) {
        dir.mkpath(dirStr);
    }
    if (!srcPath.isEmpty()) {
        checkAndRepairPathHelper(dirStr,srcPath);
    }
}

void ChatHistory::deleteUntillChar(QString &str,const QChar &ch) {
    int index = (int)str.indexOf(ch);
    if (index == -1) {
        str.clear();
    } else {
        str.remove(0,index + 1);
    }
}

void ChatHistory::loadAllChatHistories() {
    qDebug() << __FUNCTION__;
    QString chatHistoriesHashValue = isExistChatHistories();
    //不存在
    if (chatHistoriesHashValue.isEmpty()) {
        qDebug() << "is not exist: " + chatHistoriesHashValue;
        loadChatHistoriesWhileLocalNoExist();
    } else { //存在
        loadChatHistoriesWhileLocalExist(chatHistoriesHashValue);
    }

}

void ChatHistory::getAFriendChatHistoryFromLocal(const QString &aid) {
    QString outlinePath = m_historyPath + "/" + aid;
    qDebug() << "local path:" + outlinePath;
    if (!QFile::exists(outlinePath)) {
        qDebug() << aid + " local file no exist";
        return;
    }
    //在本地获取数据
    QFile outline(outlinePath + "/" + OutlineFileName);
    if (!outline.open(QFile::ReadOnly)) {
        //打开失败,结束获取
        qDebug() << "open " + outline.fileName() + " chatHistory error";
        qDebug() << "local no " + aid + " chat history";
        return;
    }
    //读取outline.txt
    qDebug() << "reading outline.txt";
    ChatString line("");
    ChatOutline ALine;
    while (!outline.atEnd()) {
        line = outline.readLine();
        qDebug() << "read line:" + line.toQString();
        //处理一条outline
        ALine.m_aid = line.getDataInBucket();
        ALine.m_type = line.getDataInBucket();
        ALine.m_content = line.getDataInBucket();
        ALine.m_timestamp = line.getDataInBucket();
        m_outlines[aid].emplace_back(ALine);
    }
    outline.close();
}

QVector<ChatOutline> ChatHistory::getAFriendHistory(const QString &aid) {
    return  m_outlines[aid];
}

void ChatHistory::getAllChatHistoriesFromLocal() {
    //从本地加载聊天记录
    QList<QString> aids = user->getAllAidOfFriend();
    if (aids.isEmpty()) {
        qDebug() << "no friends";
    }
    for (const auto &aid : aids) {
           getAFriendChatHistoryFromLocal(aid);
    }
}

void ChatHistory::getAllChatHistoriesFromServer() {
    receiveAllChatHistories();      //接收服务器发送的聊天记录
    storeAllChatHistories();            //存到本地
    decodeAllChatHistoriesContents();   //解码内容
}

void ChatHistory::loadChatHistoriesWhileLocalNoExist() {
    QString theMessageOfToSend;
    ChatString theReplyOfTheServer("");
    FormatMessage formatTheMessageOfToSend;
    theMessageOfToSend =
            ChatString::getALineFormatStr
                    (ControlMessage::Mes
                     [ControlMessage::LocalChatHistoryNoExist],
                     ControlMessage::Mes[ControlMessage::Null]);
    formatTheMessageOfToSend.addData(theMessageOfToSend);
    qDebug() << formatTheMessageOfToSend.toQString();
    socket->sendImmediatelyMessage(formatTheMessageOfToSend.toQString());
    if (socket->waitMessage(MaxWaitTime)) {
        theReplyOfTheServer = socket->readAll();
        qDebug() << "the reply of the server:" +
                    theReplyOfTheServer.toQString();
    }
    socket->reply();        //回复ok
    //服务器端有聊天记录
    if (theReplyOfTheServer.getDataInBucket() ==
        ControlMessage::Mes[ControlMessage::ChatHistoryExist]) {
        qDebug() << "server have chat history";
        getAllChatHistoriesFromServer();
        emit gotAllHistories();
    } else {        //服务器端没有聊天记录
        qDebug() << "server haven't chat history";
        return;     //结束加载
    }
}


void ChatHistory::loadChatHistoriesWhileLocalExist
(const QString &chatHistoriesHashValue) {
    QString theMessageOfToSend;
    ChatString theReplyOfTheServer("");
    FormatMessage formatTheMessageOfToSend;
    theMessageOfToSend =
            ChatString::getALineFormatStr
                    (ControlMessage::Mes
                     [ControlMessage::LocalChatHistoryExist],
                     ControlMessage::Mes[ControlMessage::Null]);
    theMessageOfToSend +=
            ChatString::getALineFormatStr
                    (ControlMessage::Mes
                     [ControlMessage::TheHashValueOfLocalChatHistory],chatHistoriesHashValue);
    formatTheMessageOfToSend.addData(theMessageOfToSend);
    socket->sendImmediatelyMessage(formatTheMessageOfToSend.toQString());
    if (socket->waitMessage(MaxWaitTime)) {
        theReplyOfTheServer = socket->readAll();
    }
    QString cmd = theReplyOfTheServer.getDataInBucket();
    //本地端记录与服务器端相同
    if (cmd  == ControlMessage::Mes
    [ControlMessage::LocalChatHistoryIsRight]) {
        qDebug() << "local chat history is right";
        getAllChatHistoriesFromLocal();
        decodeAllChatHistoriesContents();
        emit gotAllHistories();
        //本地端的记录与服务器端不匹配,先清理本地端的记录
    } else if (cmd == ControlMessage::Mes
    [ControlMessage::LocalChatHistoryIsNotRight]) {
        qDebug() << "local chat history is not right";
        socket->reply(); //回复ok
        clearAllChatHistories();            //清理本地错误的记录
        getAllChatHistoriesFromServer();     //从服务器获取记录
        emit gotAllHistories();
    } else {            //此账号不存在聊天记录
        qDebug() << "local chat history don't exist";
        clearAllChatHistories();        //清理错误记录
        emit allHistoriesCleared();
    }
}

QString ChatHistory::isExistChatHistories() {
    //从本地得到所有聊天记录
    getAllChatHistoriesFromLocal();
    QString result;
    QString AOutline;
    for (const auto &outlinesOfAFriend : m_outlines) {
        for (const auto &outline : outlinesOfAFriend) {
            AOutline = getDataWithBucket(outline.m_aid,"[]");
            AOutline += getDataWithBucket(outline.m_type,"[]");
            AOutline += getDataWithBucket(outline.m_content,"[]");
            AOutline += getDataWithBucket(outline.m_timestamp,"[]") + "\n";
            result += AOutline;
        }
    }
    qDebug() << "result :" + result;
    //清除刚读取的本地数据，防止数据污染
    m_outlines.clear();
    qDebug() << __FUNCTION__ ;
    if (result.isEmpty()) {
        return "";
    } else {
        return SHA256::calculateSha256(result.toUtf8());
    }
}

void ChatHistory::receiveAllChatHistories() {
    QString theMessageOfToReceive;
    //接收到聊天记录
    if (socket->waitMessage(MaxWaitTime)) {
        theMessageOfToReceive = socket->readAll();
    }
    QTextStream in(theMessageOfToReceive.toUtf8());
    ChatString parse("");
    ChatOutline outline;
    QString tmp;
    QVector<ChatOutline> *outlinesPtr = nullptr;
    //读取in文本流中的所有行
    while (!in.atEnd()) {
        //处理
        parse = in.readLine();
        qDebug() << "read line:" + parse.toQString();
        tmp = parse.getDataInBucket();
        if (tmp == ControlMessage::Mes
        [ControlMessage::Origin]) {
            //接下来的数据存入到outlinesPtr指向的vector中
            outlinesPtr =
                    &m_outlines[parse.getDataInBucket()];
        } else {
            if (!outlinesPtr) {
                qDebug() << "outlinesPtr 不应该为null,请检查你的程序";
                return;
            }
            outline.m_aid = tmp;
            outline.m_type = parse.getDataInBucket();
            outline.m_content = parse.getDataInBucket();
            outline.m_timestamp = parse.getDataInBucket();
            outlinesPtr->emplace_back(outline);
        }
    }
}


void ChatHistory::storeAllChatHistories() {
    QList<QString> aidsOfFriends = user->getAllAidOfFriend();
    for (const auto &aid : aidsOfFriends) {
        for (const auto &theOutlineOfAFriend : m_outlines) {
            for (const auto &outline : theOutlineOfAFriend) {
                storeChatOutlineOnLocal(aid,outline);
            }
        }
    }
}

void ChatHistory::clearAllChatHistories() {
    m_outlines.clear();
    //本地记录也全部清除
    //删除ChatHistory文件夹即可
    if (!removeDir(m_historyPath)) {
        qDebug() << "remove dir fail";
    }
}

bool ChatHistory::removeDir(QString dirPath) {
    qDebug() << "removeDir:" + dirPath;
    return removeDirHelper(dirPath);
}

bool ChatHistory::removeDirHelper(const QString &path) {
    QDir dir(path);
    if (!dir.exists()) {
       return false;
    }
    //删除目录内的所有子目录和文件
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,QDir::NoSort);
    for (auto &fileInfo : fileInfoList) {
        //是目录
        if (fileInfo.isDir()) {
            qDebug() << "remove dir:" + fileInfo.filePath();
            //如果递归删除目录失败，返回false
            if (!removeDirHelper(fileInfo.filePath())) {
                return false;
            }
        } else {
            //文件删除失败返回false
            qDebug() << "remove file:" + fileInfo.filePath();
            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                return false;
            }
        }
    }
    //删除空目录成功，返回其结果
    qDebug() << "rmdir " + path;
    return dir.rmdir(path);
}

void ChatHistory::storeChatOutlineOnLocal(const QString &chaterAid,const ChatOutline &outline) {
    QString content = getDataWithBucket(outline.m_aid, "[]");
    content += getDataWithBucket(outline.m_type, "[]");
    content += getDataWithBucket(outline.m_content.toUtf8(), "[]");
    content += getDataWithBucket(outline.m_timestamp, "[]");
    QString filePath = m_historyPath + "/" + chaterAid;
    //检查并修复路径
    checkAndRepairPath(filePath);
    QFile file(filePath + "/" + OutlineFileName);
    if (!file.open(QFile::Append)) {
        qDebug() << "file append error:file open error";
        return;
    }
    file.write(content.toUtf8() + "\n");
    file.close();
}

void ChatHistory::connectSignals() const {
    //加载到内存中后，再加载到browser的缓冲区中
    //连接失败
    if (!connect(this, &ChatHistory::gotAllHistories, chatWindow, &ChatWindow::onLoadedHistory)) {
        qDebug() << "connect got AllHistories and OnLoadedHistory fail";
    }
    //添加聊天记录

    if (!connect(chatWindow,&ChatWindow::appendOutline,this,&ChatHistory::onAppendOutline)) {
        qDebug() << "connect appendOutline And onAppendOutline fail";
    }
    if (!connect(socket,&NetworkClient::appendOutline,this,&ChatHistory::onAppendOutline)) {
        qDebug() << "connect appendOutline and onAppendOutline fail";
    }
}

void ChatHistory::printAOutline(const ChatOutline &outline) {
    qDebug() << getDataWithBucket(outline.m_aid,"[]") +
            getDataWithBucket(outline.m_type,"[]") +
            getDataWithBucket(outline.m_content,"[]") +
            getDataWithBucket(outline.m_timestamp,"[]");
}

void ChatHistory::decodeAllChatHistoriesContents() {
    for (auto &outlinesOfAFriend : m_outlines) {
        for (auto &outline : outlinesOfAFriend) {
            outline.m_content = Base32::decode(outline.m_content);
        }
    }
}
