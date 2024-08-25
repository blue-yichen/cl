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

const int ManualCloseDialog = 10;

class ALineInputDialog : public QDialog {
    Q_OBJECT
public:
  ALineInputDialog(const QString &windowTitle,
				   const QString &label, QDialog *parent = nullptr);
  ~ALineInputDialog() = default;
  const QString& getInput() const {
	return m_inputData;
  }
protected:
  void keyPressEvent(QKeyEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
private:
  void manualCloseDialog(int valueReturn) {
      m_isReturn = true;
      this->done(valueReturn);
  }
  QString m_inputData;
  QLineEdit *m_inputEdit;
  bool m_isReturn;
};

#endif //CHAT_SRC_DIALOG_H_
