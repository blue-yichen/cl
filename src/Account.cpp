#include "Account.h"

void Account::addFriend(const QString &name, const QString &aid) {
  m_friends[aid] = name;
}
void Account::removeFriend(const QString &aid) {
  if (m_friends.contains(aid)) {
	m_friends.remove(aid);
  }
}
bool Account::setPassword(const QString &password) {
  if (isQualifiedForPassword(password)) {
	m_password = password;
	return true;
  }
  return false;
}
void Account::setName(const QString &name) {
	m_name = name;
}
bool Account::setAid(const QString &aid) {
  if (isQualifiedForAid(aid)) {
	m_aid = aid;
	return true;
  }
  return false;
}
bool Account::setUser(const QString &name, const QString &aid, const QString &password) {
  if (isQualifiedForAid(aid) && isQualifiedForPassword(password)) {
	m_name = name;
	m_aid = aid;
	m_password = password;
	return true;
  }
  return false;
}

bool Account::isQualifiedForAid(const QString &aid) {
  if (aid.length() < AidMinLength || !ChatString::isNumberStr(aid)) {
	return false;
  }
  return true;
}
bool Account::isQualifiedForPassword(const QString &password) {
	if (password.length() < PasswordMinlength) {
	  return false;
	}
	return true;
}
QList<QString> Account::getAllAidOfFriend() {
  return m_friends.keys();
}
QList<QString> Account::getAllUsernameOfFriend() {
  return m_friends.values();
}

