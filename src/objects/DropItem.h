#ifndef DROPITEM_H
#define DROPITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class DropItem : public QGraphicsPixmapItem
{
public:

    enum class DropItemType {
        waterballoon,
        glove,
        power_potion,
        speed_shoes,
        turtle,
        moonwalk,
        needle
    };

    DropItem(QGraphicsItem *parent = nullptr, DropItemType t = DropItemType::glove);
    DropItemType type;

};

#endif // DROPITEM_H
