#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QFrame>

class CBaseWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CBaseWidget(QWidget *parent = nullptr);
    virtual void mousePressEvent(QMouseEvent*)override;
    virtual void mouseReleaseEvent(QMouseEvent*)override;
    virtual void mouseMoveEvent(QMouseEvent*)override;
signals:
    void dragBegin();
    void dragEnd();
private:
      QPoint clickPos;
};

#endif // BASEWIDGET_H
