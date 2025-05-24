#include "src/scenes/GameMRScene.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>            // 抵爸個
#include <QFile>             // 讀檔案
#include <QTextStream>       // 讀文字
#include <QStringList>       // 拆輸入字串
#include <QRandomGenerator>  // 亂數
#include <QList>             // 碰撞檢查

GameMRScene::GameMRScene(QObject *parent)
{
    qDebug() << "[GameMRScene] 已被建構";

    setup();
}

GameMRScene::~GameMRScene(){
    qDebug() << "[GameMRScene] 已被解構";
}

void GameMRScene::keyPressEvent(QKeyEvent *event){
    int dX = 0, dY = 0;
    switch(event->key()){
    case Qt::Key_Up:
        qDebug() << "[GameMRScene] 按了 ↑";
        dY -= 10;
        player->kup();
        break;
    case Qt::Key_Down:
        qDebug() << "[GameMRScene] 按了 ↓";
        dY += 10;
        player->kdw();
        break;
    case Qt::Key_Left:
        qDebug() << "[GameMRScene] 按了 ←";
        dX -= 10;
        player->klf();
        break;
    case Qt::Key_Right:
        qDebug() << "[GameMRScene] 按了 →";
        dX += 10;
        player->krg();
        break;
    }
    updatePlayer(dX, dY);
}

void GameMRScene::setup(){
    // 設定大小
    setSceneRect(0, 0, 550, 550);

    // MARK: - 設定 UI
    QGraphicsPixmapItem *menu = new QGraphicsPixmapItem(QPixmap(":/data/ui/ui.png").scaled(550, 100));
    menu->setPos(0, 450);
    addItem(menu);

    // MARK: - 設定地圖
    // 設定地圖座標組（用於放置位置）
    for (int row = 0; row < 9; ++row) {
        QVector<QPointF> rowVec;
        for (int col = 0; col < 11; ++col) {
            QPointF p(col * 50, row * 50);
            rowVec.append(p);
        }
        mapPos.append(rowVec);
    }

    // 讀地圖檔案
    QString filePath = ":/data/maps/testmap.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[GameMRScene] 無法開啟檔案：" << filePath;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        QStringList parts = line.split(' ');

        QVector<int> row;
        for (const QString &part : parts) {
            row.append(part.toInt());
        }
        mapObj.append(row);
    }

    file.close();

    qDebug() << "[GameMRScene] mapObj:";
    for (const QVector<int>& row : mapObj) {
        QString line;
        for (int val : row) {
            line += QString::number(val) + " ";
        }
        qDebug().noquote() << line.trimmed();
    }

    // 建構地圖
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 11; ++col) {
            int val = mapObj[row][col];

            // 地板｜地板都會建立
            QGraphicsPixmapItem *floor = new QGraphicsPixmapItem(QPixmap(":/data/brick/floor.png").scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            floor->setPos(col * 50, row * 50);  // col 是 x, row 是 y
            addItem(floor);

            // 依照地圖元素建立
            // 0, 4, 5 不動
            switch(val){
            case 1:
                {
                MaBrick *brick = new MaBrick(QPixmap(":/data/brick/movable_destructible.png").scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                brick->setPos(col * 50, row * 50);
                addItem(brick);
                break;
                }
            case 2:
                {
                FxBrick *brick = new FxBrick(QPixmap(":/data/brick/destructible_fixed_brick.png").scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                brick->setPos(col * 50, row * 50);
                addItem(brick);
                break;
                }
            case 3:
                {
                QPixmap bp = QPixmap(":/data/brick/indestructible_brick_blue.png");
                if (QRandomGenerator::global()->bounded(2) == 0){ // 綠色 or 藍色
                    bp = QPixmap(":/data/brick/indestructible_brick_green.png");
                }
                InBrick *brick = new InBrick(bp.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                brick->setPos(col * 50, row * 50);
                addItem(brick);
                break;
                }
            }
        }
    }

    // MARK: - 建構玩家
    player = new Player;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 11; ++col) {
            int val = mapObj[row][col];
            if (val == 4){
                // 設定玩家起始位置
                pX = col*50;
                pY = row*50;
                updatePlayer(0, 0);
                qDebug() << "[GameMRScene] 玩家生成於 (" << row*50 << "," << col*50 << ")";
                break;
            }
        }
    }
    addItem(player);
}

void GameMRScene::updatePlayer(int dX, int dY){

    int nextX = pX + dX;
    int nextY = pY + dY;

    // 1. 邊界檢查
    if (nextX > 500 || nextX < 0 || nextY > 400 || nextY < 0){
        qDebug() << "[GameMRScene] Player 無法走路，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";
        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(nextX, nextY, 50, 50); // 應該為 56, 50 但弄小塊一點
    QList<QGraphicsItem *> itemAtNext = items(nextRect); // 用 items 查看下一個位置有哪些區塊
    for (QGraphicsItem *item : itemAtNext) {
        // 檢查有無碰撞到以下方塊，確認要阻止或進行功能
        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) ){
            // 2-1. InBrick、FxBrick 不能移動的方塊
            qDebug() << "[GameMRScene] Player 碰到 In, Fx 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
            return;
        }
        if (MaBrick *mb = dynamic_cast<MaBrick *>(item)){
            // 前面只需檢查，現在需要將結果指標存到 MaBrick *mb 中
            // 2-2. MaBrick 可移動方塊，進行移動方塊判斷。但仍不能動
            qDebug() << "[GameMRScene] Player 碰到 Ma 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
            if (dX > 0){
                movingMaBrick(movingMaBrickWays::rg, mb);
                return;
            }
            if (dX < 0){
                movingMaBrick(movingMaBrickWays::lf, mb);
                return;
            }
            if (dY > 0){
                movingMaBrick(movingMaBrickWays::dw, mb);
                return;
            }
            if (dY < 0){
                movingMaBrick(movingMaBrickWays::up, mb);
                return;
            }
            qDebug() << "[GameMRScene] 未知的 MaBrick 移動方位";
            return;
        }
    }

    // Set Position
    pX += dX;
    pY += dY;
    qDebug() << "[GameMRScene] Player 成功走路並走到 (" << pX << "," << pY << ")";
    player->setPos(pX,pY);
}

void GameMRScene::movingMaBrick(movingMaBrickWays way, MaBrick *mb){
    int dX = 0, dY = 0;

    switch(way){
    case movingMaBrickWays::up:
        dY = -50;
        break;
    case movingMaBrickWays::dw:
        dY = 50;
        break;
    case movingMaBrickWays::lf:
        dX = -50;
        break;
    case movingMaBrickWays::rg:
        dX = 50;
        break;
    }

    qreal pX = mb->pos().x();
    qreal pY = mb->pos().y();

    int nextX = pX + dX;
    int nextY = pY + dY;

    qDebug() << "[GameMRScene] MaBrick 目前在 (" << pX << "," << pY << ")。嘗試將他走 (" << dX << "," << dY << ")";

    // 1. 邊界檢查
    if (nextX > 500 || nextX < 0 || nextY > 400 || nextY < 0){
        qDebug() << "[GameMRScene] MaBrick 無法移動，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";
        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(pX + dX, pY + dY, 50, 50);
    QList<QGraphicsItem *> itemAtNext = items(nextRect);
    for (QGraphicsItem *item : itemAtNext) {
        // MaBrick 移動的碰撞檢測
        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item)){
            // 2-1. InBrick、FxBrick、MaBrick 阻擋
            qDebug() << "[GameMRScene] MaBrick 碰到 In, Fx, Ma 磚塊，無法移動到 (" << pX+dX << "," << pY+dY << ")";
            return;
        }
        // TODO: 未來可能需要判定其他物品
    }

    // 移動成功
    mb->setPos(nextX, nextY);
    qDebug() << "[GameMRScene] MaBrick 移動成功！移動到 (" << nextX << "," << nextY << ")";
}
