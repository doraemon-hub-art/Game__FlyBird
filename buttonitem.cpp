#include "buttonitem.h"
#include<QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include<QPainter>

ButtonItem::ButtonItem(const QPixmap& pixmap,QGraphicsScene*scene):
    m_pix(pixmap)
{
    scene->addItem(this);//场景中添加当前按钮图形对象
    //鼠标为手指形状
    setCursor(QCursor(Qt::PointingHandCursor));

}

ButtonItem::~ButtonItem()
{

}

//重写按钮图形项的绘图区域
QRectF ButtonItem::boundingRect() const
{
    return QRectF(QPointF(0,0),m_pix.size());
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //判断按下为左键则发射点击信号
    if(event->button() == Qt::LeftButton){
        emit clicked();
    }
}

void ButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0,0,m_pix);
}
