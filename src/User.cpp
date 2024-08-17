#include "User.h"

void User::addFriend(const QString &name,const QString &aid) {
  m_friends[aid] = name;
}
void User::removeFriend(const QString &aid) {
  if (m_friends.contains(aid)) {
	m_friends.remove(aid);
  }
}
bool User::setPassword(const QString &password) {
  if (isQualifiedForPassword(password)) {
	m_password = password;
	return true;
  }
  return false;
}
void User::setName(const QString &name) {
	m_name = name;
}
bool User::setAid(const QString &aid) {
  if (isQualifiedForAid(aid)) {
	m_aid = aid;
	return true;
  }
  return false;
}
bool User::setUser(const QString &name, const QString &aid, const QString &password) {
  if (isQualifiedForAid(aid) && isQualifiedForPassword(password)) {
	m_name = name;
	m_aid = aid;
	m_password = password;
	return true;
  }
  return false;
}

bool User::isQualifiedForAid(const QString &aid) {
  if (aid.length() < AidMinLength || !ChatString::isNumberStr(aid)) {
	return false;
  }
  return true;
}
bool User::isQualifiedForPassword(const QString &password) {
	if (password.length() < PasswordMinlength) {
	  return false;
	}
	return true;
}

