#include "RegisterDialog.h"

RegisterDialog::RegisterDialog(QDialog *parent)
	: QDialog(parent) {
	this->setWindowTitle("设置用户名");
	this->setMinimumSize(480,320);
  	this->setMaximumSize(640,480);
	QVBoxLayout *vlayout = new QVBoxLayout(this);
	QHBoxLayout *hlayout = new QHBoxLayout();
	QLabel *captionLabel = new QLabel();
	captionLabel->setText("用户名:");
  m_inputEdit = new QLineEdit();
	hlayout->addWidget(captionLabel);
	hlayout->addWidget(m_inputEdit);
	vlayout->addLayout(hlayout);
}
void RegisterDialog::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter ||
		event->key() == Qt::Key_Return) {
		m_username = m_inputEdit->text();
		if (m_username.isEmpty()) {
		  QMessageBox::warning(this,"提醒",
								   "请输入用户名后再按下回车键");
		} else {
		  this->close();
		}
	}
}
