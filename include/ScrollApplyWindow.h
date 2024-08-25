#ifndef CHAT_SCROLLAPPLYWINDOW_H
#define CHAT_SCROLLAPPLYWINDOW_H
#include <QScrollArea>
#include <QHBoxLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <QHash>
#include "Friend.h"
#include "FloatingWindow.h"

const bool ButtonLine = true;
const bool LabelLine = false;

class ScrollApplyWindow : public QWidget {
Q_OBJECT
public:
    explicit ScrollApplyWindow(bool lineType,FloatingWindow *floatingWindow,
                      QWidget *parent = nullptr);
    ~ScrollApplyWindow() = default;
    void addRaw(const QString &showLabel);
    void removeRaw(const QString &showLabel);
signals:
    void agreeButtonClicked(const Friend &aFriend);
    void refuseButtonClicked(const Friend &aFriend);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    FloatingWindow *m_floatingWindow;
    QVBoxLayout *m_containerLayout; //所有行布局封装为widget存储在其中
    QHash<QString,QWidget*> m_itemWidgets;
    bool m_lineType;
};


#endif //CHAT_SCROLLAPPLYWINDOW_H
