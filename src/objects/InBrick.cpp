#include "src/objects/InBrick.h"

InBrick::InBrick(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}
