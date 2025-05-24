#ifndef MABRICK_H
#define MABRICK_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class MaBrick : public QGraphicsPixmapItem
{
public:
    explicit MaBrick(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
};

#endif // MABRICK_H
