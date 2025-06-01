#include "src/scenes/GameMMScene.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>            // 抵爸個
#include <QFile>             // 讀檔案
#include <QTextStream>       // 讀文字
#include <QStringList>       // 拆輸入字串
#include <QRandomGenerator>  // 亂數
#include <QList>             // 碰撞檢查
#include <QGraphicsRectItem>

GameMMScene::GameMMScene(QObject *parent)
{
    qDebug() << "[GameMMScene] 已被構建";
    setup();
}

GameMMScene::~GameMMScene(){
    qDebug() << "[GameMMScene] 已被解構";

    // 關閉計時器
    timer->stop();
}

void GameMMScene::keyPressEvent(QKeyEvent *event){
    qDebug() << "";
    switch(event->key()){
    case Qt::Key_Up:
        qDebug() << "[GameMMScene] 按了 ↑";
        movingPlayer(direction::up);
        player->kup();
        break;
    case Qt::Key_Down:
        qDebug() << "[GameMMScene] 按了 ↓";
        movingPlayer(direction::dw);
        player->kdw();
        break;
    case Qt::Key_Left:
        qDebug() << "[GameMMScene] 按了 ←";
        movingPlayer(direction::lf);
        player->klf();
        break;
    case Qt::Key_Right:
        qDebug() << "[GameMMScene] 按了 →";
        movingPlayer(direction::rg);
        player->krg();
        break;
    case Qt::Key_Space:
        qDebug() << "[GameMMScene] 按了 Space";
        puttingWaterballoon(((pX+10)/50+1),((pY+10)/50+1));
        break;
    case Qt::Key_Shift:
        qDebug() << "[GameMMScene] 按了 Shift";
        if(!itemNeedle){
            qDebug() << "[GameMMScene] 沒有 needle，略過";
            return;
        }
        if(isPlayerBlocked > 0){
            qDebug() << "[GameMMScene] 成功逃脫";
            isPlayerBlocked = 0;

            QTransform transform;
            transform.scale(-1, 1);
            transform.translate(-50, 0);
            switch(player->ws){
            case Player::walkingStatus::up01:
            case Player::walkingStatus::up02:
            case Player::walkingStatus::up03:
            case Player::walkingStatus::up04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(80, 138, 50, 56));
                break;
            case Player::walkingStatus::dw01:
            case Player::walkingStatus::dw02:
            case Player::walkingStatus::dw03:
            case Player::walkingStatus::dw04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(150, 138, 50, 56));
                break;
            case Player::walkingStatus::lf01:
            case Player::walkingStatus::lf02:
            case Player::walkingStatus::lf03:
            case Player::walkingStatus::lf04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(220, 207, 50, 56).transformed(transform));
                break;
            case Player::walkingStatus::rg01:
            case Player::walkingStatus::rg02:
            case Player::walkingStatus::rg03:
            case Player::walkingStatus::rg04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(220, 207, 50, 56));
                break;
            }
        }
        break;
    }
}

void GameMMScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem *clickedItem = itemAt(event->scenePos(), QTransform());
    if (clickedItem == uiHome){
        qDebug() << "[GameMMScene] uiHome Clicked.";
        requestSceneChange(sceneslist::title);
    }
    if (clickedItem == uiPause){
        qDebug() << "[GameMMScene] uiPause Clicked";
        uiPauseIndictator->setPlainText("暫停中");
        timerEnabled = false;
    }
    if (clickedItem == uiPauseIndictator){
        qDebug() << "[GameMMScene] uiPauseIndictator Clicked";
        uiPauseIndictator->setPlainText("");
        timerEnabled = true;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GameMMScene::setup(){
    // 設定大小
    setSceneRect(0, 0, 550, 550);

    // MARK: - 設定 UI
    qDebug() << "[GameMMScene] 設定 UI Menu";
    QGraphicsPixmapItem *menu = new QGraphicsPixmapItem(QPixmap(":/data/ui/ui.png").scaled(550, 100));
    menu->setPos(0, 450);
    addItem(menu);

    qDebug() << "[GameMMScene] 正在建立顯示物品中心";
    QGraphicsRectItem *br = new QGraphicsRectItem(300, 450, 250, 95);
    br->setBrush(Qt::blue);
    addItem(br);

    qDebug() << "[GameMMScene] 正在建立 UI 主畫面與暫停";
    uiHome = new QGraphicsPixmapItem(QPixmap(":/data/ui/home.png"));
    uiHome->setPos(200, 500);
    addItem(uiHome);
    uiPause = new QGraphicsPixmapItem(QPixmap(":/data/ui/pause.png"));
    uiPause->setPos(240, 500);
    addItem(uiPause);
    uiPauseIndictator = new QGraphicsTextItem("");
    uiPauseIndictator->setFont(QFont("Arial", 20));
    uiPauseIndictator->setPos(120, 500);
    uiPauseIndictator->setDefaultTextColor(Qt::white);
    addItem(uiPauseIndictator);

    qDebug() << "[GameMMScene] 正在建立剩餘命數";
    uiLife = new QGraphicsTextItem("1");
    uiLife->setFont(QFont("Arial", 40));
    uiLife->setPos(70, 490);
    uiLife->setDefaultTextColor(Qt::white);
    addItem(uiLife);

    qDebug() << "[GameMMScene] 正在建立各種物體數量顯示";
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/extra_water_balloons.png").scaled(30, 30));
        pi->setPos(310, 450);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("水球      /5");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(340, 450);
        addItem(ti);
        textWater = new QGraphicsTextItem("1");
        textWater->setFont(QFont("Arial", 20));
        textWater->setPos(380, 450);
        addItem(textWater);
    }
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/power_potion.png").scaled(30, 30));
        pi->setPos(310, 480);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("威力      /5");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(340, 480);
        addItem(ti);
        textPower = new QGraphicsTextItem("1");
        textPower->setFont(QFont("Arial", 20));
        textPower->setPos(380, 480);
        addItem(textPower);
    }
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/speed_shoes.png").scaled(30, 30));
        pi->setPos(310, 510);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("加速");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(340, 510);
        addItem(ti);
        textSpeed = new QGraphicsTextItem("無");
        textSpeed->setFont(QFont("Arial", 20));
        textSpeed->setPos(380, 510);
        addItem(textSpeed);
    }
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/turtle.png").scaled(30, 30));
        pi->setPos(420, 450);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("烏龜");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(450, 450);
        addItem(ti);
        textTurtle = new QGraphicsTextItem("無");
        textTurtle->setFont(QFont("Arial", 20));
        textTurtle->setPos(490, 450);
        addItem(textTurtle);
    }
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/needle.png").scaled(30, 30));
        pi->setPos(420, 480);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("針");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(450, 480);
        addItem(ti);
        textNeedle = new QGraphicsTextItem("無");
        textNeedle->setFont(QFont("Arial", 20));
        textNeedle->setPos(490, 480);
        addItem(textNeedle);
    }
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/glove.png").scaled(30, 30));
        pi->setPos(420, 510);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("手套");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(450, 510);
        addItem(ti);
        textGlove = new QGraphicsTextItem("無");
        textGlove->setFont(QFont("Arial", 20));
        textGlove->setPos(490, 510);
        addItem(textGlove);
    }
    {
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem();
        pi->setPixmap(QPixmap(":/data/item/moon_walk.png").scaled(30, 30));
        pi->setPos(160, 450);
        addItem(pi);
        QGraphicsTextItem *ti = new QGraphicsTextItem("月亮走");
        ti->setFont(QFont("Arial", 15));
        ti->setDefaultTextColor(Qt::white);
        ti->setPos(190, 450);
        addItem(ti);
        textMoon = new QGraphicsTextItem("無");
        textMoon->setFont(QFont("Arial", 20));
        textMoon->setPos(250, 450);
        addItem(textMoon);
    }

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
    qDebug() << "[GameMMScene] 讀取地圖檔案";
    QString filePath = ":/data/maps/gameMM01.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[GameMMScene] 無法開啟檔案：" << filePath;
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

    qDebug() << "[GameMMScene] mapObj:";
    for (const QVector<int>& row : mapObj) {
        QString line;
        for (int val : row) {
            line += QString::number(val) + " ";
        }
        qDebug().noquote() << line.trimmed();
    }

    // 建構地圖
    qDebug() << "[GameMMScene] 建構地圖";
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 11; ++col) {
            int val = mapObj[row][col];

            // 地板｜地板都會建立
            QGraphicsPixmapItem *floor = new QGraphicsPixmapItem(QPixmap(":/data/brick/floor.png").scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            floor->setPos(col * 50, row * 50);  // col 是 x, row 是 y
            floor->setZValue(-1);
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

                // 9 怪物
            case 9:
                {
                    Monster *ms = new Monster();
                    ms->setPos(col*50, row*50);
                    addItem(ms);
                    if(col == 2){
                        ms->ds = Monster::destDir::rg;
                    } else {
                        ms->ds = Monster::destDir::lf;
                    }
                }
            }
        }
    }

    // MARK: - 建構玩家
    qDebug() << "[GameMMScene] 建立玩家";
    player = new Player;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 11; ++col) {
            int val = mapObj[row][col];
            if (val == 4){
                // 設定玩家起始位置
                pX = col*50;
                pY = row*50 - 6;
                player->setPos(pX, pY);
                initX = pX;
                initY = pY;
                qDebug() << "[GameMMScene] 玩家生成於 (" << row*50 << "," << col*50 << ")";
                break;
            }
        }
    }
    addItem(player);
    player->setZValue(100);

    // MARK: - 開啟 Tick
    qDebug() << "[GameMMScene] 建立 tick 計時器（0.01s）";
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameMMScene::tick);
    timer->start(10);
}

void GameMMScene::movingPlayer(direction dir){

    // 如果在爆炸中 忽略
    if(isPlayerBlocked>0){
        return;
    }

    // 判斷方位
    int dX = 0, dY = 0;
    switch(dir){
    case direction::up:
        dY = -50;
        break;
    case direction::dw:
        dY = 50;
        break;
    case direction::lf:
        dX = -50;
        break;
    case direction::rg:
        dX = 50;
        break;
    }

    // nextX, nextY 預測玩家下個移動位置並進行檢查
    int nextX = pX + dX;
    int nextY = pY + dY;

    // 1. 邊界檢查
    if ((nextX > 500) || (nextX < 0) || (nextY > 400) || (nextY < -10)){
        qDebug() << "[GameMMScene] Player 無法走路，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";

        // 1-1. 是否有太空走路
        if (moonTime > 0){
            qDebug() << "[GameMMScene] 有 moonwalk";
            if(nextX > 500){
                qDebug() << "[GameMMScene] 邊界走到最左邊";
                pX = -50;
                player->setPos(-50, pY);
                ipX = 50;
            }
            if(nextX < 0){
                qDebug() << "[GameMMScene] 邊界走到最左邊";
                pX = 550;
                player->setPos(550, pY);
                ipX = -50;
            }
            if(nextY > 400){
                qDebug() << "[GameMMScene] 邊界走到最左邊";
                pY = -56;
                player->setPos(pX, -56);
                ipY = 50;
            }
            if(nextY < -10){
                qDebug() << "[GameMMScene] 邊界走到最左邊";
                pY = 450;
                player->setPos(pX, 450);
                ipY = -50;
            }
        }
        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(nextX, nextY+10, 35, 35); // 應該為 56, 50 但弄小塊一點
    QList<QGraphicsItem *> itemAtNext = items(nextRect); // 用 items 查看下一個位置有哪些區塊
    for (QGraphicsItem *item : itemAtNext) {
        // 檢查有無碰撞到以下方塊，確認要阻止或進行功能
        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<WaterBalloon*>(item)){
            // 2-1. InBrick、FxBrick 不能移動的方塊
            qDebug() << "[GameMMScene] Player 碰到 In, Fx 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
            return;
        }
        if (MaBrick *mb = dynamic_cast<MaBrick *>(item)){
            // 前面只需檢查，現在需要將結果指標存到 MaBrick *mb 中
            // 2-2. MaBrick 可移動方塊，進行移動方塊判斷。但仍不能動
            qDebug() << "[GameMMScene] Player 碰到 Ma 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
            if (dX > 0){
                movingMaBrick(direction::rg, mb);
                return;
            }
            if (dX < 0){
                movingMaBrick(direction::lf, mb);
                return;
            }
            if (dY > 0){
                movingMaBrick(direction::dw, mb);
                return;
            }
            if (dY < 0){
                movingMaBrick(direction::up, mb);
                return;
            }
            qDebug() << "[GameMMScene] 未知的 MaBrick 移動方位";
            return;
        }
    }

    // 將移動距離加入待處理（如果還可以走的話）
    if(ipX+ipY == 0){
        ipX = dX;
        ipY = dY;
        qDebug() << "[GameMMScene] 玩家成功觸發走路，將 (" << ipX << "," << ipY << ") 加入待處理序列";
        qDebug() << "[GameMMScene] Player | (" << ipX << "," << ipY << ")";
    } else {
        qDebug() << "[GameMMScene] 移動尚在冷卻期，忽略";
        return;
    }
}

void GameMMScene::movingMaBrick(direction way, MaBrick *mb){

    // 0. 如果玩家還在走路，就不要推
    if (ipX + ipY > 0){
        qDebug() << "[GameMMScene] 玩家仍在走路，跳過推動";
        return;
    }

    int dX = 0;
    int dY = 0;

    switch(way){
    case direction::up:
        dY = -50;
        break;
    case direction::dw:
        dY = 50;
        break;
    case direction::lf:
        dX = -50;
        break;
    case direction::rg:
        dX = 50;
        break;
    }


    qreal pX = mb->pos().x();
    qreal pY = mb->pos().y();

    int nextX = pX + dX;
    int nextY = pY + dY;

    qDebug() << "[GameMMScene] MaBrick 目前在 (" << pX << "," << pY << ")。嘗試將他走 (" << dX << "," << dY << ")";

    // 1. 邊界檢查
    if ((nextX > 500) || (nextX < 0) || (nextY > 400) || (nextY < 0)){
        qDebug() << "[GameMMScene] MaBrick 無法移動，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";
        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(pX + dX, pY + dY, 50, 50);
    QList<QGraphicsItem *> itemAtNext = items(nextRect);
    for (QGraphicsItem *item : itemAtNext) {
        // MaBrick 移動的碰撞檢測
        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<Player *>(item)){
            // 2-1. InBrick、FxBrick、MaBrick 阻擋
            qDebug() << "[GameMMScene] MaBrick 碰到 In, Fx, Ma 磚塊，無法移動到 (" << pX+dX << "," << pY+dY << ")";
            return;
        }
        // TODO: 未來可能需要判定其他物品
    }

    // 移動成功（加入 mb 佇列，在 tick 執行）
    if(mb->ipX + mb->ipY == 0){
        mb->ipX = dX;
        mb->ipY = dY;
        qDebug() << "[GameMMScene] 成功處理 MaBrick 移動，移動 (" << mb->ipX << "," << mb->ipY << ") 已加入佇列";
    } else {
        qDebug() << "[GameMMScene] 處理 MaBrick 失敗，因為仍有佇列";
    }
}

// MARK: - WaterBalloon
void GameMMScene::puttingWaterballoon(int cellX, int cellY){
    // 應檢查水球數量
    if (waterballCurCount >= waterballMaxCount){
        qDebug() << "[GameMMScene] 目前場上 Player 水球已達最大值" << waterballCurCount << "/" << waterballMaxCount;
        return;
    } else {
        qDebug() << "[GameMMScene] 放置之前場上 Player 水球為" << waterballCurCount << "/" << waterballMaxCount;
    }

    // 根據方塊的座標來檢查碰撞
    QRectF nextRect((cellX-1)*50, (cellY-1)*50, 50, 50);
    QList<QGraphicsItem *> itemAtNext = items(nextRect);
    for (QGraphicsItem *item : itemAtNext){
       // 放水球位置是否為空（不能有 InBrick、FxBrick、MaBrick、WaterBalloon)
       if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<WaterBalloon *>(item)){
           qDebug() << "[GameMMScene] 無法在 (" << cellX << "," << cellY << ") 放置水球，因為有物品阻擋";
           return;
       }
    }

    // 放置水球
    waterballCurCount++;
    WaterBalloon *waterballoon = new WaterBalloon();
    qDebug() << "[GameMMScene] 已在 (" << cellX << "," << cellY << ") 放置水球，目前場上 Player 水球為" << waterballCurCount << "/" << waterballMaxCount;
    waterballoon->setPos((cellX-1)*50, (cellY-1)*50);
    addItem(waterballoon);

    waterballoon->flyingX = 0;
    waterballoon->flyingY = 0;

    if(gloveTime > 0){
        qDebug() << "[GameMMScene] 手套效果已套用！";

        switch(player->ws){
        case Player::walkingStatus::up01:
        case Player::walkingStatus::up02:
        case Player::walkingStatus::up03:
        case Player::walkingStatus::up04:
            waterballoon->flyingY = -100;
            qDebug() << "[GameMMScene] 手套 往上丟！";
            break;
        case Player::walkingStatus::dw01:
        case Player::walkingStatus::dw02:
        case Player::walkingStatus::dw03:
        case Player::walkingStatus::dw04:
            waterballoon->flyingY = 100;
            qDebug() << "[GameMMScene] 手套 往下丟！";
            break;
        case Player::walkingStatus::lf01:
        case Player::walkingStatus::lf02:
        case Player::walkingStatus::lf03:
        case Player::walkingStatus::lf04:
            waterballoon->flyingX = -100;
            qDebug() << "[GameMMScene] 手套 往左丟！";
            break;
        case Player::walkingStatus::rg01:
        case Player::walkingStatus::rg02:
        case Player::walkingStatus::rg03:
        case Player::walkingStatus::rg04:
            waterballoon->flyingX = 100;
            qDebug() << "[GameMMScene] 手套 往右丟！";
            break;
        }
        gloveTime--;
    }
}

void GameMMScene::waterballoonExplosion(WaterBalloon *wb){
    // 1. 場上水球減少
    waterballCurCount--;
    qDebug() << "[GameMMScene] 有水球爆炸，場上 Player 水球剩餘" << waterballCurCount << "/" << waterballMaxCount;

    // 2. 紀錄爆炸座標
    int cellX = wb->pos().x()/50, cellY = wb->pos().y()/50;
    qDebug() << "[GameMMScene] 該水球爆炸於 (" << cellX+1 << "," << cellY+1 << ")";

    // 3. 正忠方位爆炸（放在正中間，無需檢測）
    WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::ce);
    addItem(we);
    we->setPos(cellX*50, cellY*50);

    // 4. 四方位爆炸，包含檢測：cellX*50, cellY*50, 50, 50 檢測
    QVector<QPoint> checklist;
    for(int i = 1 ; i <= waterballPower ; i++ ){
        checklist.append(QPoint(-i+cellX, cellY)); // 左
        checklist.append(QPoint(i+cellX, cellY)); // 右
        checklist.append(QPoint(cellX, -i+cellY)); // 上
        checklist.append(QPoint(cellX, i+cellY)); // 下
    }

    int putcount = waterballPower*4; // 在3,2,1,0 時是head（先減，%4 3左 2右 1上 0下）
    bool passCollf = true;
    bool passColrg = true;
    bool passColup = true;
    bool passColdw = true;
    for (QPoint &pt : checklist){
        // 4-1. 是否通過碰撞檢測 以及扣除放置個數
        bool passCol = true;
        putcount--;
        // 4-2. 邊界檢測
        if ( (pt.x()+1 > 11) || (pt.x()+1 < 0) || (pt.y()+1 > 9) || (pt.y()+1 < 0) ){
            qDebug() << "[GameMMScene] 在 (" << cellX+1 << "," << cellY+1 << ") 的水球爆炸波超過邊界，不放置";
            continue;
        }
        // 4-3. 碰撞檢測，每筆獨立
        QRectF nextRect(pt.x() *50, pt.y() *50, 50, 50);
        QList<QGraphicsItem *> itemAtNext = items(nextRect);
        for (QGraphicsItem *item : itemAtNext){
           // 放水球位置是否為空（不能有 InBrick、FxBrick、MaBrick)
           if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item)){
               qDebug() << "[GameMMScene] 無法在 (" << pt.x()+1 << "," << pt.y()+1 << ") 放置水球爆炸波，因為有物品阻擋";
               passCol = false;
           }
           // 連鎖反應（偵測到 WaterBalloon 讓他也一起爆炸），免 Return
           if (WaterBalloon *wb = dynamic_cast<WaterBalloon *>(item)){
               qDebug() << "[GameMMScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 發現其餘水球，迫使他爆炸";
               wb->boomgatime = 30;
           }
        }
        // 4-4. 如果碰撞檢測失敗，就跳過這個座標，順便禁止該方向之後的所有爆炸
        if (!passCol){
            QRect prevRect;
            QList<QGraphicsItem *> prevList;
            switch(putcount%4){
            case 3: //left
                qDebug() << "[GameMMScene] 左爆炸已被禁止";
                passCollf = false;
                // 要偵測上一個 把它變成頭
                prevRect = QRect(pt.x()*50 + 50, pt.y()*50, 50, 50);
                prevList = items(prevRect);
                for (QGraphicsItem *prevItem : prevList){
                    if(WaterExplosion *we = dynamic_cast<WaterExplosion *>(prevItem)){
                        if (we->direction == WaterExplosion::WaterExplosionDirection::lf){
                            we->direction = WaterExplosion::WaterExplosionDirection::lfh;
                        }
                    }
                }
                break;
            case 2: // right
                qDebug() << "[GameMMScene] 右爆炸已被禁止";
                passColrg = false;
                prevRect = QRect(pt.x()*50 - 50, pt.y()*50, 50, 50);
                prevList = items(prevRect);
                for (QGraphicsItem *prevItem : prevList){
                    if(WaterExplosion *we = dynamic_cast<WaterExplosion *>(prevItem)){
                        if (we->direction == WaterExplosion::WaterExplosionDirection::rg){
                            we->direction = WaterExplosion::WaterExplosionDirection::rgh;
                        }
                    }
                }
                break;
            case 1: // up
                qDebug() << "[GameMMScene] 上爆炸已被禁止";
                passColup = false;
                prevRect = QRect(pt.x()*50, pt.y()*50 + 50, 50, 50);
                prevList = items(prevRect);
                for (QGraphicsItem *prevItem : prevList){
                    if(WaterExplosion *we = dynamic_cast<WaterExplosion *>(prevItem)){
                        if (we->direction == WaterExplosion::WaterExplosionDirection::up){
                            we->direction = WaterExplosion::WaterExplosionDirection::uph;
                        }
                    }
                }
                break;
            case 0: //down
                qDebug() << "[GameMMScene] 下爆炸已被禁止";
                passColdw = false;
                prevRect = QRect(pt.x()*50, pt.y()*50 - 50, 50, 50);
                prevList = items(prevRect);
                for (QGraphicsItem *prevItem : prevList){
                    if(WaterExplosion *we = dynamic_cast<WaterExplosion *>(prevItem)){
                        if (we->direction == WaterExplosion::WaterExplosionDirection::dw){
                            we->direction = WaterExplosion::WaterExplosionDirection::dwh;
                        }
                    }
                }
                break;
            }

            continue;
        }
        // 4-5. 偵測位置放
        switch(putcount%4){
        case 3: // 左
            if(!passCollf){
                qDebug() << "[GameMMScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 LF";
                continue;
            }
            if(putcount == 3 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::lfh);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") LFH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::lf);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") LF";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        case 2: // 右
            if(!passColrg){
                qDebug() << "[GameMMScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 RG";
                continue;
            }
            if(putcount == 2 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::rgh);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") RGH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::rg);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") RG";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        case 1: // 上
            if(!passColup){
                qDebug() << "[GameMMScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 UP";
                continue;
            }
            if(putcount == 1 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::uph);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") UPH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::up);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") UP";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        case 0: // 下
            if(!passColdw){
                qDebug() << "[GameMMScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 DW";
                continue;
            }
            if(putcount == 0 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::dwh);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") DWH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::dw);
                qDebug() << "[GameMMScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") DW";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        }
    }

    // 5. 場上的爆炸，全部（破壞 MaBrick, FxBrick, DropItem, Player etc）
    QList<QGraphicsItem *> itemsNext = items();
    for (QGraphicsItem *item : itemsNext){
        // 5-1. 以「WaterExplosion」為主體
        if (WaterExplosion *we = dynamic_cast<WaterExplosion*>(item)){
            // 5-1-1. 偵測是否為重要點位 做刪除「MaBrick, FxBrick」
            switch(we->direction){
            case WaterExplosion::WaterExplosionDirection::ce: // 中間 偵測四周 上 → 下 → 左 → 右
            {
                QRectF nextRect(we->pos().x(), we->pos().y()-50, 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        generateItem(ma->pos().x(), ma->pos().y());
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        generateItem(fb->pos().x(), fb->pos().y());
                        fb->setPos(-100, -100);
                    }
                }
            }
            {
                QRectF nextRect(we->pos().x(), we->pos().y()+50, 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        generateItem(ma->pos().x(), ma->pos().y());
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        generateItem(fb->pos().x(), fb->pos().y());
                        fb->setPos(-100, -100);
                    }
                }
            }
            {
                QRectF nextRect(we->pos().x()-50, we->pos().y(), 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        generateItem(ma->pos().x(), ma->pos().y());
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        generateItem(fb->pos().x(), fb->pos().y());
                        fb->setPos(-100, -100);
                    }
                }
            }
            {
                QRectF nextRect(we->pos().x()+50, we->pos().y(), 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        generateItem(ma->pos().x(), ma->pos().y());
                        ma->setPos(-100, -100);
                        qDebug() << "[GameMMScene] MaBrick 已被移動至 (-100, -100)";
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        generateItem(fb->pos().x(), fb->pos().y());
                        fb->setPos(-100, -100);
                        qDebug() << "[GameMMScene] FxBrick 已被移動至 (-100, -100)";
                    }
                }
            }
                break;
            case WaterExplosion::WaterExplosionDirection::uph: // 上頭 如果有截斷 偵測往上一格
                if (!passColup)
            {
                QRectF nextRect(we->pos().x(), we->pos().y()-50, 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        fb->setPos(-100, -100);
                    }
                }
            }
                break;
            case WaterExplosion::WaterExplosionDirection::dwh: // 下頭 如果有截斷 偵測往下一格
                if (!passColdw)
            {
                QRectF nextRect(we->pos().x(), we->pos().y()+50, 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        fb->setPos(-100, -100);
                    }
                }
            }
                break;
            case WaterExplosion::WaterExplosionDirection::lfh: // 左頭 如果有截斷 偵測往左一格
                if (!passCollf)
            {
                QRectF nextRect(we->pos().x()-50, we->pos().y(), 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        fb->setPos(-100, -100);
                    }
                }
            }
                break;
            case WaterExplosion::WaterExplosionDirection::rgh: // 右頭 如果有截斷 偵測往右一格
                if (!passColrg)
            {
                QRectF nextRect(we->pos().x()+50, we->pos().y(), 50, 50);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(MaBrick *ma = dynamic_cast<MaBrick*>(item)){
                        ma->setPos(-100, -100);
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        fb->setPos(-100, -100);
                    }
                }
            }
                break;
            }
            // 5-1-2. 刪除所有當地的 DropItem, 影響 Player, Robot, Monster
            {
                QRectF nextRect(we->pos().x(), we->pos().y(), 40, 40);
                QList<QGraphicsItem *> itemAtNext = items(nextRect);
                for (QGraphicsItem *item : itemAtNext) {
                    if(DropItem *di = dynamic_cast<DropItem*>(item)){
                        di->setPos(-100, -100);
                        qDebug() << "[GameMMScene] DropItem 已被移動至 (-100, -100)";
                    }
                    if(Player *pl = dynamic_cast<Player *>(item)){
                        if(itemTurtle){
                            qDebug() << "[GameMMScene] 烏龜抵擋傷害";
                            player->turtleEnabled = false;
                            itemTurtle = false;

                            QTransform transform;
                            transform.scale(-1, 1);
                            transform.translate(-50, 0);
                            switch(player->ws){
                            case Player::walkingStatus::up01:
                            case Player::walkingStatus::up02:
                            case Player::walkingStatus::up03:
                            case Player::walkingStatus::up04:
                                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(80, 138, 50, 56));
                                break;
                            case Player::walkingStatus::dw01:
                            case Player::walkingStatus::dw02:
                            case Player::walkingStatus::dw03:
                            case Player::walkingStatus::dw04:
                                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(150, 138, 50, 56));
                                break;
                            case Player::walkingStatus::lf01:
                            case Player::walkingStatus::lf02:
                            case Player::walkingStatus::lf03:
                            case Player::walkingStatus::lf04:
                                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(220, 207, 50, 56).transformed(transform));
                                break;
                            case Player::walkingStatus::rg01:
                            case Player::walkingStatus::rg02:
                            case Player::walkingStatus::rg03:
                            case Player::walkingStatus::rg04:
                                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(220, 207, 50, 56));
                                break;
                            }
                        }else{
                            isPlayerBlocked = 300;
                            qDebug() << "[GameMMScene] 玩家已被水球炸到，倒數三秒爆炸";
                        }
                    }
                    if(Monster *mn = dynamic_cast<Monster *>(item)){
                        mn->setPos(-100, -100);
                        qDebug() << "[GameMMScene] Monster 已移動至 (-100, -100)";
                    }
                }
            }
        }
    }
    delete(wb);
}

void GameMMScene::generateItem(int x, int y){
    DropItem *di;
    switch(QRandomGenerator::global()->bounded(7)){
    case 6:
        di = new DropItem(nullptr, DropItem::DropItemType::moonwalk);
        qDebug() << "[GameMMScene] 生成「月亮鞋」！";
        break;
    case 5:
        di = new DropItem(nullptr, DropItem::DropItemType::needle);
        qDebug() << "[GameMMScene] 生成「針」！";
        break;
    case 4:
        di = new DropItem(nullptr, DropItem::DropItemType::glove);
        qDebug() << "[GameMMScene] 生成「手套」！";
        break;
    case 3:
        di = new DropItem(nullptr, DropItem::DropItemType::power_potion);
        qDebug() << "[GameMMScene] 生成「藥水」！";
        break;
    case 2:
        di = new DropItem(nullptr, DropItem::DropItemType::speed_shoes);
        qDebug() << "[GameMMScene] 生成「加速」！";
        break;
    case 1:
        di = new DropItem(nullptr, DropItem::DropItemType::turtle);
        qDebug() << "[GameMMScene] 生成「烏龜」！";
        break;
    case 0:
        di = new DropItem(nullptr, DropItem::DropItemType::waterballoon);
        qDebug() << "[GameMMScene] 生成「水球」！";
        break;
    }
    addItem(di);
    di->setPos(x+10, y);
}

// MARK: - Tick
void GameMMScene::tick() {
    // 每 0.01 秒觸發一次
    if (!timerEnabled) {
        return;
    }
    QList<QGraphicsItem*> allItems = items(); // scene 所有的 items
    timeSec++;

    // 1. 觸發玩家走路（移動順暢）
    if ((ipX+ipY) != 0){
        if(ipX > 0){
            pX++;
            ipX--;
            player->krg();
        } else if (ipX < 0) {
            pX--;
            ipX++;
            player->klf();
        }
        if(ipY > 0){
            pY++;
            ipY--;
            player->kdw();
        } else if (ipY < 0) {
            pY--;
            ipY++;
            player->kup();
        }
        if (ipX+ipY == 0){
            qDebug() << "[GameMMScene] Player 成功走路並走到 (" << pX << "," << pY << ")";
            qDebug() << "[GameMMScene] 目前 Player 所屬 Cell (" << ((pX+10)/50+1) << "," << ((pY+10)/50+1) << ")";
        }
        player->setPos(pX,pY);
    }

    // 1-1. 雙倍走路（再偵測一次）｜不包含烏龜
    if(itemSpeed && !itemTurtle){
        if ((ipX+ipY) != 0){
            if(ipX > 0){
                pX++;
                ipX--;
            } else if (ipX < 0) {
                pX--;
                ipX++;
            }
            if(ipY > 0){
                pY++;
                ipY--;
            } else if (ipY < 0) {
                pY--;
                ipY++;
            }
            if (ipX+ipY == 0){
                qDebug() << "[GameMMScene] Player 成功走路並走到 (" << pX << "," << pY << ")";
                qDebug() << "[GameMMScene] 目前 Player 所屬 Cell (" << ((pX+10)/50+1) << "," << ((pY+10)/50+1) << ")";
            }
            player->setPos(pX,pY);
        }
    }

    // 2. 檢查場景 objects 處理事件
    for (QGraphicsItem *item : allItems){
        // 2-1. MaBrick
        if (MaBrick* mb = dynamic_cast<MaBrick*>(item)){
            if((mb->ipX+mb->ipY)!=0){
                // 2-1-1. 處理 MaBrick 移動事件
                // qDebug() << "[GameMMScene] Tick 正在處理 MaBrick 移動，剩餘 (" << mb->ipX << "," << mb->ipY << ")";
                if(mb->ipX > 0){
                    mb->setPos(mb->pos().x()+1,mb->pos().y());
                    mb->ipX--;
                } else if (mb->ipX < 0) {
                    mb->setPos(mb->pos().x()-1,mb->pos().y());
                    mb->ipX++;
                }
                if(mb->ipY > 0){
                    mb->setPos(mb->pos().x(),mb->pos().y()+1);
                    mb->ipY--;
                } else if (mb->ipY < 0) {
                    mb->setPos(mb->pos().x(),mb->pos().y()-1);
                    mb->ipY++;
                }
                if (mb->ipX + mb->ipY == 0){
                    qDebug() << "[GameMMScene] MaBrick 成功移動到 (" << mb->pos().x() << "," << mb->pos().y() << ")";
                }
            }
            continue;
        }
        // 2-2. Water Balloon
        else if (WaterBalloon *wb = dynamic_cast<WaterBalloon*>(item)){
            if(wb->flyingX + wb->flyingY != 0){
                // 2-2-1. 手套丟水球
                if(wb->flyingX != 0){
                    // 2-2-1-1. 左或右
                    // 丟兩格 中點為 50;
                    if(wb->flyingX > 0){
                        wb->setPos(wb->pos().x()+1, wb->pos().y());
                        wb->flyingX--;
                    }else {
                        wb->setPos(wb->pos().x()-1, wb->pos().y());
                        wb->flyingX++;
                    }
                }
                if(wb->flyingY != 0){
                    // 2-2-1-1. 左或右
                    // 丟兩格 中點為 50;
                    if(abs(wb->flyingY)-50 > 0){
                        if(wb->flyingY > 0){
                            wb->setPos(wb->pos().x(), wb->pos().y()+2);
                            wb->flyingY -= 2;
                        } else {
                            wb->setPos(wb->pos().x(), wb->pos().y()-2);
                            wb->flyingY += 2;
                        }
                    } else {
                        if(wb->flyingY > 0){
                            wb->setPos(wb->pos().x(), wb->pos().y()+1);
                            wb->flyingY -= 1;
                        } else {
                            wb->setPos(wb->pos().x(), wb->pos().y()-1);
                            wb->flyingY += 1;
                        }
                    }
                }
            }

            // 偵測手套（有無丟到邊界）
            if (wb->pos().x() >= 550) { // 移出右邊界
                wb->setPos(wb->pos().x() - 550, wb->pos().y());
                qDebug() << "[WaterBalloon] Wrapped from right to left!";
            } else if (wb->pos().x() < -50) { // 移出左邊界 (圖像左上角在-50時完全離開)
                wb->setPos(wb->pos().x() + 550, wb->pos().y());
                qDebug() << "[WaterBalloon] Wrapped from left to right!";
            }

            if (wb->pos().y() >= 450) { // 移出下邊界 (遊戲區域Y上限)
                wb->setPos(wb->pos().x(), wb->pos().y() - 450);
                qDebug() << "[WaterBalloon] Wrapped from bottom to top!";
            } else if (wb->pos().y() < -50) { // 移出上邊界 (圖像左上角在-50時完全離開)
                wb->setPos(wb->pos().x(), wb->pos().y() + 450);
                qDebug() << "[WaterBalloon] Wrapped from top to bottom!";
            }

            // 檢測停止時有無碰到物品，有的話再往該方向一格
            if(abs(wb->flyingX + wb->flyingY) == 1){
                qDebug() << "[GameMMScene] 炸彈著地偵測中";
                QRectF wbRect(wb->pos().x()+10, wb->pos().y()+10, 10, 10);
                QList<QGraphicsItem*> wbItems = items(wbRect);
                for(QGraphicsItem *item : wbItems){
                    if (dynamic_cast<InBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<FxBrick *>(item)){
                        qDebug() << "[GameMMScene] 水球著地位碰到方塊";
                        if(wb->flyingX > 0){
                            wb->flyingX = 50;
                        } else if (wb->flyingX < 0){
                            wb->flyingX = -50;
                        } else if (wb->flyingY > 0){
                            wb->flyingY = 50;
                        } else if (wb->flyingY < 0){
                            wb->flyingY = -50;
                        }
                        break;
                    }
                }
            }

            if(wb->boomgatime > 0){
                // 2-2-2. 處理Boomga時間（沒有在飛時才減少時間）
                if(wb->flyingX + wb->flyingY == 0){
                    wb->boomgatime--;
                }
                wb->updateBallon();
            } else {
                // 2-2-3. 爆炸刪除＋爆炸影響
                waterballoonExplosion(wb);
            }
            continue;
        }
        // 2-3. Water Balloon Explonsion
        else if (WaterExplosion *we = dynamic_cast<WaterExplosion*>(item)){
            // 他附近的事件
            // 遇到碰撞刪除箱子等等
            // 自己本身變更動畫
            we->time--;
            we->updateImage();
            if(we->time <= 0){
                delete(we);
            }
        }
        // 2-4. DropItem
        else if (DropItem *di = dynamic_cast<DropItem*>(item)){
            // 2-4-1. Animation
            if(timeSec%20<10){
                di->setPos(di->pos().x(), di->pos().y()-1);
            }else{
                di->setPos(di->pos().x(), di->pos().y()+1);
            }
            // 2-4-2. touched Player
            QRect nextRect(di->pos().x()+5, di->pos().y()+5, 20, 20);
            QList<QGraphicsItem*> touchedItems = items(nextRect);
            for(QGraphicsItem *tItem : touchedItems){
                if(dynamic_cast<Player*>(tItem)){
                    qDebug() << "[GameMMScene] 物品碰到玩家";
                    switch(di->type){
                    case DropItem::DropItemType::waterballoon:
                        waterballMaxCount++;
                        if(waterballMaxCount > 5){
                            waterballMaxCount = 5;
                        }
                        qDebug() << "[GameMMScene] 水球++，目前有" << waterballMaxCount;
                        break;
                    case DropItem::DropItemType::glove:
                        gloveTime = 3;
                        break;
                    case DropItem::DropItemType::power_potion:
                        waterballPower++;
                        if(waterballPower > 5){
                            waterballPower = 5;
                        }
                        qDebug() << "[GameMMScene] 威力++，目前有" << waterballPower;
                        break;
                    case DropItem::DropItemType::speed_shoes:
                        itemSpeed = true;
                        qDebug() << "[GameMMScene] 獲得加速";
                        break;
                    case DropItem::DropItemType::turtle:
                        itemTurtle = true;
                        qDebug() << "[GameMMScene] 獲得烏龜";
                        player->turtleEnabled = true;
                        break;
                    case DropItem::DropItemType::moonwalk:
                        moonTime = 1000;
                        break;
                    case DropItem::DropItemType::needle:
                        itemNeedle = true;
                        break;
                    }
                    qDebug() << "[GameMMScene] 道具再見！（移動到陌生地方）";
                    di->setPos(-100, -100);
                }
            }
        }
        // 2-5. Monster
        else if (Monster *ms = dynamic_cast<Monster*>(item)){
            if(ms->ipX + ms->ipY == 0){
                movingMonster(ms);
            } else {
                if(ms->ipX > 0){
                    ms->ipX--;
                    ms->setPos(ms->pos().x()+1, ms->pos().y());
                    ms->rg();
                } else if (ms->ipX < 0) {
                    ms->ipX++;
                    ms->setPos(ms->pos().x()-1, ms->pos().y());
                    ms->lf();
                }
                if(ms->ipY > 0){
                    ms->ipY--;
                    ms->setPos(ms->pos().x(), ms->pos().y()+1);
                    ms->dw();
                } else if (ms->ipY < 0) {
                    ms->ipY++;
                    ms->setPos(ms->pos().x(), ms->pos().y()-1);
                    ms->up();
                }
            }
            // 碰到玩家殺死他
            QRect nextRect(ms->pos().x()+5, ms->pos().y()+5, 20, 20);
            QList<QGraphicsItem*> touchedItems = items(nextRect);
            for(QGraphicsItem *tItem : touchedItems){
                if(dynamic_cast<Player*>(tItem)){
                    qDebug() << "[GameMMScene] Monster 碰到玩家";
                    // 扣命
                    life--;
                    itemSpeed = false;
                    itemTurtle = false;
                    itemNeedle = false;
                    gloveTime = 0;
                    moonTime = 0;
                    waterballMaxCount = 1;
                    waterballPower = 1;
                    player->turtleEnabled = false;
                    // 回初始點
                    ipX = 0;
                    ipY = 0;
                    player->setPos(initX, initY);
                    pX = initX;
                    pY = initY;

                    // 沒命？
                    if (life <= 0){
                        requestSceneChange(sceneslist::ending);
                    }
                }
            }
        }
    }

    // 3. ui info
    updateTextInfo();

    // 4. 調整道具時間
    if (moonTime > 0){
        moonTime--;
    }

    // 5. 玩家在水球中
    if (isPlayerBlocked > 0){
        player->isBlocked = true;
        switch(timeSec%80){
        case 10:
            player->setPixmap(QPixmap(":/data/character/palyer.png").copy(222,274,64,64));
            break;
        case 30:
            player->setPixmap(QPixmap(":/data/character/palyer.png").copy(295,274,64,64));
            break;
        case 50:
            player->setPixmap(QPixmap(":/data/character/palyer.png").copy(364,274,64,64));
            break;
        case 70:
            player->setPixmap(QPixmap(":/data/character/palyer.png").copy(436,274,64,64));
            break;
        }
        isPlayerBlocked--;

        if(isPlayerBlocked == 10){
            player->setPixmap(QPixmap(":/data/character/palyer.png").copy(217,355,70,70));
        }

        if(isPlayerBlocked == 5){
            player->setPixmap(QPixmap(":/data/character/palyer.png").copy(416,355,70,70));
        }
        if(isPlayerBlocked == 0){
            // 扣命
            life--;
            itemSpeed = false;
            itemTurtle = false;
            itemNeedle = false;
            gloveTime = 0;
            moonTime = 0;
            waterballMaxCount = 1;
            waterballPower = 1;
            player->turtleEnabled = false;
            // 回初始點
            ipX = 0;
            ipY = 0;
            player->setPos(initX, initY);
            pX = initX;
            pY = initY;

            // 沒命？
            if (life <= 0){
                requestSceneChange(sceneslist::ending);
            }

            // 死亡複製上面

            QTransform transform;
            transform.scale(-1, 1);
            transform.translate(-50, 0);
            switch(player->ws){
            case Player::walkingStatus::up01:
            case Player::walkingStatus::up02:
            case Player::walkingStatus::up03:
            case Player::walkingStatus::up04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(80, 138, 50, 56));
                break;
            case Player::walkingStatus::dw01:
            case Player::walkingStatus::dw02:
            case Player::walkingStatus::dw03:
            case Player::walkingStatus::dw04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(150, 138, 50, 56));
                break;
            case Player::walkingStatus::lf01:
            case Player::walkingStatus::lf02:
            case Player::walkingStatus::lf03:
            case Player::walkingStatus::lf04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(220, 207, 50, 56).transformed(transform));
                break;
            case Player::walkingStatus::rg01:
            case Player::walkingStatus::rg02:
            case Player::walkingStatus::rg03:
            case Player::walkingStatus::rg04:
                player->setPixmap(QPixmap(":/data/character/palyer.png").copy(220, 207, 50, 56));
                break;
            }
        }
    } else {
        player->isBlocked = false;
    }

    // 6. 每一秒檢查有沒有贏
    if (timeSec % 100 == 0){
        bool doesNotHaveMonsters = true;
        QRectF fullRect = QRectF(0, 0, 550, 450);
        for(QGraphicsItem *item : items(fullRect)){
            if(dynamic_cast<Monster *>(item)){
                doesNotHaveMonsters = false;
                break;
            }
        }
        if(doesNotHaveMonsters){
            // WIN!
            qDebug() << "[GameMMScene] 這場獲勝。";
            requestSceneChange(sceneslist::gameMMT);
        }
    }
}

// MARK: - ITEM
void GameMMScene::updateTextInfo(){
    textWater->setPlainText(QString("%1").arg(waterballMaxCount));
    textPower->setPlainText(QString("%1").arg(waterballPower));
    uiLife->setPlainText(QString("%1").arg(life));
    if (itemSpeed){
        textSpeed->setPlainText("有");
    } else {
        textSpeed->setPlainText("無");
    }
    if (itemTurtle){
        textTurtle->setPlainText("有");
    } else {
        textTurtle->setPlainText("無");
    }
    if (itemNeedle){
        textNeedle->setPlainText("有");
    } else {
        textNeedle->setPlainText("無");
    }
    if (gloveTime > 0){
        textGlove->setPlainText(QString("%1次").arg(gloveTime));
    } else {
        textGlove->setPlainText("無");
    }
    if (moonTime > 0){
        textMoon->setPlainText(QString("%1.%2s").arg(moonTime/100).arg(moonTime%100));
    } else {
        textMoon->setPlainText("無");
    }
}

// MARK: - Monster

void GameMMScene::movingMonster(Monster *mn){
    // 判斷方位
    int dX = 0, dY = 0;
    switch(mn->ds){
    case Monster::destDir::up:
        dY = -10;
        break;
    case Monster::destDir::dw:
        dY = 10;
        break;
    case Monster::destDir::lf:
        dX = -10;
        break;
    case Monster::destDir::rg:
        dX = 10;
        break;
    }

    // nextX, nextY 預測玩家下個移動位置並進行檢查
    int nextX = mn->pos().x() + dX;
    int nextY = mn->pos().y() + dY;

    // 1. 邊界檢查
    if ((nextX > 500) || (nextX < 0) || (nextY > 400) || (nextY < -10)){
        // qDebug() << "[GameMMScene] Monster 無法走路，移動 (" << dX << "," << dY << ") 後為 (" << mn->pos().x() << "," << mn->pos().y() << ") 將超過邊界";

        switch(mn->ds){
        case Monster::destDir::up:
            // 本官不處理
            break;
        case Monster::destDir::dw:
            // 本官不處理
            break;
        case Monster::destDir::lf:
            mn->ds = Monster::destDir::rg;
            break;
        case Monster::destDir::rg:
            mn->ds = Monster::destDir::lf;
            break;
        }


        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(nextX, nextY+10, 35, 35); // 應該為 56, 50 但弄小塊一點
    QList<QGraphicsItem *> itemAtNext = items(nextRect); // 用 items 查看下一個位置有哪些區塊
    for (QGraphicsItem *item : itemAtNext) {
        // 檢查有無碰撞到以下方塊，確認要阻止或進行功能

        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<WaterBalloon *>(item)){
            // 2-1. InBrick、FxBrick 不能移動的方塊
            // qDebug() << "[GameMMScene] Monster 碰到 In, Fx 磚塊，無法移動到 (" << nextX << "," << nextY << ")";

            // 2-2. 試著轉換方向（依照關卡測試）
            switch(mn->ds){
            case Monster::destDir::up:
                // 本官不處理
                break;
            case Monster::destDir::dw:
                // 本官不處理
                break;
            case Monster::destDir::lf:
                mn->ds = Monster::destDir::rg;
                break;
            case Monster::destDir::rg:
                mn->ds = Monster::destDir::lf;
                break;
            }

            return;
        }
    }

    // 將移動距離加入待處理（如果還可以走的話）
    if(mn->ipX+mn->ipY == 0){
        mn->ipX = dX;
        mn->ipY = dY;
        // qDebug() << "[GameMMScene] Monster 成功觸發走路，將 (" << mn->ipX << "," << mn->ipY << ") 加入待處理序列";
        // qDebug() << "[GameMMScene] Monster | (" << mn->ipX << "," << mn->ipY << ")";
    } else {
        // qDebug() << "[GameMMScene] 移動尚在冷卻期，忽略";
        return;
    }
}
