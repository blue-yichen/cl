#ifndef FLOATINGWINDOW_H
#define FLOATINGWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QString>

class FloatingWindow : public QWidget {
Q_OBJECT
signals:
    void clicked();
public:
    FloatingWindow(const QString &show,QWidget *parent = nullptr);
    ~FloatingWindow() = default;
    void showAtPrevPos();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QRect m_popupArea;
    QPoint m_dragStartPos;
    QPoint m_savePos;
};

#endif // FLOATINGWINDOW_H
