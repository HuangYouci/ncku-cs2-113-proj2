#include "src/objects/Monster.h"

#include <QDebug>

Monster::Monster()
{
    // Load Full Image
    fullimage = QPixmap(":/data/character/monster.png");

    // Default
    setPixmap(fullimage.copy(120, 6, 50, 50));
}

void Monster::up(){
    switch(ws){
    case Monster::walkingStatus::up01:
        setPixmap(fullimage.copy(65, 6, 50, 50));
        ws = Monster::walkingStatus::up02;
    default:
        setPixmap(fullimage.copy(10, 6, 50, 50));
        ws = Monster::walkingStatus::up01;
    }
}

void Monster::dw(){
    switch(ws){
    case Monster::walkingStatus::dw01:
        setPixmap(fullimage.copy(168, 6, 50, 50));
        ws = Monster::walkingStatus::dw02;
    default:
        setPixmap(fullimage.copy(118, 6, 50, 50));
        ws = Monster::walkingStatus::dw01;
    }
}

void Monster::lf(){
    switch(ws){
    case Monster::walkingStatus::lf01:
        setPixmap(fullimage.copy(275, 6, 50, 50));
        ws = Monster::walkingStatus::lf02;
    default:
        setPixmap(fullimage.copy(222, 6, 50, 50));
        ws = Monster::walkingStatus::lf01;
    }
}

void Monster::rg(){
    switch(ws){
    case Monster::walkingStatus::rg01:
        setPixmap(fullimage.copy(381, 6, 50, 50));
        ws = Monster::walkingStatus::rg02;
    default:
        setPixmap(fullimage.copy(329, 6, 50, 50));
        ws = Monster::walkingStatus::rg01;
    }
}
