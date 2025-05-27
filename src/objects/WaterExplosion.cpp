#include "src/objects/WaterExplosion.h"

WaterExplosion::WaterExplosion(QGraphicsItem *parent, WaterExplosionDirection dir) : QGraphicsPixmapItem(parent), direction(dir)
{
    // Constructor
    QPixmap fullimage = QPixmap(":/data/water_ballon.png");

    // Set Pixmap Image Ac to Dir
    switch(direction){
    case WaterExplosionDirection::uph:
        setPixmap(fullimage.copy(29,186,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::dwh:
        setPixmap(fullimage.copy(29,259,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::lfh:
        setPixmap(fullimage.copy(276,259,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::rgh:
        setPixmap(fullimage.copy(276,185,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::up:
        setPixmap(fullimage.copy(29,344,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::dw:
        setPixmap(fullimage.copy(29,419,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::lf:
        setPixmap(fullimage.copy(272,419,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::rg:
        setPixmap(fullimage.copy(272,344,40,40).scaled(50, 50));
        break;
    case WaterExplosionDirection::ce:
        setPixmap(fullimage.copy(28,114,40,40).scaled(50, 50));
        break;
    }
}

void WaterExplosion::updateImage(){
    QPixmap fullimage = QPixmap(":/data/water_ballon.png");
    // Type A
    if(time < 20){
        if (time == 15){
            switch(direction){
                case WaterExplosionDirection::uph:
                case WaterExplosionDirection::up:
                    setPixmap(fullimage.copy(145,186,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::dwh:
                case WaterExplosionDirection::dw:
                    setPixmap(fullimage.copy(145,259,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::lfh:
                case WaterExplosionDirection::lf:
                    setPixmap(fullimage.copy(394,259,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::rgh:
                case WaterExplosionDirection::rg:
                    setPixmap(fullimage.copy(394,185,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::ce:
                    setPixmap(fullimage.copy(85,114,40,40).scaled(50, 50));
                    break;
            }
        } else if (time == 5) {
            switch(direction){
                case WaterExplosionDirection::uph:
                case WaterExplosionDirection::up:
                    setPixmap(fullimage.copy(202,186,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::dwh:
                case WaterExplosionDirection::dw:
                    setPixmap(fullimage.copy(202,259,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::lfh:
                case WaterExplosionDirection::lf:
                    setPixmap(fullimage.copy(434,259,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::rgh:
                case WaterExplosionDirection::rg:
                    setPixmap(fullimage.copy(434,185,40,40).scaled(50, 50));
                    break;
                case WaterExplosionDirection::ce:
                    setPixmap(fullimage.copy(85,114,40,40).scaled(50, 50));
                    break;
            }
        }
    } else if (time%20 == 0){
        switch(direction){
        case WaterExplosionDirection::uph:
            setPixmap(fullimage.copy(29,186,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::dwh:
            setPixmap(fullimage.copy(29,259,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::lfh:
            setPixmap(fullimage.copy(276,259,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::rgh:
            setPixmap(fullimage.copy(276,185,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::up:
            setPixmap(fullimage.copy(29,344,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::dw:
            setPixmap(fullimage.copy(29,419,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::lf:
            setPixmap(fullimage.copy(272,419,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::rg:
            setPixmap(fullimage.copy(272,344,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::ce:
            setPixmap(fullimage.copy(144,114,40,40).scaled(50, 50));
            break;
        }
    } else if (time%20 == 10){
        switch(direction){
        case WaterExplosionDirection::uph:
            setPixmap(fullimage.copy(87,186,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::dwh:
            setPixmap(fullimage.copy(87,259,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::lfh:
            setPixmap(fullimage.copy(335,259,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::rgh:
            setPixmap(fullimage.copy(335,185,40,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::up:
            setPixmap(fullimage.copy(29,344,40,20).scaled(50, 50));
            break;
        case WaterExplosionDirection::dw:
            setPixmap(fullimage.copy(29,419,40,20).scaled(50, 50));
            break;
        case WaterExplosionDirection::lf:
            setPixmap(fullimage.copy(272,419,20,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::rg:
            setPixmap(fullimage.copy(272,344,20,40).scaled(50, 50));
            break;
        case WaterExplosionDirection::ce:
            setPixmap(fullimage.copy(144,114,40,40).scaled(50, 50));
            break;
        }
    }
}
