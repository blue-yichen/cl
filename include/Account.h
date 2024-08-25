#ifndef CHAT_SRC_USER_H_
#define CHAT_SRC_USER_H_
#include <QString>
#include <QHash>
#include <QObject>
#include "Friend.h"
#include "ChatString.h"

const int AidMinLength = 6;
const int PasswordMinlength = 6;

class Account : public QObject {
  Q_OBJECT
  using AccountType = long long;
public:
  Account(const QString &name, const  QString &aid,
		  const QString &password)
  	: m_name(name), m_aid(aid), m_password(password) { }
  Account()
  	: Account("", "", "") { }
  Account(const QString &name)
  	: Account(name, "", "") { }
  Account(const QString &name, const QString &aid)
	  : Account(name, aid, "") { }
  ~Account() = default;
  void addFriend(const QString &name,const QString &aid);
  void removeFriend(const QString &aid);
  bool setPassword(const QString &password);
  void setName(const QString &name);
  bool setAid(const QString &aid);
  bool setUser(const QString &name,
			   const QString &aid, const QString &password);
  QList<QString> getAllAidOfFriend();
  QList<QString> getAllUsernameOfFriend();
  const QString& getName() const {
	return m_name;
  }
  const QString& getAid() const {
	return m_aid;
  }
  const QString& getPassword() const {
	return m_password;
  }
  QString getNameFromFriend(const QString &aid) const {
	if (m_friends.contains(aid)) {
	  return m_friends[aid];
	}
	return "";
  }
  AccountType getFriendsNumber() const {
	return m_friends.size();
  }
  static bool isQualifiedForAid(const QString &aid);
  static bool isQualifiedForPassword(const QString &password);
signals:
    void friendAdded(const Friend &aFriend);
private:
  QString m_name;
  QString m_aid;
  QString m_password;
  QHash<QString,QString> m_friends;
};

#endif //CHAT_SRC_USER_H_
