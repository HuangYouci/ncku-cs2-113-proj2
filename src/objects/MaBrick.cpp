#include "src/objects/MaBrick.h"

MaBrick::MaBrick(const QPixmap &pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap, parent)
{
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}
