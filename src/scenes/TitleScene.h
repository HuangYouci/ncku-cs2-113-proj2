#ifndef TITLESCENE_H
#define TITLESCENE_H

#include <QGraphicsScene>
#include <QObject>

#include "src/elements/ClickableButton.h"
#include "src/core/enums.h"

class TitleScene : public QGraphicsScene {
    Q_OBJECT

public:
    TitleScene(QObject *parent = nullptr);
    ~TitleScene();

signals:
    // 當需要切換場景時發射此信號，參數是目標場景的枚舉值 (來自 enums.h)
    void requestSceneChange(sceneslist scene);

    // 當需要退出應用程式時發射此信號
    void requestQuitApplication();

private:
    void setup(); // 初始化場景內容

    // 槽函數，用於響應按鈕的 clicked 信號
    void handleButtonClicked(ClickableButton::Action action);
};

#endif // TITLESCENE_H
