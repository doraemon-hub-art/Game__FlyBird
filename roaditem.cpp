#include "roaditem.h"
#include <QPainter>
#include <QPropertyAnimation>

#define ROAD_ITEM_HEIGHT 38

RoadItem::RoadItem(QGraphicsScene* scene):
       m_scene(scene)
{
    scene->addItem(this);//将当前道路图像项添加到游戏场景中去

    startMove();
}

RoadItem::~RoadItem()
{

}

//重写绘图区域
QRectF RoadItem::boundingRect() const
{
    //384*64
    return QRectF(0,m_scene->height() - ROAD_ITEM_HEIGHT,
                  m_scene->width() *2,ROAD_ITEM_HEIGHT);
}

//重写绘图事件
void RoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //drawImage
    //——将给定图像的矩形部分源绘制到绘制设备中的目标矩形中。

    //画前后两段马路
    painter->drawImage(QRectF(0,m_scene->height()-ROAD_ITEM_HEIGHT,
                              m_scene->width(),ROAD_ITEM_HEIGHT),
                                QImage(":/BirdGame/Resources/image/road.png"));

    painter->drawImage(QRectF(m_scene->width(),
                              m_scene->height()-ROAD_ITEM_HEIGHT,
                              m_scene->width(),ROAD_ITEM_HEIGHT),
                               QImage(":/BirdGame/Resources/image/road.png"));
}

void RoadItem::startMove()
{
    //pos是指根据位置设置属性动画
   QPropertyAnimation* moveAnimation = new QPropertyAnimation(this,"pos");
   //设置动画的循环次数，0不启动，-1无限循环
   moveAnimation->setLoopCount(-1);
   //设置动画时长
   moveAnimation->setDuration(6000);
   //设置起始值-开始位置
   moveAnimation->setStartValue(QPoint(0,pos().y()));//动画启动值_位置初始值
   moveAnimation->setEndValue(QPoint(0-m_scene->width(),pos().y()));//动画的结束值
   moveAnimation->setEasingCurve(QEasingCurve::Linear);
   moveAnimation->start();
}
