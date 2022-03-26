#include "birditem.h"
#include <QGraphicsScene>
#include <QTimer>
#include<QPropertyAnimation>
#include <QPainter>
#include <QKeyEvent>

BirdItem::BirdItem(QGraphicsScene* scene):
        m_curflyStatus(0),
        m_IsLandFall(true),
        m_IsRaise(true)
{
    //将当前的小鸟对象添加到游戏场景中去
    //this指的当前鸟儿对象
    scene->addItem(this);
    m_scene = scene;

    //定时刷新小鸟
    m_birdRefreshTimer = new QTimer(this);//初始化计时器
    connect(m_birdRefreshTimer,SIGNAL(timeout()),this,SLOT(onRefreshBrid()));
    m_birdRefreshTimer->start(10);

    //小鸟位置属性动画
    //通过设置小鸟的位置属性值，达到动画效果
    m_flyAnimation = new QPropertyAnimation(this,"pos");
    connect(m_flyAnimation,SIGNAL(finished()),
                this,SLOT(onFlyRaiseAnimationFinished()));

}

BirdItem::~BirdItem()
{

}

//重写绘图区域
QRectF BirdItem::boundingRect() const
{
    //左上角位置，宽，高
    return QRectF(60,FLY_BIRD_SIZE * 5,FLY_BIRD_SIZE,FLY_BIRD_SIZE);
}

//小鸟上升动画
void BirdItem::flyRaiseAnimation()
{
    if(m_IsRaise){
        m_IsRaise = false;
        m_flyAnimation->stop();//结束之前的动画
        int nMove = pos().y();//鸟儿移动的y值
        if(nMove > -180){//窗口正中间是原点
            m_flyAnimation->setDuration(300);//动画时长
            m_flyAnimation->setEndValue(QPoint(pos().x(),pos().y()-FLY_BIRD_SIZE));

        }else{//小鸟飞到了顶上
            m_flyAnimation->setDuration(300);
            m_flyAnimation->setEndValue(pos());
        }
        m_flyAnimation->setEasingCurve(QEasingCurve::Linear);//动画曲线效果
        m_flyAnimation->start();

         connect(m_flyAnimation,SIGNAL(finished()),this,SLOT(onFlyRaiseAnimationFinished()));

    }
}

//下降动画
void BirdItem::flyLandFallAnimation()
{
    if(m_IsLandFall){
        m_flyAnimation->stop();
        m_flyAnimation->setDuration(1500);
        m_flyAnimation->setEndValue(QPoint(pos().x(),m_scene->height()));//每次降一个小鸟
        m_flyAnimation->setEasingCurve(QEasingCurve::InQuad);
        m_flyAnimation->start();
        m_IsLandFall  = false;
    }
}

//碰撞检测
bool BirdItem::checkIsCollided()
{
    //collidingItems();//返回所有与当前图形项发生冲突的图形项
    if(!collidingItems().isEmpty()){
        return true;
    }else{
        return false;
    }
}

//刷新小鸟
void BirdItem::onRefreshBird()
{
    update();//刷新
}

void BirdItem::onFlyRaiseAnimationFinished()
{
    m_IsLandFall = true;
    flyLandFallAnimation();
}

//重写绘图事件
void BirdItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
{
    //设置小鸟图像及小鸟翅膀位置
    if(m_curflyStatus < 10){
         //翅膀往上
        m_curflyStatus++;
        painter->drawImage(boundingRect(),QImage(":/BirdGame/Resources/image/bird1.png"));
    }else if(m_curflyStatus < 20){
        //翅膀居中
        m_curflyStatus++;
        painter->drawImage(boundingRect(),QImage(":/BirdGame/Resources/image/bird2.png"));
    }else if(m_curflyStatus < 30){
        //翅膀往下
        m_curflyStatus++;
        painter->drawImage(boundingRect(),QImage(":/BirdGame/Resources/image/bird3.png"));
    }else{
        m_curflyStatus = 0;
    }
}

void BirdItem::keyPressEvent(QKeyEvent *event)
{
    //空格键
    if(event->key() == Qt::Key_Space){
        m_IsRaise = true;
        flyRaiseAnimation();
    }
}
