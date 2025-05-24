#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    // 建構與解構
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private:
    QGraphicsView *view;
};

#endif
