#ifndef GAMEMRSCENE_H
#define GAMEMRSCENE_H

#include <QGraphicsScene> // 場景建立
#include <QObject>        // 連接槽
#include <QVector>        // 動態陣列
#include <QPointF>        // 座標
#include <QKeyEvent>      // 鍵盤
#include <QTimer>         // Tick

#include "src/core/enums.h"
#include "src/objects/InBrick.h"
#include "src/objects/MaBrick.h"
#include "src/objects/FxBrick.h"
#include "src/objects/Player.h"
#include "src/objects/WaterBalloon.h"
#include "src/objects/WaterExplosion.h"

class GameMRScene : public QGraphicsScene
{
    Q_OBJECT

    enum class direction {
        up,
        lf,
        rg,
        dw,
    };

public:
    GameMRScene(QObject *parent = nullptr);
    ~GameMRScene();

signals:
    void requestSceneChange(sceneslist scene);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    // TICK
    QTimer *timer;
    bool timerEnabled = false;

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
    int waterballMaxCount = 2; // 可施放水球最大數
    int waterballCurCount = 0; // 目前水球在場上
    int waterballPower = 5;    // 水球威力（格，最大 5）

    void movingMaBrick(direction way, MaBrick *mb);

    // WATERBALLOON
    void puttingWaterballoon(int cellX, int cellY);
    void waterballoonExplosion(WaterBalloon *wb);

private slots:
    void tick();
};

#endif // GAMEMRSCENE_H
