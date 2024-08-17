#include "MainWindow.h"
MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent) {

}

MainWindow::~MainWindow() = default;

void MainWindow::resizeEvent(QResizeEvent *event) {

	QSize newSize = event->size();
	m_width = newSize.width();
	m_height = newSize.height();
	QWidget::resizeEvent(event);
	updateWidgetSize();
	QWidget::resizeEvent(event);
}
