#include "ALineInputDialog.h"

ALineInputDialog::ALineInputDialog(const QString &windowTitle, const QString &label, QDialog *parent)
	: QDialog(parent) {
	this->setWindowTitle(windowTitle);
	this->setMinimumSize(480,320);
  	this->setMaximumSize(640,480);
	QVBoxLayout *vlayout = new QVBoxLayout(this);
	QHBoxLayout *hlayout = new QHBoxLayout();
	QLabel *captionLabel = new QLabel();
	captionLabel->setText(label + ":");
  	m_inputEdit = new QLineEdit();
	hlayout->addWidget(captionLabel);
	hlayout->addWidget(m_inputEdit);
	vlayout->addLayout(hlayout);
}
void ALineInputDialog::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter ||
		event->key() == Qt::Key_Return) {
	    m_inputData = m_inputEdit->text();
		if (m_inputData.isEmpty()) {
		  QMessageBox::warning(this,"提醒",
								   "请输入用户名后再按下回车键");
		} else {
            manualCloseDialog(ManualCloseDialog);
            this->close();
		}
	}
    QDialog::keyPressEvent(event);
}

void ALineInputDialog::closeEvent(QCloseEvent *event) {
    //点击关闭按钮关闭的
    if (!m_isReturn) {
        this->done(QDialog::Rejected);
    }
    //否则就是点回车键确认的
    QDialog::closeEvent(event);
}


