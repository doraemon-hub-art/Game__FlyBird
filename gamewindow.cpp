#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QSequentialAnimationGroup>
#include<QPropertyAnimation>
#include <QStyleOption>
#include"buttonitem.h"
#include <QTImer>

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow),
    m_startGame(false)
{
    ui->setupUi(this);
    initControl();
    initBackGroundMusic();
    this->setWindowTitle("飞翔的小鸟");
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    //自定义Paint事件
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_startGame){
         m_bridItem->keyPressEvent(event);
    }
}

void GameWindow::initControl()
{
    //加载样式
    loadStyleSheet("GameWindow");

    //设置场景
    const QRect gameRect = rect();//获取当前游戏场景的矩形区域
    m_scene = new GameScene(this,gameRect);//初始化游戏场景
    QGraphicsView* view = new QGraphicsView(m_scene,this);
    view->setScene(m_scene);//设置游戏场景
    view->setStyleSheet("border:none;\
                                        background:transparent;");//设置视图无边框透明
    view->setCacheMode(QGraphicsView::CacheBackground);//设置缓存为背景模式
    startWelcome();

}

//加载样式
void GameWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/BirdGame/Resources/qss/"+sheetName+".css");

    if(file.open(QFile::ReadOnly)){
        QString strStyleSheet = this->styleSheet();//获取当前样式
        strStyleSheet += file.readAll();//加载出来的样式与当前已有的样式进行组合
        this->setStyleSheet(strStyleSheet);
        file.close();
    }
}

//启动欢迎
void GameWindow::startWelcome()
{
    //道路
    RoadItem * roadItem = new RoadItem(m_scene);

    //管道
    PipeItem* pipeItem = new PipeItem(m_scene);

    //小鸟
    m_bridItem= new BirdItem(m_scene);

    //游戏状态检测
    m_checkGameStatusTimer = new QTimer(this);
    //定时检测游戏状态
    connect(m_checkGameStatusTimer,SIGNAL(timeout()),this,SLOT(onCheckGameStatus()));

    //封面句子——创建结构体直接使用
    const int nLetters = 15;
    struct
    {
        char const*pix;
        qreal initX;
        qreal initY;
        qreal destX;
        qreal destY;
    }letterData[nLetters] = {
        {"飞", -1000, -1000, 250, 100 },//第一行
        {"吧", -800, -1000, 300, 100 },
        {"，", -600, -1000, 350, 100 },
        {"像", -400, -1000, 150, 160 },//第二行
        {"风", 1000, 2000, 200, 160 },
        {"一", 800, 2000, 250, 160 },
        {"样", 600, 2000, 300, 160 },
        {"自", 400, 2000, 350, 160 },
        {"由", 200, 2000, 400, 160 },
        {"，", 0, 2000, 450, 160 },
        {"无", 0, 2000, 175, 220 },//第三行
        {"法", 0, 2000, 225, 220 },
        {"挽", 0, 2000, 275, 220 },
        {"留", 0, 2000, 325, 220 },
        {"...", 0, 2000, 375, 220 }
    };

    //添加连续动画组
    QSequentialAnimationGroup* letterGroupMoving = new QSequentialAnimationGroup(this);

    //并行动画组
    m_lettersGroupFading = new QParallelAnimationGroup(this);

    for(int i = 0;i<nLetters;i++){
        //文本图形项
        QString htmlText = QString("<span style=\"font-family:\
                                                    'Berlin Sans FB';font-size:38px;\
                                                    font-weight:600;color:#194819;\">\
                                                    %1</span>").
                                                    arg(letterData[i].pix);
        QGraphicsTextItem* letter = new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(letterData[i].initX,letterData[i].initY);

        //文本项属性动画
        QPropertyAnimation* moveAnimation = new QPropertyAnimation(letter,"pos",letterGroupMoving);
        //设置字符动画结束的位置
        moveAnimation->setEndValue(QPoint(letterData[i].destX,letterData[i].destY));
        moveAnimation->setDuration(200);//设置动画时长
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);//设置动画曲线效果
        letterGroupMoving->addPause(50);

        //设置渐渐消失动画——设置透明度越来越透明
        QPropertyAnimation *fadeAnimation = new QPropertyAnimation(letter,"opacity",m_lettersGroupFading);
        fadeAnimation->setDuration(1000);
        fadeAnimation->setEndValue(0);
        fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

        m_scene->addItem(letter);//往游戏场景中添加字符图像项
    }
    //启动字符动画
     //动画停止后删除——达到只播放一遍的效果
    letterGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);

    //按钮图形项
    QPixmap pix = QPixmap(":/BirdGame/Resources/image/startButton.png");
    ButtonItem* btnItem = new ButtonItem(pix,m_scene);
    btnItem->setPos(QPoint(240,300));

    //按钮渐渐消失
    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(btnItem,"opacity",
                                                               m_lettersGroupFading);
    fadeAnimation->setDuration(600);//动画时长
    fadeAnimation->setEndValue(0);//opacity为0就全透明
    fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);
    //按钮图像项点击，则开始游戏
    connect(btnItem,SIGNAL(clicked()),this,SLOT(onStartBtnClicked()));
    connect(fadeAnimation,&QPropertyAnimation::finished,
            [this](){
        m_startGame = true;
        m_checkGameStatusTimer->start(50);
        m_bridItem->flyRaiseAnimation();//执行往下飞的动画

        //点击开始按钮后更换音乐
        m_welcomePlayer->stop();
        m_welcomePlayerList->clear();
        //m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/background.mp3"));
        m_welcomePlayer->play();
    });
}

void GameWindow::GameOver()
{
    const int nLetters = 4;
    struct{
            char const *pix;
            qreal initX, initY;
            qreal destX, destY;
        }letterData[nLetters] = {
        {"小", -1000, -1000, 200, 160 },//第一行
        {"辣", -800, -1000, 250, 160 },
        {"鸡", -600, -1000, 300, 160 },
        {"!", -400, -1000, 350, 160 },//第二行
        };

    //文字动画组
    QParallelAnimationGroup* lettersGroupMoving = new QParallelAnimationGroup(this);

    for(int i= 0;i < nLetters;i++){
        //文本图形项
               QString htmlText = QString("<span style=\"font-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819;\">%1</span>").arg(letterData[i].pix);
               QGraphicsTextItem *letter = new QGraphicsTextItem();
               letter->setHtml(htmlText);
               letter->setPos(letterData[i].initX,letterData[i].initY);

               //文本项属性动画
               QPropertyAnimation *moveAnimation = new QPropertyAnimation(letter,"pos",lettersGroupMoving);
               moveAnimation->setEndValue(QPointF(letterData[i].destX,letterData[i].destY));//设置字符动画结束的位置
               moveAnimation->setDuration(200);//动画时长200ms
               moveAnimation->setEasingCurve(QEasingCurve::OutElastic);//动画曲线效果

               //文本图形项添加到游戏场景中去
               m_scene->addItem(letter);
    }

    lettersGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);

    //将小鸟图形项删除
    m_scene->removeItem(m_bridItem);

    //游戏结束的音效
    m_welcomePlayer->stop();
    m_welcomePlayerList->clear();
    //m_welcomePlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/gameover.wav"));
    m_welcomePlayer->setVolume(100);
    m_welcomePlayer->play();
}

void GameWindow::initBackGroundMusic()
{

    m_welcomePlayer = new QMediaPlayer(this);//播放器

    //播放一次
    //m_welcomePlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
    //m_welcomePlayer->setVolume(100);
    //m_welcomePlayer->play();

    //主菜单音乐-循环播放
    m_welcomePlayerList = new QMediaPlaylist(this);
    //m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
    m_welcomePlayerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//循环播放当前列表曲目
    //将播放列表放入播放器中
    m_welcomePlayer->setPlaylist(m_welcomePlayerList);
    m_welcomePlayer->play();//播放
}

void GameWindow::onStartBtnClicked()
{
    //启动渐渐消失动画，动画结束后删除
    m_lettersGroupFading->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWindow::onCheckGameStatus()
{
    //检查鸟儿是否与场景有冲突
    if(m_bridItem->checkIsCollided()){
        GameOver();
    }else{

    }
}
