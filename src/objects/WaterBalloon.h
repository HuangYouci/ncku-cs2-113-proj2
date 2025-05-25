#ifndef WATERBALLOON_H
#define WATERBALLOON_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class WaterBalloon : public QGraphicsPixmapItem
{
public:
    WaterBalloon(QGraphicsItem *parent = nullptr);

    void updateBallon();  // 水球照片隨時間

    int boomgatime = 300; // 爆炸時間
};

#endif // WATERBALLOON_H
