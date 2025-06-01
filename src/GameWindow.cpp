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
    setWindowTitle("Crazy Arcade (Group 34, Introduction to Computer Science, 2025 Spring, NCKU EE)");
    setFixedSize(550, 550);

    // MARK: - Scene Manager
    // Title Creation & Connect Slots
    title = new TitleScene(this);
    connect(title, &TitleScene::requestSceneChange, this, &GameWindow::handleSwitchScene);
    connect(title, &TitleScene::requestQuitApplication, this, &GameWindow::handleQuitRequest);

    // GameEnd Creation & Connect Slots
    end = new EndingScene(this);
    connect(end, &EndingScene::requestSceneChange, this, &GameWindow::handleSwitchScene);
    connect(end, &EndingScene::requestQuitApplication, this, &GameWindow::handleQuitRequest);

    // Win
    win = new WiningScene(this);
    connect(win, &WiningScene::requestSceneChange, this, &GameWindow::handleSwitchScene);
    connect(win, &WiningScene::requestQuitApplication, this, &GameWindow::handleQuitRequest);

    // Switch Scene (Default)
    handleSwitchScene(sceneslist::title);
}

GameWindow::~GameWindow() {
    qDebug() << "[GameWindow] 已被解構";
}

void GameWindow::handleSwitchScene(sceneslist to){

    GameMRScene *tempMR = nullptr;
    GameMMScene *tempMM = nullptr;
    GameMMTScene *tempMMT = nullptr;
    GameMMBScene *tempMMB = nullptr;

    if (gameMR){
        tempMR = gameMR;
    }

    if(gameMM){
        tempMM = gameMM;
    }

    if(gameMMT){
        tempMMT = gameMMT;
    }

    if(gameMMB){
        tempMMB = gameMMB;
    }

    // Game Creation & Connect Slots
    gameMR = new GameMRScene(this);
    connect(gameMR, &GameMRScene::requestSceneChange, this, &GameWindow::handleSwitchScene);

    gameMM = new GameMMScene(this);
    connect(gameMM, &GameMMScene::requestSceneChange, this, &GameWindow::handleSwitchScene);

    gameMMT = new GameMMTScene(this);
    connect(gameMMT, &GameMMTScene::requestSceneChange, this, &GameWindow::handleSwitchScene);

    gameMMB = new GameMMBScene(this);
    connect(gameMMB, &GameMMBScene::requestSceneChange, this, &GameWindow::handleSwitchScene);


    switch(to){
    case sceneslist::gameMR:
        gameMR->timerEnabled = true;
        view->setScene(gameMR);
        break;
    case sceneslist::title:
        view->setScene(title);
        break;
    case sceneslist::ending:
        view->setScene(end);
        break;
    case sceneslist::gameMM:
        gameMM->timerEnabled = true;
        view->setScene(gameMM);
        break;
    case sceneslist::gameMMT:
        gameMMT->timerEnabled = true;
        view->setScene(gameMMT);
        break;
    case sceneslist::wining:
        view->setScene(win);
        break;
    case sceneslist::gameMMB:
        gameMMB->timerEnabled = true;
        view->setScene(gameMMB);
        break;
    }

    // Delete Old Scene
    if (tempMR){
        tempMR->deleteLater();
    }
    if (tempMM){
        tempMM->deleteLater();
    }
    if (tempMMT){
        tempMMT->deleteLater();
    }
    if (tempMMB){
        tempMMB->deleteLater();
    }
}

void GameWindow::handleQuitRequest() {
    // 在除錯視窗中顯示接收到退出請求
    qDebug() << "[GameWindow] Received request to quit program.";

    // 呼叫 Qt 的退出方法，結束應用程式
    QApplication::quit();
}

