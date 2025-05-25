#ifndef WATEREXPLOSION_H
#define WATEREXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class WaterExplosion : public QGraphicsPixmapItem
{

public:


    enum class WaterExplosionDirection {
        uph,
        dwh,
        lfh,
        rgh,
        up,
        dw,
        lf,
        rg
    };

    WaterExplosion(QGraphicsItem *parent = nullptr, WaterExplosionDirection dir = WaterExplosionDirection::up);
    WaterExplosionDirection direction;

    int time = 200;

    void updateImage();

};

#endif // WATEREXPLOSION_H
