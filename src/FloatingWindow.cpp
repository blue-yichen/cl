#include "FloatingWindow.h"
#include <QFont>
#include <QLabel>

FloatingWindow::FloatingWindow(const QString &show,QWidget *parent)
        : QWidget{parent},m_savePos(100,100)
{
    resize(50,50);
    // 设置无边框窗口和始终置顶
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // 设置默认样式表
    setStyleSheet("border: 2px solid gray; background-color: rgba(255, 255,"
                  " 255, 255); border-radius: 10px;");
    // 添加一个标签以测试显示内容
    QLabel *label = new QLabel(show, this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: black;");
    label->setGeometry(0,0,50,50);  // 设置标签位置和大小
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    m_popupArea = QRect(0,25,50,50);
}

void FloatingWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        this->move((event->globalPosition() - m_dragStartPos).toPoint());
        m_savePos = this->pos();
        event->accept();
    }
}

void FloatingWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (m_popupArea.contains(event->pos())) {
            emit clicked();
            event->accept();
        } else {
            m_dragStartPos = (event->globalPosition() - frameGeometry()
                    .topLeft()).toPoint();
            event->accept();
        }
    }
}

void FloatingWindow::showAtPrevPos() {
    show();
    move(m_savePos);
}
