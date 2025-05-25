#include "src/objects/WaterBalloon.h"

WaterBalloon::WaterBalloon(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    // Load Full Image
    QPixmap fullimage = QPixmap(":/data/water_ballon.png");
    setPixmap(fullimage.copy(24,35,50,50));         // 完整水球
}

void WaterBalloon::updateBallon(){
    // Load Full Image
    QPixmap fullimage = QPixmap(":/data/water_ballon.png");
    // 每 0.3 秒（30）一個循環，最後 0.3 秒播放爆炸
    if (boomgatime > 30){
        int cycle = boomgatime%30;
        if (cycle == 0){
            setPixmap(fullimage.copy(24,35,50,50));
        } else if (cycle == 10){
            setPixmap(fullimage.copy(77,35,50,50));
        } else if (cycle == 20){
            setPixmap(fullimage.copy(134,35,50,50));
        }
    } else {
        if (boomgatime == 30){
            setPixmap(fullimage.copy(28,114,40,40).scaled(50, 50));
        } else if (boomgatime == 20){
            setPixmap(fullimage.copy(85,114,40,40).scaled(50, 50));
        } else if (boomgatime == 10){
            setPixmap(fullimage.copy(144,114,40,40).scaled(50, 50));
        }
    }
}
