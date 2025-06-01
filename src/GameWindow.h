#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

// Scenes
#include "src/scenes/TitleScene.h"
#include "src/scenes/GameMRScene.h"
#include "src/scenes/EndingScene.h"
#include "src/scenes/GameMMScene.h"

// Core
#include "src/core/enums.h"

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    // 建構與解構
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    // 場景切換曹函數
    void handleSwitchScene(sceneslist to);

    // 退出程式曹函數
    void handleQuitRequest();

private:
    // Main View
    QGraphicsView *view;

    // Scenes
    sceneslist currentScene = sceneslist::title;
    TitleScene *title = nullptr;
    GameMRScene *gameMR = nullptr;
    EndingScene *end = nullptr;
    GameMMScene *gameMM = nullptr;
};

#endif
