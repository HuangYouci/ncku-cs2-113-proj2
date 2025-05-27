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
        rg,
        ce
    };

    WaterExplosion(QGraphicsItem *parent = nullptr, WaterExplosionDirection dir = WaterExplosionDirection::up);
    WaterExplosionDirection direction;

    int time = 100;

    void updateImage();

};

#endif // WATEREXPLOSION_H
