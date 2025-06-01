#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Robot : public QGraphicsPixmapItem
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

    void kup();
    void kdw();
    void klf();
    void krg();

    walkingStatus ws;
    bool isBlocked = false;

    Robot();


private:
    QPixmap fullimage;
};

#endif // ROBOT_H
