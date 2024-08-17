#ifndef CHAT_SRC_DIALOG_H_
#define CHAT_SRC_DIALOG_H_
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMessageBox>

class RegisterDialog : public QDialog {
public:
  RegisterDialog(QDialog *parent = nullptr);
  ~RegisterDialog() = default;
  const QString& getUserName() const {
	return m_username;
  }
protected:
  void keyPressEvent(QKeyEvent *event) override;
private:
  QString m_username;
  QLineEdit *m_inputEdit;
};

#endif //CHAT_SRC_DIALOG_H_
