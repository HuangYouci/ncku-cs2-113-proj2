#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Player : public QGraphicsPixmapItem
{

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

public:
    Player();

    void kup();
    void kdw();
    void klf();
    void krg();

private:
    QPixmap fullimage;
    walkingStatus ws;
};

#endif // PLAYER_H
