#include "src/GameWindow.h"

#include <QDebug>
#include <QApplication>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent){

    // MARK: - Construct
    qDebug() << "[GameWindow] 已被構建";

    // MARK: - Create View
    view = new QGraphicsView(this);

    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCentralWidget(view);
    setWindowTitle("[Game] Crazy Arcade (Group 34, Introduction to Computer Science, 2025 Spring, NCKU EE)");
    setFixedSize(550, 550);

    // MARK: - Scene Manager
    // Title Creation & Connect Slots
    title = new TitleScene(this);
    connect(title, &TitleScene::requestSceneChange, this, &GameWindow::handleSwitchScene);
    connect(title, &TitleScene::requestQuitApplication, this, &GameWindow::handleQuitRequest);

    // Game Creation & Connect Slots
    gameMR = new GameMRScene(this);
    connect(gameMR, &GameMRScene::requestSceneChange, this, &GameWindow::handleSwitchScene);

    // Switch Scene (Default)
    handleSwitchScene(sceneslist::title);
//    handleSwitchScene(sceneslist::title); // For Test
}

GameWindow::~GameWindow() {
    qDebug() << "[GameWindow] 已被解構";
}

void GameWindow::handleSwitchScene(sceneslist to){
    switch(to){
    case sceneslist::gameMR:
        view->setScene(gameMR);
        break;
    case sceneslist::title:
        view->setScene(title);
        break;
    }
}

void GameWindow::handleQuitRequest() {
    // 在除錯視窗中顯示接收到退出請求
    qDebug() << "[GameWindow] Received request to quit program.";

    // 呼叫 Qt 的退出方法，結束應用程式
    QApplication::quit();
}

