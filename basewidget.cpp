#include "basewidget.h"
#include <QMouseEvent>

CBaseWidget::CBaseWidget(QWidget *parent)
    : QFrame{parent}
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setFrameStyle(QFrame::Plain | QFrame::StyledPanel);
    setMouseTracking(true);
    clickPos = QPoint();
}

void CBaseWidget::mousePressEvent(QMouseEvent *e)
{
    clickPos = (e->globalPosition() - this->pos()).toPoint();
}

void CBaseWidget::mouseReleaseEvent(QMouseEvent *)
{
    clickPos = QPoint();
    emit dragEnd();
}

void CBaseWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(clickPos.x() != 0){
        auto delta = e->globalPosition() - clickPos;
        this->move(delta.toPoint());
    }
}
