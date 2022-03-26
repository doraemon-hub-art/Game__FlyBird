#ifndef GAMESCENE_H
#define GAMESCENE_H

#include<QGraphicsScene>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject* parent,const QRectF &sceneRect);
    ~GameScene();
};

#endif // GAMESCENE_H
