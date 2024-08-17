#ifndef CHAT_SRC_TOOLBAR_H_
#define CHAT_SRC_TOOLBAR_H_
#include <QToolBar>

class ToolBar : public QToolBar {
  Q_OBJECT
public:
  int maxButtonNumber(int buttonHeight) const {
	  return this->height() / buttonHeight;
  }
};

#endif //CHAT_SRC_TOOLBAR_H_
