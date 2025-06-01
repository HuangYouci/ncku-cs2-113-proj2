#include "src/objects/Player.h"

#include <QDebug>
#include <QTransform> // 為了左右翻轉圖片

Player::Player(){
    // Load Full Image
    fullimage = QPixmap(":/data/character/palyer.png");

    // Default Constructor
    setPixmap(fullimage.copy(80, 138, 50, 56));
    overlayItem = new QGraphicsPixmapItem(fullimage.copy(150, 56, 50, 40));
    overlayItem->setParentItem(this);
    overlayItem->setPos(0, -30);
    overlayItem->setZValue(1);
    overlayItem->setVisible(false);
}

void Player::kup(){
    if (isBlocked) {
        return;
    }
    if (turtleEnabled){
        setPixmap(fullimage.copy(80,717,50,56));
        overlayItem->setPixmap(fullimage.copy(79, 56, 50, 40));
        overlayItem->setVisible(true);
    } else {
        overlayItem->setVisible(false);
        switch(ws){
        case walkingStatus::up01:
            setPixmap(fullimage.copy(80, 138, 50, 56));
            ws = Player::walkingStatus::up02;
            break;
        case walkingStatus::up02:
            setPixmap(fullimage.copy(80, 209, 50, 56));
            ws = Player::walkingStatus::up03;
            break;
        case walkingStatus::up03:
            setPixmap(fullimage.copy(80, 280, 50, 56));
            ws = Player::walkingStatus::up04;
            break;
        case walkingStatus::up04:
            setPixmap(fullimage.copy(80, 351, 50, 56));
            ws = Player::walkingStatus::up01;
            break;
        default:
            setPixmap(fullimage.copy(80, 351, 50, 56));
            ws = Player::walkingStatus::up01;
            break;
        }
    }
}

void Player::kdw(){
    if (isBlocked){
        return;
    }
    if (turtleEnabled){
        setPixmap(fullimage.copy(150,717,50,56));
        overlayItem->setPixmap(fullimage.copy(150, 56, 50, 40));
        overlayItem->setVisible(true);
    } else {
        overlayItem->setVisible(false);
        switch(ws){
        case walkingStatus::dw01:
            setPixmap(fullimage.copy(150, 138, 50, 56));
            ws = Player::walkingStatus::dw02;
            break;
        case walkingStatus::dw02:
            setPixmap(fullimage.copy(150, 209, 50, 56));
            ws = Player::walkingStatus::dw03;
            break;
        case walkingStatus::dw03:
            setPixmap(fullimage.copy(150, 280, 50, 56));
            ws = Player::walkingStatus::dw04;
            break;
        case walkingStatus::dw04:
            setPixmap(fullimage.copy(150, 351, 50, 56));
            ws = Player::walkingStatus::dw01;
            break;
        default:
            setPixmap(fullimage.copy(150, 351, 50, 56));
            ws = Player::walkingStatus::dw01;
            break;
        }
    }
}

void Player::klf(){
    if (isBlocked){
        return;
    }
    QTransform transform;
    transform.scale(-1, 1);
    transform.translate(-50, 0);
    if (turtleEnabled){
        setPixmap(fullimage.copy(292,714,50,56).transformed(transform));
        overlayItem->setPixmap(fullimage.copy(217, 56, 50, 40).transformed(transform));
        overlayItem->setVisible(true);
    } else {
        overlayItem->setVisible(false);
        switch(ws){
        case walkingStatus::lf01:
            setPixmap(fullimage.copy(220, 207, 50, 56).transformed(transform));
            ws = Player::walkingStatus::lf02;
            break;
        case walkingStatus::lf02:
            setPixmap(fullimage.copy(292, 207, 50, 56).transformed(transform));
            ws = Player::walkingStatus::lf03;
            break;
        case walkingStatus::lf03:
            setPixmap(fullimage.copy(362, 207, 50, 56).transformed(transform));
            ws = Player::walkingStatus::lf04;
            break;
        case walkingStatus::lf04:
            setPixmap(fullimage.copy(434, 207, 50, 56).transformed(transform));
            ws = Player::walkingStatus::lf01;
            break;
        default:
            setPixmap(fullimage.copy(220, 207, 50, 56).transformed(transform));
            ws = Player::walkingStatus::lf01;
            break;
        }
    }
}

void Player::krg(){
    if (isBlocked){
        return;
    }
    if (turtleEnabled){
        setPixmap(fullimage.copy(292,714,50,56));
        overlayItem->setPixmap(fullimage.copy(217, 56, 50, 40));
        overlayItem->setVisible(true);
    } else {
        overlayItem->setVisible(false);
        switch(ws){
        case walkingStatus::rg01:
            setPixmap(fullimage.copy(220, 207, 50, 56));
            ws = Player::walkingStatus::rg02;
            break;
        case walkingStatus::rg02:
            setPixmap(fullimage.copy(292, 207, 50, 56));
            ws = Player::walkingStatus::rg03;
            break;
        case walkingStatus::rg03:
            setPixmap(fullimage.copy(362, 207, 50, 56));
            ws = Player::walkingStatus::rg04;
            break;
        case walkingStatus::rg04:
            setPixmap(fullimage.copy(434, 207, 50, 56));
            ws = Player::walkingStatus::rg01;
            break;
        default:
            setPixmap(fullimage.copy(220, 207, 50, 56));
            ws = Player::walkingStatus::rg01;
            break;
        }
    }
}

