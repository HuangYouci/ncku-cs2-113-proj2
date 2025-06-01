#ifndef MONSTER_H
#define MONSTER_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Monster : public QGraphicsPixmapItem
{
public:

    enum class walkingStatus {
        up01,
        up02,
        dw01,
        dw02,
        lf01,
        lf02,
        rg01,
        rg02
    };

    enum class destDir {
        up,
        dw,
        lf,
        rg
    };

    void up();
    void dw();
    void lf();
    void rg();

    Monster();

    int dieTimer = 0;
    walkingStatus ws = walkingStatus::dw01;
    destDir ds = destDir::up;

    int ipX = 0;
    int ipY = 0;

    bool mmtCounterClockWise = false;

private:
    QPixmap fullimage;
};

#endif // MONSTER_H
