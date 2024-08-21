#include "TextEdit.h"
void TextEdit::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return ||
      event->key() == Qt::Key_Enter) {
	emit returnPressed();
  } else {
	QTextEdit::keyPressEvent(event);
  }
}
