#ifndef CHAT_SRC_TEXTEDIT_H_
#define CHAT_SRC_TEXTEDIT_H_
#include <QTextEdit>
#include <QKeyEvent>

const int TextMaxSize = 1024;

class TextEdit : public QTextEdit {
	Q_OBJECT
public:
  TextEdit(QTextEdit *parent = nullptr)
  	: QTextEdit(parent) { }
  ~TextEdit() = default;
signals:
  void returnPressed();
protected:
	void keyPressEvent(QKeyEvent *event) override;
private:
};

#endif //CHAT_SRC_TEXTEDIT_H_
