#ifndef MABRICK_H
#define MABRICK_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class MaBrick : public QGraphicsPixmapItem
{
public:
    explicit MaBrick(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~MaBrick();

    int ipX = 0; // 等待移動的位置Ｘ
    int ipY = 0; // 等待移動的位置Ｙ
};

#endif // MABRICK_H
