#include "src/elements/ClickableButton.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

ClickableButton::ClickableButton(const QString &text, const QRectF &rect, Action action, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_rect(rect),
      m_text(text),
      m_action(action),
      m_isPressed(false),
      m_isHovered(false) {
    // 啟用懸停事件，這樣 hoverEnterEvent 和 hoverLeaveEvent 才會被調用
    setAcceptHoverEvents(true);
}

QRectF ClickableButton::boundingRect() const {
    // 返回按鈕的精確邊界
    return m_rect;
}

void ClickableButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // Q_UNUSED 用於避免未使用參數的編譯器警告
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor backgroundColor;
    QColor textColor = Qt::black; // 預設文字顏色

    if (m_isPressed) {
        backgroundColor = QColor(0, 100, 0); // 深綠色
        textColor = Qt::white;
    } else if (m_isHovered) {
        backgroundColor = QColor(144, 238, 144); // 淺綠色
    } else {
        backgroundColor = Qt::white; // 預設白色
    }

    // 繪製按鈕背景
    painter->setBrush(backgroundColor);
    painter->setPen(Qt::NoPen);
    // 繪製一個帶有小圓角的矩形
    painter->drawRoundedRect(m_rect, 10.0, 10.0); // 5.0 是圓角的 x 和 y 半徑

    // 繪製按鈕文字
    painter->setPen(textColor);
    // 在矩形 m_rect 內居中繪製文字 m_text
    painter->drawText(m_rect, Qt::AlignCenter, m_text);
}

void ClickableButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // 只響應滑鼠左鍵按下
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        // update() 會觸發重繪 (調用 paint())
        update();
        // event->accept() 表示此事件已被處理，不會再傳遞給其他 item 或 scene
        event->accept();
    } else {
        // 如果不是左鍵，則忽略此事件
        event->ignore();
    }
}

void ClickableButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    // 只響應滑鼠左鍵釋放，並且按鈕之前是處於按下狀態
    if (event->button() == Qt::LeftButton && m_isPressed) {
        bool wasPressed = m_isPressed;
        m_isPressed = false;
        update();

        // 檢查滑鼠釋放時是否仍在按鈕區域內
        // 並且確認是從按下狀態轉換來的（避免僅移入就觸發）
        if (wasPressed && m_rect.contains(event->pos())) {
            qDebug() << "[ClickableButton] '" << m_text << "' clicked, emitting action:" << static_cast<int>(m_action);
            // 發射 clicked 信號，並帶上此按鈕的動作類型
            emit clicked(m_action);
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void ClickableButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    // Q_UNUSED(event); // 如果事件參數未使用
    m_isHovered = true;
    update(); // 更新外觀以反映懸停狀態
    event->accept();
}

void ClickableButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    // Q_UNUSED(event); // 如果事件參數未使用
    m_isHovered = false;
    // 如果滑鼠移開時按鈕是按下的 (例如按住滑鼠拖出按鈕區域)，也應該重置按下狀態
    if (m_isPressed) {
        m_isPressed = false;
    }
    update(); // 更新外觀
    event->accept();
}
