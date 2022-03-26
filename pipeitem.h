#ifndef PIPEITEM_H
#define PIPEITEM_H
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QPainter>
#include <QPropertyAnimation>

class PipeItem : public QGraphicsObject
{
public:
    PipeItem(QGraphicsScene* scene);
    ~PipeItem();

    //同重写绘图区域与绘图事件，加上你自己的动画效果，来实现自定义的动画显示效果。
protected:
    //重写图像项的绘图区域
    QRectF boundingRect()const;

    //重写绘图事件
    void paint(QPainter* painter,const QStyleOptionGraphicsItem*,QWidget*);

    QPainterPath shape()const;
private:
    void startMove();//管道开始移动

    void createPipeHeight();//创建管道高度

private:

    QGraphicsScene* m_scene;
    int m_upPipeHeight;
    int m_downPipeHeight;
};

#endif // PIPEITEM_H
