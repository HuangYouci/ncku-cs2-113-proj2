#include "src/scenes/TitleScene.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>

TitleScene::TitleScene(QObject *parent) : QGraphicsScene(parent)
{
    qDebug() << "[TitleScene] 已被構建";
    setup();
}

TitleScene::~TitleScene(){
    qDebug() << "[TitleScene] 已被解構";
}

void TitleScene::setup(){
    // 設定大小
    setSceneRect(0, 0, 550, 550);

    // 背景圖片
    qDebug() << "[TitleScene] Trying to create QGraphicsPixmapItem 'bg'";
    QGraphicsPixmapItem *bg = new QGraphicsPixmapItem(QPixmap(":/data/ui/startpage.jpg").scaled(550, 550, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    addItem(bg);

    // 文字
    qDebug() << "[TitleScene] Trying to create Title text";
    QGraphicsTextItem *t1 = new QGraphicsTextItem("爆爆王 成大電機計概二第34組");
    QFont t1f("Arial", 20);
    t1f.setBold(true);
    t1->setFont(t1f);
    t1->setDefaultTextColor(Qt::black);
    t1->setPos(250, 200);
    addItem(t1);

    // 按鈕
    qDebug() << "[TitleScene] Trying to create title menu buttons";
    ClickableButton *g1Button = new ClickableButton("開始遊戲（機器人）",
                                                       QRectF(300, 280, 200, 40),
                                                       ClickableButton::Action::StartG1,
                                                       nullptr);

    addItem(g1Button);
    connect(g1Button, &ClickableButton::clicked, this, &TitleScene::handleButtonClicked);

    ClickableButton *g2Button = new ClickableButton("開始遊戲（怪物）",
                                                       QRectF(300, 330, 200, 40),
                                                       ClickableButton::Action::StartG2,
                                                       nullptr);

    addItem(g2Button);
    connect(g2Button, &ClickableButton::clicked, this, &TitleScene::handleButtonClicked);

    ClickableButton *g3Button = new ClickableButton("退出遊戲",
                                                       QRectF(300, 380, 200, 40),
                                                       ClickableButton::Action::QuitGame,
                                                       nullptr);

    addItem(g3Button);
    connect(g3Button, &ClickableButton::clicked, this, &TitleScene::handleButtonClicked);

}

void TitleScene::handleButtonClicked(ClickableButton::Action action) {
    // 根據按鈕的動作類型執行相應操作
    switch (action) {

        case ClickableButton::Action::StartG1:
        // 發射信號，請求 GameWindow 切換到機器人遊戲
            qDebug() << "[TitleScene] '開始遊戲(機器人)' 按鈕被點擊。";
            emit requestSceneChange(sceneslist::gameMR);
            break;

        case ClickableButton::Action::StartG2:
        // 發射信號，請求 GameWindow 切換到怪物遊戲
            qDebug() << "[TitleScene] '開始遊戲(怪物)' 按鈕被點擊。";
            emit requestSceneChange(sceneslist::gameMM);
            break;

        case ClickableButton::Action::QuitGame:
        // 發射信號，請求 GameWindow 退出應用程式
            qDebug() << "[TitleScene] '離開遊戲' 按鈕被點擊。";
            emit requestQuitApplication();
            break;

        default:
            qWarning() << "[TitleScene] 未知或無動作的按鈕被點擊。";
            break;
    }
}
