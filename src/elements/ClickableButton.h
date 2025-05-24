#ifndef CLICKABLEBUTTON_H
#define CLICKABLEBUTTON_H

#include <QGraphicsObject>
#include <QString>
#include <QRectF>

// 前向聲明，避免不必要的頭文件包含
class QPainter;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;

class ClickableButton : public QGraphicsObject {
    Q_OBJECT

public:
    // 定義按鈕可能觸發的動作類型
    enum class Action {
        StartG1,
        StartG2,
        QuitGame
    };

    ClickableButton(const QString &text,
                    const QRectF &rect,
                    Action action = Action::QuitGame,
                    QGraphicsItem *parent = nullptr);

    // 返回 item 的邊界矩形，用於繪製和碰撞檢測
    QRectF boundingRect() const override;

    // 實際的繪製函數
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    // 當按鈕被有效點擊時發射此信號，參數 action 表示是哪個類型的動作被觸發
    void clicked(ClickableButton::Action action);

protected:
    // 覆寫 QGraphicsItem 的滑鼠事件處理函數
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    // 覆寫 QGraphicsItem 的懸停事件處理函數
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QRectF m_rect;          // 按鈕的矩形區域
    QString m_text;         // 按鈕上顯示的文字
    Action m_action;        // 此按鈕關聯的動作類型

    bool m_isPressed;       // 標記按鈕是否被按下
    bool m_isHovered;       // 標記滑鼠是否懸停在按鈕上
};

#endif // CLICKABLEBUTTON_H
