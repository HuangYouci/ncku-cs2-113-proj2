#ifndef FXBRICK_H
#define FXBRICK_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class FxBrick : public QGraphicsPixmapItem
{
public:
    explicit FxBrick(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
};

#endif // FXBRICK_H
