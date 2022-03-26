#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include"gamescene.h"
#include"birditem.h"
#include <QFile>
#include<QGraphicsview>
#include<QParallelAnimationGroup>
#include"roaditem.h"
#include "pipeitem.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

protected:

    //重写绘图事件
    void paintEvent(QPaintEvent* event);

    //重写键盘按键事件
    void keyPressEvent(QKeyEvent* event);
private:
    void initControl();//初始化部件
    void loadStyleSheet(const QString& sheetName);//加载样式
    void startWelcome();//启动欢迎
    void GameOver();//游戏结束
    void initBackGroundMusic();//初始化背景音乐
private slots:
    void onStartBtnClicked();

    void onCheckGameStatus();
private:
    Ui::GameWindow *ui;
    GameScene* m_scene;//游戏场景
    BirdItem* m_bridItem;//鸟儿对象
    bool m_startGame;//开始游戏标志
    QParallelAnimationGroup* m_lettersGroupFading;//渐渐消失动画组
    QTimer* m_checkGameStatusTimer;//检测游戏状态
    QMediaPlayer* m_welcomePlayer;//欢迎音乐
    QMediaPlaylist* m_welcomePlayerList;//片头音乐播放列表
};

#endif // GAMEWINDOW_H
