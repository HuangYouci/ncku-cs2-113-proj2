#include "src/objects/Robot.h"

#include <QDebug>
#include <QTransform>

Robot::Robot()
{

    // Load Full Image
    fullimage = QPixmap(":/data/character/robot.png");

    // Default Constructor
    setPixmap(fullimage.copy(138, 138, 50, 56));
}


void Robot::kup(){
    switch(ws){
    case walkingStatus::up01:
        setPixmap(fullimage.copy(65, 138, 50, 56));
        ws = Robot::walkingStatus::up02;
        break;
    case walkingStatus::up02:
        setPixmap(fullimage.copy(65, 209, 50, 56));
        ws = Robot::walkingStatus::up03;
        break;
    case walkingStatus::up03:
        setPixmap(fullimage.copy(65, 280, 50, 56));
        ws = Robot::walkingStatus::up04;
        break;
    case walkingStatus::up04:
        setPixmap(fullimage.copy(65, 351, 50, 56));
        ws = Robot::walkingStatus::up01;
        break;
    default:
        setPixmap(fullimage.copy(65, 351, 50, 56));
        ws = Robot::walkingStatus::up01;
        break;
    }
}

void Robot::kdw(){
    switch(ws){
    case walkingStatus::dw01:
        setPixmap(fullimage.copy(138, 138, 50, 56));
        ws = Robot::walkingStatus::dw02;
        break;
    case walkingStatus::dw02:
        setPixmap(fullimage.copy(138, 209, 50, 56));
        ws = Robot::walkingStatus::dw03;
        break;
    case walkingStatus::dw03:
        setPixmap(fullimage.copy(138, 280, 50, 56));
        ws = Robot::walkingStatus::dw04;
        break;
    case walkingStatus::dw04:
        setPixmap(fullimage.copy(138, 351, 50, 56));
        ws = Robot::walkingStatus::dw01;
        break;
    default:
        setPixmap(fullimage.copy(138, 351, 50, 56));
        ws = Robot::walkingStatus::dw01;
        break;
    }
}

void Robot::klf(){
    QTransform transform;
    transform.scale(-1, 1);
    transform.translate(-50, 0);
    switch(ws){
    case walkingStatus::rg01:
        setPixmap(fullimage.copy(261, 138, 50, 56).transformed(transform));
        ws = Robot::walkingStatus::rg02;
        break;
    case walkingStatus::rg02:
        setPixmap(fullimage.copy(331, 138, 50, 56).transformed(transform));
        ws = Robot::walkingStatus::rg03;
        break;
    case walkingStatus::rg03:
        setPixmap(fullimage.copy(402, 138, 50, 56).transformed(transform));
        ws = Robot::walkingStatus::rg04;
        break;
    case walkingStatus::rg04:
        setPixmap(fullimage.copy(472, 138, 50, 56).transformed(transform));
        ws = Robot::walkingStatus::rg01;
        break;
    default:
        setPixmap(fullimage.copy(261, 138, 50, 56).transformed(transform));
        ws = Robot::walkingStatus::rg01;
        break;
    }
}

void Robot::krg(){
    switch(ws){
    case walkingStatus::rg01:
        setPixmap(fullimage.copy(261, 138, 50, 56));
        ws = Robot::walkingStatus::rg02;
        break;
    case walkingStatus::rg02:
        setPixmap(fullimage.copy(331, 138, 50, 56));
        ws = Robot::walkingStatus::rg03;
        break;
    case walkingStatus::rg03:
        setPixmap(fullimage.copy(402, 138, 50, 56));
        ws = Robot::walkingStatus::rg04;
        break;
    case walkingStatus::rg04:
        setPixmap(fullimage.copy(472, 138, 50, 56));
        ws = Robot::walkingStatus::rg01;
        break;
    default:
        setPixmap(fullimage.copy(261, 138, 50, 56));
        ws = Robot::walkingStatus::rg01;
        break;
    }
}
