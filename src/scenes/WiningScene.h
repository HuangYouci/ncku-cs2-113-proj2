#ifndef winingSCENE_H
#define winingSCENE_H

#include <QGraphicsScene>
#include <QObject>

#include <src/elements/ClickableButton.h>
#include <src/core/enums.h>

class WiningScene : public QGraphicsScene
{
    Q_OBJECT

public:
    WiningScene(QObject *parent = nullptr);
    ~WiningScene();

signals:
    void requestSceneChange(sceneslist scene);
    void requestQuitApplication();

private:
    void setup();

    void handleButtonClicked(ClickableButton::Action action);

};

#endif // winingSCENE_H
