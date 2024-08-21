#ifndef CHAT_SRC_MAINWINDOW_H_
#define CHAT_SRC_MAINWINDOW_H_
#include <QWidget>
#include <QResizeEvent>

struct SizePercent {
  double height;
  double width;
  SizePercent(double _height = 0.0, double _width = 0.0)
	  : height(_height), width(_width) { }
  void setSize(double _height, double _width) {
	  height = _height;
	  width = _width;
  }
};

class MainWindow : public QWidget {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setWidth(int width) {
	  m_width = width;
	  resize(m_width, m_height);
  }
  void setHeight(int height) {
	  m_height = height;
	  resize(m_width, m_height);
  }
  void setSize(int width,int height) {
	  setWidth(width);
	  setHeight(height);
	  resize(m_width, m_height);
  }
protected:
  void resizeEvent(QResizeEvent *event) override;
  virtual void updateWidgetSize() = 0;
  int m_width;
  int m_height;
};

#endif //CHAT_SRC_MAINWINDOW_H_
