#ifndef INBRICK_H
#define INBRICK_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class InBrick : public QGraphicsPixmapItem
{
public:
    explicit InBrick(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
};

#endif // INBRICK_H
