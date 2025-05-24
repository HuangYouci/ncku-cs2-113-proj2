#include "src/GameWindow.h"

#include <QDebug>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent){
    qDebug() << "[GameWindow] 已被構建";

    view = new QGraphicsView(this);

    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();

    setWindowTitle("[Game] Crazy Arcade (Group 34, Introduction to Computer Science, 2025 Spring, NCKU EE)");
    setFixedSize(525, 450);
}

GameWindow::~GameWindow() {
    qDebug() << "[GameWindow] 已被解構";
}