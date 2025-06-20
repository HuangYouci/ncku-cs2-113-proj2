#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Player : public QGraphicsPixmapItem
{

public:

    enum class walkingStatus {
        up01,
        up02,
        up03,
        up04,
        dw01,
        dw02,
        dw03,
        dw04,
        lf01,
        lf02,
        lf03,
        lf04,
        rg01,
        rg02,
        rg03,
        rg04
    };


    Player();

    void kup();
    void kdw();
    void klf();
    void krg();

    bool turtleEnabled = false;
    walkingStatus ws;
    bool isBlocked = false;

private:
    QPixmap fullimage;
    QGraphicsPixmapItem *overlayItem;
};

#endif // PLAYER_H
