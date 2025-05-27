#include "src/objects/DropItem.h"

DropItem::DropItem(QGraphicsItem *parent, DropItemType t) : QGraphicsPixmapItem(parent), type(t)
{
    // Constructor
    QPixmap waterballoon = QPixmap(":/data/item/extra_water_balloons.png");
    QPixmap glove = QPixmap(":/data/item/glove.png");
    QPixmap power_potion = QPixmap(":/data/item/power_potion.png");
    QPixmap speed_shoes = QPixmap(":/data/item/speed_shoes.png");
    QPixmap turtle = QPixmap(":/data/item/turtle.png");
    QPixmap moonwalk = QPixmap(":/data/item/moon_walk.png").scaled(40, 40);
    QPixmap needle = QPixmap(":/data/item/needle.png");

    switch(type){
    case DropItemType::waterballoon:
        setPixmap(waterballoon);
        break;
    case DropItemType::glove:
        setPixmap(glove);
        break;
    case DropItemType::power_potion:
        setPixmap(power_potion);
        break;
    case DropItemType::speed_shoes:
        setPixmap(speed_shoes);
        break;
    case DropItemType::turtle:
        setPixmap(turtle);
        break;
    case DropItemType::moonwalk:
        setPixmap(moonwalk);
        break;
    case DropItemType::needle:
        setPixmap(needle);
        break;
    }
}
