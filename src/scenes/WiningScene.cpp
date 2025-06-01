#include "src/scenes/WiningScene.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>


WiningScene::WiningScene(QObject *parent) : QGraphicsScene(parent)
{
    qDebug() << "[WiningScene] 已被構建";
    setup();
}

WiningScene::~WiningScene(){
    qDebug() << "[WiningScene] 已被解構";
}

void WiningScene::setup(){
    setSceneRect(0, 0, 550, 550);
    setBackgroundBrush(Qt::black);
    // 文字
    QGraphicsTextItem *t1 = new QGraphicsTextItem("遊戲結束！");
    t1->setFont(QFont("Arial", 40));
    t1->setDefaultTextColor(Qt::white);
    t1->setPos(100, 50);
    addItem(t1);


    QGraphicsTextItem *t2 = new QGraphicsTextItem("你贏了！");
    t2->setFont(QFont("Arial", 30));
    t2->setDefaultTextColor(Qt::white);
    t2->setPos(100, 100);
    addItem(t2);

    // 按鈕
    ClickableButton *g1Button = new ClickableButton("再玩一次（機器人）",
                                                       QRectF(100, 280, 200, 40),
                                                       ClickableButton::Action::StartG1,
                                                       nullptr);

    addItem(g1Button);
    connect(g1Button, &ClickableButton::clicked, this, &WiningScene::handleButtonClicked);


    ClickableButton *g2Button = new ClickableButton("再玩一次（怪物）",
                                                       QRectF(100, 330, 200, 40),
                                                       ClickableButton::Action::StartG2,
                                                       nullptr);

    addItem(g2Button);
    connect(g2Button, &ClickableButton::clicked, this, &WiningScene::handleButtonClicked);


    ClickableButton *g3Button = new ClickableButton("退出遊戲",
                                                       QRectF(100, 380, 200, 40),
                                                       ClickableButton::Action::QuitGame,
                                                       nullptr);

    addItem(g3Button);
    connect(g3Button, &ClickableButton::clicked, this, &WiningScene::handleButtonClicked);


    ClickableButton *g4Button = new ClickableButton("回到標題頁",
                                                       QRectF(100, 430, 200, 40),
                                                       ClickableButton::Action::Title,
                                                       nullptr);

    addItem(g4Button);
    connect(g4Button, &ClickableButton::clicked, this, &WiningScene::handleButtonClicked);
}

void WiningScene::handleButtonClicked(ClickableButton::Action action) {
    // 根據按鈕的動作類型執行相應操作
    switch (action) {

        case ClickableButton::Action::StartG1:
        // 發射信號，請求 GameWindow 切換到機器人遊戲
            qDebug() << "[WiningScene] '開始遊戲(機器人)' 按鈕被點擊。";
            emit requestSceneChange(sceneslist::gameMR);
            break;

        case ClickableButton::Action::StartG2:
        // 發射信號，請求 GameWindow 切換到怪物遊戲
            qDebug() << "[WiningScene] '開始遊戲(怪物)' 按鈕被點擊。";
            emit requestSceneChange(sceneslist::gameMM);
            break;

        case ClickableButton::Action::QuitGame:
        // 發射信號，請求 GameWindow 退出應用程式
            qDebug() << "[WiningScene] '離開遊戲' 按鈕被點擊。";
            emit requestQuitApplication();
            break;

    case ClickableButton::Action::Title:
        qDebug() << "[WiningScene] 回到標題頁";
        emit requestSceneChange(sceneslist::title);
        break;

        default:
            qWarning() << "[WiningScene] 未知或無動作的按鈕被點擊。";
            break;
    }
}
