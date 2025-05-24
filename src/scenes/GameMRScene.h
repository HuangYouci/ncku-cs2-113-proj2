#ifndef GAMEMRSCENE_H
#define GAMEMRSCENE_H

#include <QGraphicsScene> // 場景建立
#include <QObject>        // 連接槽
#include <QVector>        // 動態陣列
#include <QPointF>        // 座標
#include <QKeyEvent>      // 鍵盤

#include "src/core/enums.h"
#include "src/objects/InBrick.h"
#include "src/objects/MaBrick.h"
#include "src/objects/FxBrick.h"
#include "src/objects/Player.h"

class GameMRScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GameMRScene(QObject *parent = nullptr);
    ~GameMRScene();

signals:
    void requestSceneChange(sceneslist scene);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    // MAP BUILDER
    void setup();
    QVector<QVector<QPointF>> mapPos;
    QVector<QVector<int>> mapObj;

    // PLAYER MANAGER
    void updatePlayer(int dX, int dY);
    Player *player;
    int pX;
    int pY;

    // MOVING BOX (MABRICK)
    enum class movingMaBrickWays {
        up,
        lf,
        rg,
        dw,
    };

    void movingMaBrick(movingMaBrickWays way, MaBrick *mb);
};

#endif // GAMEMRSCENE_H
