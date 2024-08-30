#include "ScrollApplyWindow.h"
#include "Data.h"

ScrollApplyWindow::ScrollApplyWindow(bool lineType,
                                     FloatingWindow *floatingWindow,
                                     QWidget *parent)
                                     : QWidget(parent),
                                     m_floatingWindow
                                     (floatingWindow),
                                     m_lineType(lineType) {
    //大小
    setFixedSize(300,400);
    //窗口布局
    auto *mainLayout = new QVBoxLayout(this);
    //滚动条区域
    auto *scrollArea = new QScrollArea(this);
    auto *areaContainer = new QWidget(scrollArea);
    m_containerLayout = new QVBoxLayout(areaContainer);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
    this->setLayout(mainLayout);
    setWindowTitle("好友申请");
    setStyleSheet("background-color: white;");
}

void ScrollApplyWindow::closeEvent(QCloseEvent *event) {
    if (m_floatingWindow) {
        m_floatingWindow->show();
    }
    QWidget::closeEvent(event);
}

void ScrollApplyWindow::addRaw(const QString &showLabel) {
    auto *label = new QLabel(showLabel);
    auto *layout = new QHBoxLayout();
    layout->addWidget(label);
    auto *container = new QWidget();
    if (m_lineType == ButtonLine) {
        auto *agreeButton = new QPushButton("同意");
        auto *refuseButton = new QPushButton("拒绝");
        QString username = showLabel.section('(',0,0);
        QString aid = showLabel.section('(',1,1).section(')',0,0);
        layout->addWidget(agreeButton);
        layout->addWidget(refuseButton);
        connect(agreeButton,&QPushButton::clicked,[=]() {
            emit agreeButtonClicked(Friend(aid,username));
        });
        connect(refuseButton,&QPushButton::clicked,[=]() {
            emit refuseButtonClicked(Friend(aid,username));
        });
    } else {
        auto *applyState = new QLabel("已申请,等待同意");
        layout->addWidget(applyState);
    }
    container->setLayout(layout);
    //方便删除
    m_itemWidgets[showLabel] = container;
    m_containerLayout->addWidget(container);
}

void ScrollApplyWindow::removeRaw(const QString &showLabel) {
    if (m_itemWidgets.contains(showLabel)) {
        m_containerLayout->removeWidget(m_itemWidgets[showLabel]);
        delete m_itemWidgets[showLabel];
        m_itemWidgets.remove(showLabel);
    }
}
