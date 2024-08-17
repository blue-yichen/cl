#ifndef CHAT_SRC_USER_H_
#define CHAT_SRC_USER_H_
#include <QString>
#include <QHash>
#include "ChatString.h"

const int AidMinLength = 6;
const int PasswordMinlength = 6;

class User {
public:
  User(const QString &name,const  QString &aid,
	   const QString &password)
  	: m_name(name), m_aid(aid), m_password(password) { }
  User()
  	: User("","","") { }
  User(const QString &name)
  	: User(name,"","") { }
  User(const QString &name,const QString &aid)
	  : User(name,aid,"") { }
  ~User() = default;
  void addFriend(const QString &name,const QString &aid);
  void removeFriend(const QString &aid);
  bool setPassword(const QString &password);
  void setName(const QString &name);
  bool setAid(const QString &aid);
  bool setUser(const QString &name,
			   const QString &aid, const QString &password);
  const QString& getName() const {
	return m_name;
  }
  const QString& getAid() const {
	return m_aid;
  }
  const QString& getPassword() const {
	return m_password;
  }
  static bool isQualifiedForAid(const QString &aid);
  static bool isQualifiedForPassword(const QString &password);
private:
  QString m_name;
  QString m_aid;
  QString m_password;
  QHash<QString,QString> m_friends;
};

#endif //CHAT_SRC_USER_H_
