#include "src/objects/Boss.h"

#include <QDebug>
#include <QTransform>

Boss::Boss()
{

    // Load Full Image
    fullimage = QPixmap(":/data/character/octupus.png");

    // Default Constructor
    setPixmap(fullimage.copy(30, 45, 50, 56));

    //
    oi1 = new QGraphicsPixmapItem(fullimage.copy(101, 38, 70, 73));
    oi2 = new QGraphicsPixmapItem(fullimage.copy(352, 76, 70, 34));
    oi1->setParentItem(this);
    oi2->setParentItem(this);
    oi1->setPos(-10, -25);
    oi2->setPos(-10, 30);
}


void Boss::kup(){
    oi1->setPixmap(fullimage.copy(21, 30, 70, 73));
    oi2->setPixmap(fullimage.copy(270, 76, 70, 34));
}

void Boss::kdw(){
    oi1->setPixmap(fullimage.copy(101, 38, 70, 73));
    oi2->setPixmap(fullimage.copy(352, 76, 70, 34));
}

void Boss::klf(){
    QTransform transform;
    transform.scale(-1, 1);
    transform.translate(-50, 0);
    oi1->setPixmap(fullimage.copy(183, 32, 70, 73).transformed(transform));
    oi2->setPixmap(fullimage.copy(435, 62, 70, 45).transformed(transform));
}

void Boss::krg(){
    oi1->setPixmap(fullimage.copy(183, 32, 70, 73));
    oi2->setPixmap(fullimage.copy(435, 62, 70, 45));
}
