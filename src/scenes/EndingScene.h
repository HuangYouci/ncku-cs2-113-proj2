#ifndef ENDINGSCENE_H
#define ENDINGSCENE_H

#include <QGraphicsScene>
#include <QObject>

#include <src/elements/ClickableButton.h>
#include <src/core/enums.h>

class EndingScene : public QGraphicsScene
{
    Q_OBJECT

public:
    EndingScene(QObject *parent = nullptr);
    ~EndingScene();

signals:
    void requestSceneChange(sceneslist scene);
    void requestQuitApplication();

private:
    void setup();

    void handleButtonClicked(ClickableButton::Action action);

};

#endif // ENDINGSCENE_H
