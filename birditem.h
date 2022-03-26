#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QGraphicsObject>
class QPropertyAnimation;

#define FLY_BIRD_SIZE 45


//游戏中的鸟儿对象(图形项)
//自定义图像时，需要从写paint,boundingRect
class BirdItem :public QGraphicsObject
{
    Q_OBJECT

public:
    BirdItem(QGraphicsScene* scene);
    ~BirdItem();
public:
    //重写绘图区域
    QRectF boundingRect() const;

    void flyRaiseAnimation();//上升动画
    void flyLandFallAnimation();//下降动画
    bool checkIsCollided();//碰撞检测


private slots:
    void onRefreshBird();//定时刷新小鸟
    void onFlyRaiseAnimationFinished();//上升动画结束

private:
    //重写绘图事件
    void paint(QPainter* ,const QStyleOptionGraphicsItem*,QWidget*);

public:

    //重写键盘事件-空格键控制小鸟
    void keyPressEvent(QKeyEvent* event);

private:
    int m_curflyStatus;//当前飞行状态
    bool m_IsLandFall;//下降标志
    bool m_IsRaise;//上升标志
    QGraphicsScene* m_scene;//所在场景
    QTimer* m_birdRefreshTimer;//刷新小鸟计时器
    QPropertyAnimation* m_flyAnimation;//飞行动画
};

#endif // BIRDITEM_H
