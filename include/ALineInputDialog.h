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

class ALineInputDialog : public QDialog {
public:
  ALineInputDialog(const QString &windowTitle,
				   const QString &label, QDialog *parent = nullptr);
  ~ALineInputDialog() = default;
  const QString& getInput() const {
	return m_inputData;
  }
protected:
  void keyPressEvent(QKeyEvent *event) override;
private:
  QString m_inputData;
  QLineEdit *m_inputEdit;
};

#endif //CHAT_SRC_DIALOG_H_
