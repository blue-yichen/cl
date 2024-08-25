#include "TextEdit.h"
#include <QDebug>
void TextEdit::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return ||
      event->key() == Qt::Key_Enter) {
	emit returnPressed();
  } else {
    //打字
    if (this->toPlainText().size() <= TextMaxSize) {
        QTextEdit::keyPressEvent(event);
    } else {
        //警告提示音
        qDebug() << "input text get to limits";
    }
  }
}
