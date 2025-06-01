#ifndef GameMMTScene_H
#define GameMMTScene_H

#include <QGraphicsScene> // 場景建立
#include <QObject>        // 連接槽
#include <QVector>        // 動態陣列
#include <QPointF>        // 座標
#include <QKeyEvent>      // 鍵盤
#include <QTimer>         // Tick
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

#include "src/core/enums.h"
#include "src/objects/InBrick.h"
#include "src/objects/MaBrick.h"
#include "src/objects/FxBrick.h"
#include "src/objects/Player.h"
#include "src/objects/WaterBalloon.h"
#include "src/objects/WaterExplosion.h"
#include "src/objects/DropItem.h"
#include "src/objects/Monster.h"

class GameMMTScene : public QGraphicsScene
{
    Q_OBJECT

    enum class direction {
        up,
        lf,
        rg,
        dw,
    };

public:
    GameMMTScene(QObject *parent = nullptr);
    ~GameMMTScene();
    bool timerEnabled = false;

signals:
    void requestSceneChange(sceneslist scene);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // TICK
    QTimer *timer;
    long long int timeSec = 0;

    // MAP BUILDER
    void setup();
    QVector<QVector<QPointF>> mapPos;
    QVector<QVector<int>> mapObj;

    // PLAYER MANAGER
    void movingPlayer(direction dir);
    Player *player;     // 玩家物體
    int pX;             // 玩家位置Ｘ
    int pY;             // 玩家位置Ｙ
    int ipX;            // 待處理的玩家位置Ｘ
    int ipY;            // 待處理的玩家位置Ｙ
    int waterballMaxCount = 1; // 可施放水球最大數
    int waterballCurCount = 0; // 目前水球在場上
    int waterballPower = 1;    // 水球威力（格，最大 5）
    bool itemSpeed = false; // speed
    bool itemTurtle = false; // turtle
    bool itemNeedle = false; //  Needle
    int gloveTime = 0; // Glove
    int moonTime = 0; // moon time
    int initX = 0;
    int initY = 0;
    int life = 3;

    void movingMaBrick(direction way, MaBrick *mb);

    // WATERBALLOON
    void puttingWaterballoon(int cellX, int cellY);
    void waterballoonExplosion(WaterBalloon *wb);
    int isPlayerBlocked = 0;

    // ITEMS
    void generateItem(int x, int y);
    QGraphicsTextItem *textWater;
    QGraphicsTextItem *textPower;
    QGraphicsTextItem *textSpeed;
    QGraphicsTextItem *textTurtle;
    QGraphicsTextItem *textNeedle;
    QGraphicsTextItem *textGlove;
    QGraphicsTextItem *textMoon;
    void updateTextInfo();

    // UI ITEM
    QGraphicsPixmapItem *uiHome;
    QGraphicsPixmapItem *uiPause;
    QGraphicsTextItem *uiPauseIndictator;
    QGraphicsTextItem *uiLife;

    // MONSTER
    void movingMonster(Monster *mn);

private slots:
    void tick();
};

#endif // GameMMTScene_H
