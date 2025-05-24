#include "src/objects/FxBrick.h"

FxBrick::FxBrick(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}
