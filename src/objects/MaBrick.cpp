#include "src/objects/MaBrick.h"
#include <QDebug>

MaBrick::MaBrick(const QPixmap &pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap, parent)
{
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

MaBrick::~MaBrick(){
//    qDebug() << "[MaBrick] 被刪除";
}
