#include "src/scenes/GameMMBScene.h"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>            // 抵爸個
#include <QFile>             // 讀檔案
#include <QTextStream>       // 讀文字
#include <QStringList>       // 拆輸入字串
#include <QRandomGenerator>  // 亂數
#include <QList>             // 碰撞檢查
#include <QGraphicsRectItem>

GameMMBScene::GameMMBScene(QObject *parent)
{
    qDebug() << "[GameMMBScene] 已被構建";
    setup();
}

GameMMBScene::~GameMMBScene(){
    qDebug() << "[GameMMBScene] 已被解構";

    // 關閉計時器
    timer->stop();
}

void GameMMBScene::keyPressEvent(QKeyEvent *event){
    qDebug() << "";
    switch(event->key()){
    case Qt::Key_Up:
        qDebug() << "[GameMMBScene] 按了 ↑";
        movingPlayer(direction::up);
        player->kup();
        break;
    case Qt::Key_Down:
        qDebug() << "[GameMMBScene] 按了 ↓";
        movingPlayer(direction::dw);
        player->kdw();
        break;
    case Qt::Key_Left:
        qDebug() << "[GameMMBScene] 按了 ←";
        movingPlayer(direction::lf);
        player->klf();
        break;
    case Qt::Key_Right:
        qDebug() << "[GameMMBScene] 按了 →";
        movingPlayer(direction::rg);
        player->krg();
        break;
    case Qt::Key_Space:
        qDebug() << "[GameMMBScene] 按了 Space";
        puttingWaterballoon(((pX+10)/50+1),((pY+10)/50+1));
        break;
    case Qt::Key_Shift:
        qDebug() << "[GameMMBScene] 按了 Shift";
        if(!itemNeedle){
            qDebug() << "[GameMMBScene] 沒有 needle，略過";
            return;
        }
        if(isPlayerBlocked > 0){
            qDebug() << "[GameMMBScene] 成功逃脫";
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


        // 模擬機器人
        case Qt::Key_W:
            qDebug() << "[GameMMBScene] 模擬機器人 按下 W 前";
            robot->kup();
            movingRobot(direction::up);
            break;
        case Qt::Key_A:
            qDebug() << "[GameMMBScene] 模擬機器人 按下 A 左";
            robot->klf();
            movingRobot(direction::lf);
            break;
        case Qt::Key_S:
            qDebug() << "[GameMMBScene] 模擬機器人 按下 S 下";
            robot->kdw();
            movingRobot(direction::dw);
            break;
        case Qt::Key_D:
            qDebug() << "[GameMMBScene] 模擬機器人 按下 D 右";
            robot->krg();
            movingRobot(direction::rg);
            break;
    case Qt::Key_Q:
        qDebug() << "[GameMMBScene] 模擬機器人 按下 Q 放下炸彈";
        qDebug() << "[GameMMBScene] 機器人放下之炸彈不計數";
        waterballCurCount--;
        puttingWaterballoon(((rX+10)/50+1),((rY+10)/50+1));
        break;
    }

}

void GameMMBScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem *clickedItem = itemAt(event->scenePos(), QTransform());
    if (clickedItem == uiHome){
        qDebug() << "[GameMMBScene] uiHome Clicked.";
        requestSceneChange(sceneslist::title);
    }
    if (clickedItem == uiPause){
        qDebug() << "[GameMMBScene] uiPause Clicked";
        uiPauseIndictator->setPlainText("暫停中");
        timerEnabled = false;
    }
    if (clickedItem == uiPauseIndictator){
        qDebug() << "[GameMMBScene] uiPauseIndictator Clicked";
        uiPauseIndictator->setPlainText("");
        timerEnabled = true;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GameMMBScene::setup(){
    // 設定大小
    setSceneRect(0, 0, 550, 550);

    // MARK: - 設定 UI
    qDebug() << "[GameMMBScene] 設定 UI Menu";
    QGraphicsPixmapItem *menu = new QGraphicsPixmapItem(QPixmap(":/data/ui/ui.png").scaled(550, 100));
    menu->setPos(0, 450);
    addItem(menu);

    qDebug() << "[GameMMBScene] 正在建立顯示物品中心";
    QGraphicsRectItem *br = new QGraphicsRectItem(300, 450, 250, 95);
    br->setBrush(Qt::blue);
    addItem(br);

    qDebug() << "[GameMMBScene] 正在建立 UI 主畫面與暫停";
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

    qDebug() << "[GameMMBScene] 正在建立本模式禁用道具 以及 機器人部署 資訊";
//    {
//        QGraphicsTextItem *text = new QGraphicsTextItem("本模式禁用道具");
//        text->setFont(QFont("Arial", 15));
//        text->setDefaultTextColor(Qt::red);
//        text->setPos(180, 480);
//        addItem(text);
//    }
    robotText = new QGraphicsTextItem("老大思考中");
    robotText->setFont(QFont("Arial",20));
    robotText->setDefaultTextColor(Qt::white);
    robotText->setPos(10, 450);
    addItem(robotText);

    qDebug() << "[GameMMBScene] 正在建立剩餘命數";
    uiLife = new QGraphicsTextItem("1");
    uiLife->setFont(QFont("Arial", 40));
    uiLife->setPos(70, 490);
    uiLife->setDefaultTextColor(Qt::white);
    addItem(uiLife);
    uiBoss = new QGraphicsTextItem("10/10");
    uiBoss->setFont(QFont("Arial", 20));
    uiBoss->setPos(100, 470);
    uiBoss->setDefaultTextColor(Qt::white);
    addItem(uiBoss);

    qDebug() << "[GameMMBScene] 正在建立各種物體數量顯示";
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

    // 讀地圖檔案
    qDebug() << "[GameMMBScene] 讀取地圖檔案";
    QString filePath = ":/data/maps/gameMM03.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[GameMMBScene] 無法開啟檔案：" << filePath;
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

    qDebug() << "[GameMMBScene] mapObj:";
    for (const QVector<int>& row : mapObj) {
        QString line;
        for (int val : row) {
            line += QString::number(val) + " ";
        }
        qDebug().noquote() << line.trimmed();
    }

    // 建構地圖
    qDebug() << "[GameMMBScene] 建構地圖";
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
            }
        }
    }

    // 建構雕像
    if (true){
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem(QPixmap(":/data/brick/statue_1.png"));
        pi->setPos(-10, 290);
        addItem(pi);
    }
    if (true){
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem(QPixmap(":/data/brick/statue_1.png"));
        pi->setPos(490, 290);
        addItem(pi);
    }
    if (true){
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem(QPixmap(":/data/brick/statue_2.png").scaled(50, 50));
        pi->setPos(0, 0);
        addItem(pi);
    }
    if (true){
        QGraphicsPixmapItem *pi = new QGraphicsPixmapItem(QPixmap(":/data/brick/statue_2.png").scaled(50, 50));
        pi->setPos(500, 0);
        addItem(pi);
    }

    // MARK: - 建構玩家
    qDebug() << "[GameMMBScene] 建立玩家";
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
                qDebug() << "[GameMMBScene] 玩家生成於 (" << row*50 << "," << col*50 << ")";
                break;
            }
        }
    }
    addItem(player);
    player->setZValue(100);

    // MARK: - 建構機器人
    qDebug() << "[GameMMBScene] 建立老大";
    robot = new Boss;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 11; ++col) {
            int val = mapObj[row][col];
            if (val == 5){
                rX = col*50;
                rY = row*50 - 6;
                robot->setPos(rX, rY);
                initrX = pX;
                initrY = pY;
                qDebug() << "[GameMMBScene] 機器人生成於 (" << row*50 << "," << col*50 << ")";
                break;
            }
        }
    }
    addItem(robot);
    robot->setZValue(100);

    // MARK: - 開啟 Tick
    qDebug() << "[GameMMBScene] 建立 tick 計時器（0.01s）";
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameMMBScene::tick);
    timer->start(10);
}

void GameMMBScene::movingPlayer(direction dir){

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
        qDebug() << "[GameMMBScene] Player 無法走路，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";

        // 1-1. 是否有太空走路
        if (moonTime > 0){
            qDebug() << "[GameMMBScene] 有 moonwalk";
            if(nextX > 500){
                qDebug() << "[GameMMBScene] 邊界走到最左邊";
                pX = -50;
                player->setPos(-50, pY);
                ipX = 50;
            }
            if(nextX < 0){
                qDebug() << "[GameMMBScene] 邊界走到最左邊";
                pX = 550;
                player->setPos(550, pY);
                ipX = -50;
            }
            if(nextY > 400){
                qDebug() << "[GameMMBScene] 邊界走到最左邊";
                pY = -56;
                player->setPos(pX, -56);
                ipY = 50;
            }
            if(nextY < -10){
                qDebug() << "[GameMMBScene] 邊界走到最左邊";
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
            qDebug() << "[GameMMBScene] Player 碰到 In, Fx 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
            return;
        }
        if (MaBrick *mb = dynamic_cast<MaBrick *>(item)){
            // 前面只需檢查，現在需要將結果指標存到 MaBrick *mb 中
            // 2-2. MaBrick 可移動方塊，進行移動方塊判斷。但仍不能動
            qDebug() << "[GameMMBScene] Player 碰到 Ma 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
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
            qDebug() << "[GameMMBScene] 未知的 MaBrick 移動方位";
            return;
        }
    }

    // 將移動距離加入待處理（如果還可以走的話）
    if(ipX+ipY == 0){
        ipX = dX;
        ipY = dY;
        qDebug() << "[GameMMBScene] 玩家成功觸發走路，將 (" << ipX << "," << ipY << ") 加入待處理序列";
        qDebug() << "[GameMMBScene] Player | (" << ipX << "," << ipY << ")";
    } else {
        qDebug() << "[GameMMBScene] 移動尚在冷卻期，忽略";
        return;
    }
}

void GameMMBScene::movingRobot(direction dir){
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
    int nextX = rX + dX;
    int nextY = rY + dY;

    // 1. 邊界檢查
    if ((nextX > 500) || (nextX < 0) || (nextY > 400) || (nextY < -10)){
        qDebug() << "[GameMMBScene] Robot 無法走路，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";
        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(nextX, nextY+10, 35, 35); // 應該為 56, 50 但弄小塊一點
    QList<QGraphicsItem *> itemAtNext = items(nextRect); // 用 items 查看下一個位置有哪些區塊
    bool putBombAfter = false;
    for (QGraphicsItem *item : itemAtNext) {
        // 檢查有無碰撞到以下方塊，確認要阻止或進行功能
//        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) ){
//            // 2-1. InBrick、FxBrick 不能移動的方塊
//            qDebug() << "[GameMMBScene] Robot 碰到 In, Fx 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
//            putBombAfter = true;
//        }
        if(WaterBalloon *wb = dynamic_cast<WaterBalloon *>(item)){
            // 碰到player炸彈立刻爆炸
            if(!wb->isOctupusWater){
                wb->boomgatime = 10;
            }
        }
        if(Player *pl = dynamic_cast<Player *>(item)){
            // 碰到玩家立刻死亡
            qDebug() << "[Boss] 殺死玩家";
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
        if (MaBrick *mb = dynamic_cast<MaBrick *>(item)){
            // 前面只需檢查，現在需要將結果指標存到 MaBrick *mb 中
            // 2-2. MaBrick 可移動方塊，進行移動方塊判斷。但仍不能動
            qDebug() << "[GameMMBScene] Robot 碰到 Ma 磚塊，無法移動到 (" << nextX << "," << nextY << ")";
            if (dX > 0){
                if (movingMaBrickByRobot(direction::rg, mb)){
                    return;
                } else {
                    putBombAfter = true;
                    break;
                }
            }
            if (dX < 0){
                if (movingMaBrickByRobot(direction::lf, mb)){
                    return;
                } else {
                    putBombAfter = true;
                    break;
                }
            }
            if (dY > 0){
                if (movingMaBrickByRobot(direction::dw, mb)){
                    return;
                } else {
                    putBombAfter = true;
                    break;
                }
            }
            if (dY < 0){
                if (movingMaBrickByRobot(direction::up, mb)){
                    return;
                } else {
                    putBombAfter = true;
                    break;
                }
            }
            qDebug() << "[GameMMBScene] 未知的 MaBrick 移動方位";
            putBombAfter = true;
        }
    }
    if(putBombAfter){
        qDebug() << "[Robot] 放下水球！";
//        puttingWaterballoon(((rX+10)/50+1),((rY+10)/50+1));
        return;
    }

    // 將移動距離加入待處理（如果還可以走的話）
    if(iprX+iprY == 0){
        iprX = dX;
        iprY = dY;
        qDebug() << "[GameMMBScene] Robot 成功觸發走路，將 (" << iprX << "," << iprY << ") 加入待處理序列";
        qDebug() << "[GameMMBScene] Robot | (" << iprX << "," << iprY << ")";
    } else {
        qDebug() << "[GameMMBScene] Robot 移動尚在冷卻期，忽略";
        return;
    }

    // 可以走（走成功）判斷是否和玩家在同一個區域，發射飛球
    // 在左邊（X 小於自己，Y 差值小於10）
    if((pX < rX)&&(pY == rY)){
          qDebug() << "[Boss] 往左邊發射水球";
          WaterBalloon *waterballoon = new WaterBalloon();
          waterballoon->setPos(rX, rY);
          addItem(waterballoon);

          waterballoon->flyingX = -1000;
          waterballoon->flyingY = 0;
          waterballoon->isOctupusWater = true;
    }
    // 在右邊
    if((pX > rX)&&(pY == rY)){
          qDebug() << "[Boss] 往右邊發射水球";
          WaterBalloon *waterballoon = new WaterBalloon();
          waterballoon->setPos(rX, rY);
          addItem(waterballoon);

          waterballoon->flyingX = 1000;
          waterballoon->flyingY = 0;
          waterballoon->isOctupusWater = true;
    }
    // 在上面
    if((pY < rY)&&(pX == rX)){
          qDebug() << "[Boss] 往上邊發射水球";
          WaterBalloon *waterballoon = new WaterBalloon();
          waterballoon->setPos(rX, rY);
          addItem(waterballoon);

          waterballoon->flyingX = 0;
          waterballoon->flyingY = -1000;
          waterballoon->isOctupusWater = true;
    }
    // 在下面
    if((pY > rY)&&(pX == rX)){
          qDebug() << "[Boss] 往下邊發射水球";
          WaterBalloon *waterballoon = new WaterBalloon();
          waterballoon->setPos(rX, rY);
          addItem(waterballoon);

          waterballoon->flyingX = 0;
          waterballoon->flyingY = 1000;
          waterballoon->isOctupusWater = true;
    }
}

void GameMMBScene::movingMaBrick(direction way, MaBrick *mb){

    // 0. 如果玩家還在走路，就不要推
    if (ipX + ipY > 0){
        qDebug() << "[GameMMBScene] 玩家仍在走路，跳過推動";
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

    qDebug() << "[GameMMBScene] MaBrick 目前在 (" << pX << "," << pY << ")。嘗試將他走 (" << dX << "," << dY << ")";

    // 1. 邊界檢查
    if ((nextX > 500) || (nextX < 0) || (nextY > 400) || (nextY < 0)){
        qDebug() << "[GameMMBScene] MaBrick 無法移動，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";
        return;
    }

    // 2. 碰撞檢查
    QRectF nextRect(pX + dX, pY + dY, 50, 50);
    QList<QGraphicsItem *> itemAtNext = items(nextRect);
    for (QGraphicsItem *item : itemAtNext) {
        // MaBrick 移動的碰撞檢測
        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<Player *>(item) || dynamic_cast<Boss *>(item)){
            // 2-1. InBrick、FxBrick、MaBrick 阻擋
            qDebug() << "[GameMMBScene] MaBrick 碰到 In, Fx, Ma 磚塊，無法移動到 (" << pX+dX << "," << pY+dY << ")";
            return;
        }
        // TODO: 未來可能需要判定其他物品
    }

    // 移動成功（加入 mb 佇列，在 tick 執行）
    if(mb->ipX + mb->ipY == 0){
        mb->ipX = dX;
        mb->ipY = dY;
        qDebug() << "[GameMMBScene] 成功處理 MaBrick 移動，移動 (" << mb->ipX << "," << mb->ipY << ") 已加入佇列";
    } else {
        qDebug() << "[GameMMBScene] 處理 MaBrick 失敗，因為仍有佇列";
    }
}

bool GameMMBScene::movingMaBrickByRobot(direction way, MaBrick *mb){

    // true 推成功、 false 推失敗

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

    qDebug() << "[GameMMBScene] MaBrick 目前在 (" << pX << "," << pY << ")。嘗試將他走 (" << dX << "," << dY << ")";

    // 1. 邊界檢查
    if ((nextX > 500) || (nextX < 0) || (nextY > 400) || (nextY < 0)){
        qDebug() << "[GameMMBScene] MaBrick 無法移動，移動 (" << dX << "," << dY << ") 後為 (" << pX << "," << pY << ") 將超過邊界";
        return false;
    }

    // 2. 碰撞檢查
    QRectF nextRect(pX + dX, pY + dY, 50, 50);
    QList<QGraphicsItem *> itemAtNext = items(nextRect);
    for (QGraphicsItem *item : itemAtNext) {
        // MaBrick 移動的碰撞檢測
        if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<Player *>(item) || dynamic_cast<Boss *>(item)){
            // 2-1. InBrick、FxBrick、MaBrick 阻擋
            qDebug() << "[GameMMBScene] MaBrick 碰到 In, Fx, Ma 磚塊，無法移動到 (" << pX+dX << "," << pY+dY << ")";
            return false;
        }
    }

    // 移動成功（加入 mb 佇列，在 tick 執行）
    if(mb->ipX + mb->ipY == 0){
        mb->ipX = dX;
        mb->ipY = dY;
        qDebug() << "[GameMMBScene] 成功處理 MaBrick 移動，移動 (" << mb->ipX << "," << mb->ipY << ") 已加入佇列";
        return true;
    } else {
        qDebug() << "[GameMMBScene] 處理 MaBrick 失敗，因為仍有佇列";
        return false;
    }
    return false;
}

// MARK: - WaterBalloon
void GameMMBScene::puttingWaterballoon(int cellX, int cellY){
    // 應檢查水球數量
    if (waterballCurCount >= waterballMaxCount){
        qDebug() << "[GameMMBScene] 目前場上 Player 水球已達最大值" << waterballCurCount << "/" << waterballMaxCount;
        return;
    } else {
        qDebug() << "[GameMMBScene] 放置之前場上 Player 水球為" << waterballCurCount << "/" << waterballMaxCount;
    }

    // 根據方塊的座標來檢查碰撞
    QRectF nextRect((cellX-1)*50, (cellY-1)*50, 50, 50);
    QList<QGraphicsItem *> itemAtNext = items(nextRect);
    for (QGraphicsItem *item : itemAtNext){
       // 放水球位置是否為空（不能有 InBrick、FxBrick、MaBrick、WaterBalloon)
       if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<WaterBalloon *>(item)){
           qDebug() << "[GameMMBScene] 無法在 (" << cellX << "," << cellY << ") 放置水球，因為有物品阻擋";
           return;
       }
    }

    // 放置水球
    waterballCurCount++;
    WaterBalloon *waterballoon = new WaterBalloon();
    qDebug() << "[GameMMBScene] 已在 (" << cellX << "," << cellY << ") 放置水球，目前場上 Player 水球為" << waterballCurCount << "/" << waterballMaxCount;
    waterballoon->setPos((cellX-1)*50, (cellY-1)*50);
    addItem(waterballoon);

    waterballoon->flyingX = 0;
    waterballoon->flyingY = 0;

    if(gloveTime > 0){
        qDebug() << "[GameMMBScene] 手套效果已套用！";

        switch(player->ws){
        case Player::walkingStatus::up01:
        case Player::walkingStatus::up02:
        case Player::walkingStatus::up03:
        case Player::walkingStatus::up04:
            waterballoon->flyingY = -100;
            qDebug() << "[GameMMBScene] 手套 往上丟！";
            break;
        case Player::walkingStatus::dw01:
        case Player::walkingStatus::dw02:
        case Player::walkingStatus::dw03:
        case Player::walkingStatus::dw04:
            waterballoon->flyingY = 100;
            qDebug() << "[GameMMBScene] 手套 往下丟！";
            break;
        case Player::walkingStatus::lf01:
        case Player::walkingStatus::lf02:
        case Player::walkingStatus::lf03:
        case Player::walkingStatus::lf04:
            waterballoon->flyingX = -100;
            qDebug() << "[GameMMBScene] 手套 往左丟！";
            break;
        case Player::walkingStatus::rg01:
        case Player::walkingStatus::rg02:
        case Player::walkingStatus::rg03:
        case Player::walkingStatus::rg04:
            waterballoon->flyingX = 100;
            qDebug() << "[GameMMBScene] 手套 往右丟！";
            break;
        }
        gloveTime--;
    }
}

void GameMMBScene::waterballoonExplosion(WaterBalloon *wb){
    // 1. 場上水球減少
    if(!wb->isOctupusWater){
        waterballCurCount--;
    }
    qDebug() << "[GameMMBScene] 有水球爆炸，場上 Player 水球剩餘" << waterballCurCount << "/" << waterballMaxCount;

    // 2. 紀錄爆炸座標
    int cellX = wb->pos().x()/50, cellY = wb->pos().y()/50;
    qDebug() << "[GameMMBScene] 該水球爆炸於 (" << cellX+1 << "," << cellY+1 << ")";

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
            qDebug() << "[GameMMBScene] 在 (" << cellX+1 << "," << cellY+1 << ") 的水球爆炸波超過邊界，不放置";
            continue;
        }
        // 4-3. 碰撞檢測，每筆獨立
        QRectF nextRect(pt.x() *50, pt.y() *50, 50, 50);
        QList<QGraphicsItem *> itemAtNext = items(nextRect);
        for (QGraphicsItem *item : itemAtNext){
           // 放水球位置是否為空（不能有 InBrick、FxBrick、MaBrick)
           if (dynamic_cast<InBrick *>(item) || dynamic_cast<FxBrick *>(item) || dynamic_cast<MaBrick *>(item)){
               qDebug() << "[GameMMBScene] 無法在 (" << pt.x()+1 << "," << pt.y()+1 << ") 放置水球爆炸波，因為有物品阻擋";
               passCol = false;
           }
           // 連鎖反應（偵測到 WaterBalloon 讓他也一起爆炸），免 Return
           if (WaterBalloon *wb = dynamic_cast<WaterBalloon *>(item)){
               qDebug() << "[GameMMBScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 發現其餘水球，迫使他爆炸";
               wb->boomgatime = 30;
           }
        }
        // 4-4. 如果碰撞檢測失敗，就跳過這個座標，順便禁止該方向之後的所有爆炸
        if (!passCol){
            QRect prevRect;
            QList<QGraphicsItem *> prevList;
            switch(putcount%4){
            case 3: //left
                qDebug() << "[GameMMBScene] 左爆炸已被禁止";
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
                qDebug() << "[GameMMBScene] 右爆炸已被禁止";
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
                qDebug() << "[GameMMBScene] 上爆炸已被禁止";
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
                qDebug() << "[GameMMBScene] 下爆炸已被禁止";
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
                qDebug() << "[GameMMBScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 LF";
                continue;
            }
            if(putcount == 3 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::lfh);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") LFH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::lf);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") LF";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        case 2: // 右
            if(!passColrg){
                qDebug() << "[GameMMBScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 RG";
                continue;
            }
            if(putcount == 2 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::rgh);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") RGH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::rg);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") RG";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        case 1: // 上
            if(!passColup){
                qDebug() << "[GameMMBScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 UP";
                continue;
            }
            if(putcount == 1 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::uph);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") UPH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::up);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") UP";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            }
        case 0: // 下
            if(!passColdw){
                qDebug() << "[GameMMBScene] 在 (" << pt.x()+1 << "," << pt.y()+1 << ") 的水球雖可放置，但該方向已被阻擋，故跳過 DW";
                continue;
            }
            if(putcount == 0 ){
                // head
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::dwh);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") DWH";
                we->setPos(pt.x()*50, pt.y()*50);
                addItem(we);
                continue;
            } else {
                WaterExplosion *we = new WaterExplosion(nullptr, WaterExplosion::WaterExplosionDirection::dw);
                qDebug() << "[GameMMBScene] 已放置水球爆炸於 (" << pt.x()+1 << "," << pt.y() +1 << ") DW";
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
                        qDebug() << "[GameMMBScene] MaBrick 已被移動至 (-100, -100)";
                    } else if (FxBrick *fb = dynamic_cast<FxBrick*>(item)){
                        generateItem(fb->pos().x(), fb->pos().y());
                        fb->setPos(-100, -100);
                        qDebug() << "[GameMMBScene] FxBrick 已被移動至 (-100, -100)";
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
                        qDebug() << "[GameMMBScene] DropItem 已被移動至 (-100, -100)";
                    }
                    if(Player *pl = dynamic_cast<Player *>(item)){
                        if(itemTurtle){
                            qDebug() << "[GameMMBScene] 烏龜抵擋傷害";
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
                            qDebug() << "[GameMMBScene] 玩家已被水球炸到，倒數三秒爆炸";
                        }
                    }
                    if(Boss *rb = dynamic_cast<Boss *>(item)){
                        // 老大扣血
                        bossLife--;
                        if(bossLife <= 0){
                            // 玩家獲勝
                            qDebug() << "[GameMMBScene] 機器人已被水球炸到死亡";
                            rb->setPos(-100, -100);
                            requestSceneChange(sceneslist::wining);
                        }
                    }
                }
            }
        }
    }
    delete(wb);
}

void GameMMBScene::generateItem(int x, int y){
    DropItem *di;
//    // This Mode Disabled Item
//    qDebug() << "[GameMMBScene] 本模式禁用道具";
//    return;
    //
    switch(QRandomGenerator::global()->bounded(7)){
    case 6:
        di = new DropItem(nullptr, DropItem::DropItemType::moonwalk);
        qDebug() << "[GameMMBScene] 生成「月亮鞋」！";
        break;
    case 5:
        di = new DropItem(nullptr, DropItem::DropItemType::needle);
        qDebug() << "[GameMMBScene] 生成「針」！";
        break;
    case 4:
        di = new DropItem(nullptr, DropItem::DropItemType::glove);
        qDebug() << "[GameMMBScene] 生成「手套」！";
        break;
    case 3:
        di = new DropItem(nullptr, DropItem::DropItemType::power_potion);
        qDebug() << "[GameMMBScene] 生成「藥水」！";
        break;
    case 2:
        di = new DropItem(nullptr, DropItem::DropItemType::speed_shoes);
        qDebug() << "[GameMMBScene] 生成「加速」！";
        break;
    case 1:
        di = new DropItem(nullptr, DropItem::DropItemType::turtle);
        qDebug() << "[GameMMBScene] 生成「烏龜」！";
        break;
    case 0:
        di = new DropItem(nullptr, DropItem::DropItemType::waterballoon);
        qDebug() << "[GameMMBScene] 生成「水球」！";
        break;
    }
    addItem(di);
    di->setPos(x+10, y);
}

// MARK: - Tick
void GameMMBScene::tick() {
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
            qDebug() << "[GameMMBScene] Player 成功走路並走到 (" << pX << "," << pY << ")";
            qDebug() << "[GameMMBScene] 目前 Player 所屬 Cell (" << ((pX+10)/50+1) << "," << ((pY+10)/50+1) << ")";
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
                qDebug() << "[GameMMBScene] Player 成功走路並走到 (" << pX << "," << pY << ")";
                qDebug() << "[GameMMBScene] 目前 Player 所屬 Cell (" << ((pX+10)/50+1) << "," << ((pY+10)/50+1) << ")";
            }
            player->setPos(pX,pY);
        }
    }

    // 1A. 觸發機器人走路
    if ((iprX+iprY) != 0){
        if(iprX > 0){
            rX++;
            iprX--;
            robot->krg();
        } else if (iprX < 0) {
            rX--;
            iprX++;
            robot->klf();
        }
        if(iprY > 0){
            rY++;
            iprY--;
            robot->kdw();
        } else if (iprY < 0) {
            rY--;
            iprY++;
            robot->kup();
        }
        if (iprX+iprY == 0){
            qDebug() << "[GameMMBScene] Robot 成功走路並走到 (" << rX << "," << rY << ")";
            qDebug() << "[GameMMBScene] 目前 Robot 所屬 Cell (" << ((rX+10)/50+1) << "," << ((rY+10)/50+1) << ")";
        }
        robot->setPos(rX,rY);
    }

    // 2. 檢查場景 objects 處理事件
    for (QGraphicsItem *item : allItems){
        // 2-1. MaBrick
        if (MaBrick* mb = dynamic_cast<MaBrick*>(item)){
            if((mb->ipX+mb->ipY)!=0){
                // 2-1-1. 處理 MaBrick 移動事件
                // qDebug() << "[GameMMBScene] Tick 正在處理 MaBrick 移動，剩餘 (" << mb->ipX << "," << mb->ipY << ")";
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
                    qDebug() << "[GameMMBScene] MaBrick 成功移動到 (" << mb->pos().x() << "," << mb->pos().y() << ")";
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
            if(wb->isOctupusWater){
                if(wb->pos().x() > 500){
                    qDebug() << "[GameMMBScene] 丟出到右邊界，讓他回去";
                    wb->flyingX = -50;
                    if(wb->isOctupusWater){
                        wb->flyingX = -1000;
                    }
                }
                if(wb->pos().x() < 0){
                    qDebug() << "[GameMMBScene] 丟出到右邊界，讓他回去";
                    wb->flyingX = 50;
                    if(wb->isOctupusWater){
                        wb->flyingX = 1000;
                    }
                }
                if(wb->pos().y() > 450){
                    qDebug() << "[GameMMBScene] 丟出到下邊界，讓他回去";
                    wb->flyingY = -50;
                    if(wb->isOctupusWater){
                        wb->flyingY = -1000;
                    }
                }
                if(wb->pos().y() < 0){
                    qDebug() << "[GameMMBScene] 丟出到上邊界，讓他回去";
                    wb->flyingY = 50;
                    if(wb->isOctupusWater){
                        wb->flyingY = 1000;
                    }
                }
            } else {
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
            }

            // 如果是章魚水球 碰到玩家立刻爆炸
            if(wb->isOctupusWater){
                for(QGraphicsItem *item : items(QRectF(wb->pos().x()+10, wb->pos().y()+10, 10, 10))){
                    if(dynamic_cast<Player *>(item)){
                        wb->boomgatime = 3;
                    }
                }
            }

            // 檢測停止時有無碰到物品，有的話再往該方向一格
            if(abs(wb->flyingX + wb->flyingY) == 1){
                qDebug() << "[GameMMBScene] 炸彈著地偵測中";
                QRectF wbRect(wb->pos().x()+10, wb->pos().y()+10, 10, 10);
                QList<QGraphicsItem*> wbItems = items(wbRect);
                for(QGraphicsItem *item : wbItems){
                    if (dynamic_cast<InBrick *>(item) || dynamic_cast<MaBrick *>(item) || dynamic_cast<FxBrick *>(item)){
                        qDebug() << "[GameMMBScene] 水球著地位碰到方塊";
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
                } else if (wb->isOctupusWater){
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
                if(dynamic_cast<Boss *>(tItem)){
                    qDebug() << "[GameMMBScene] 物品碰到老大，自動消失";
                    di->setPos(-100, -100);
                }
                if(dynamic_cast<Player*>(tItem)){
                    qDebug() << "[GameMMBScene] 物品碰到玩家";
                    switch(di->type){
                    case DropItem::DropItemType::waterballoon:
                        waterballMaxCount++;
                        if(waterballMaxCount > 5){
                            waterballMaxCount = 5;
                        }
                        qDebug() << "[GameMMBScene] 水球++，目前有" << waterballMaxCount;
                        break;
                    case DropItem::DropItemType::glove:
                        gloveTime = 3;
                        break;
                    case DropItem::DropItemType::power_potion:
                        waterballPower++;
                        if(waterballPower > 5){
                            waterballPower = 5;
                        }
                        qDebug() << "[GameMMBScene] 威力++，目前有" << waterballPower;
                        break;
                    case DropItem::DropItemType::speed_shoes:
                        itemSpeed = true;
                        qDebug() << "[GameMMBScene] 獲得加速";
                        break;
                    case DropItem::DropItemType::turtle:
                        itemTurtle = true;
                        qDebug() << "[GameMMBScene] 獲得烏龜";
                        player->turtleEnabled = true;
                        break;
                    case DropItem::DropItemType::moonwalk:
                        moonTime = 1000;
                        break;
                    case DropItem::DropItemType::needle:
                        itemNeedle = true;
                        break;
                    }
                    qDebug() << "[GameMMBScene] 道具再見！（移動到陌生地方）";
                    di->setPos(-100, -100);
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

    // 6. 每一秒 且機器人不在走路時 讓機器人思考（本項為機器人思考）
    if((timeSec%100==0) && (iprX+iprY == 0) &&(timeSec > 500)){
        int mapNow[9][11] = {0};
        int robotCellX = 0;
        int robotCellY = 0;
        // 6-1. 製作地圖
        for(int i = 0; i < 9; i++){
            for(int j = 0 ; j < 11; j++){
                QRectF nextRect = QRectF(j*50+25, i*50+25, 10, 10);
                QList<QGraphicsItem *> itemsNext = items(nextRect);
                for(QGraphicsItem *item : itemsNext){
                    if(dynamic_cast<MaBrick *>(item)){
                        mapNow[i][j] = 1;
                        continue;
                    }
                    if(dynamic_cast<FxBrick *>(item)){
                        mapNow[i][j] = 2;
                        continue;
                    }
                    if(dynamic_cast<InBrick *>(item)){
                        mapNow[i][j] = 3;
                        continue;
                    }
                    if(dynamic_cast<Player *>(item)){
                        mapNow[i][j] = 4;
                        continue;
                    }
                    if(dynamic_cast<Boss *>(item)){
                        mapNow[i][j] = 5;
                        robotCellY = i;
                        robotCellX = j;
                        continue;
                    }
                    if(dynamic_cast<WaterBalloon *>(item)){
                        mapNow[i][j] = 6;
                        // 炸彈可能區域
                        // 上
                        for(int k = 1 ; k <= waterballPower ; k++){
                            if(i+k<9){
                                if (mapNow[i+k][j] == 0){
                                mapNow[i+k][j] = 7;
                                }
                            }
                            if(i-k>0){
                                if (mapNow[i-k][j] == 0){
                                mapNow[i-k][j] = 7;
                                }
                            }
                            if(j+k<11){
                                if (mapNow[i][j+k] == 0){
                                mapNow[i][j+k] = 7;
                                }
                            }
                            if(j-k>0){
                                if (mapNow[i][j-k]==0){
                                mapNow[i][j-k] = 7;
                                }
                            }
                        }
                        continue;
                    }
                }
            }
        }

        // 6-2. 畫出地圖
        if (true){
            qDebug() << "[GameMMBScene] Now Map:";
            for(int i = 0 ; i < 9 ; i++){
                QString temp;
                for(int j = 0 ; j < 11 ; j++){
                    temp += QString::number(mapNow[i][j]);
                    temp += " ";
                }
                qDebug().noquote() << temp;
            }
        }

        bool robotactioned = false; // 機器人是否執行過操作？
        bool haveToRun = false; // 是否要跑？
        // 6-3. 機器人判斷：若有炸彈
        if(mapNow[robotCellY][robotCellX] == 6 && !robotactioned && !haveToRun){
            qDebug() << "[Robot] 機器人這格有炸彈！！！";
            haveToRun = true;

            {
//            if(robotCellX < 11 && !robotactioned){ // 嘗試往右跑
//                qDebug() << "[Robot] 機器人嘗試往右跑？";
//                // 右邊有位置
//                if(mapNow[robotCellY][robotCellX+1] == 0){
//                    movingRobot(direction::rg);
//                    robotactioned = true;
//                    qDebug() << "[Robot] 機器人往右跑！";
//                }
//            }
//            if(robotCellY > 0 && !robotactioned){ // 嘗試往上跑
//                qDebug() << "[Robot] 機器人嘗試往上跑？";
//                // 下邊有位置
//                if(mapNow[robotCellY-1][robotCellX] == 0){
//                    movingRobot(direction::up);
//                    robotactioned = true;
//                    qDebug() << "[Robot] 機器人往上跑！";
//                }
//            }
//            if(robotCellY < 9 && !robotactioned){ // 嘗試往下跑
//                qDebug() << "[Robot] 機器人嘗試往下跑？";
//                // 上邊有位置
//                if(mapNow[robotCellY+1][robotCellX] == 0){
//                    movingRobot(direction::dw);
//                    robotactioned = true;
//                    qDebug() << "[Robot] 機器人往下跑！";
//                }
//            }
//            if(!robotactioned){
//                qDebug() << "[Robot] 機器人這格有炸彈，但無能為力！！！";
//            }
            }
        }
        for(int k = 1 ; k <= waterballPower && !robotactioned && !haveToRun ; k++){
            // 判斷威力
            // 左
            if(robotCellX-k > 0){
                if(mapNow[robotCellY][robotCellX-k] == 6){
                    qDebug() << "[Robot] 在左側偵測到炸彈";
                    haveToRun = true;
                }
            }
            // 右邊
            if(robotCellX+k < 11){
                if(mapNow[robotCellY][robotCellX+k] == 6){
                    qDebug() << "[Robot] 在右側偵測到炸彈";
                    haveToRun = true;
                }
            }
            // 上
            if(robotCellY-k > 0){
                if(mapNow[robotCellY-k][robotCellX] == 6){
                    qDebug() << "[Robot] 在上側偵測到炸彈";
                    haveToRun = true;
                }
            }
            // 下
            if(robotCellX+k < 9){
                if(mapNow[robotCellY+k][robotCellX] == 6){
                    qDebug() << "[Robot] 在下側偵測到炸彈";
                    haveToRun = true;
                }
            }
        }

        if(haveToRun && !robotactioned){
            switch(getDirectionToNearestZero(mapNow, robotCellX, robotCellY)){
            case direction::up:
                movingRobot(direction::up);
                qDebug() << "[Robot] 機器人為了逃離，往上走";
                break;
            case direction::lf:
                movingRobot(direction::lf);
                qDebug() << "[Robot] 機器人為了逃離，往左走";
                break;
            case direction::dw:
                movingRobot(direction::dw);
                qDebug() << "[Robot] 機器人為了逃離，往下走";
                break;
            case direction::rg:
                movingRobot(direction::rg);
                qDebug() << "[Robot] 機器人為了逃離，往右走";
                break;
            }
            robotSteps++;
            robotactioned = true;
        }

        // 6-3A. 機器人判斷：如果機器人在自己的 1 格之內，放炸彈
//        if(!robotactioned){
//            if(robotCellX > 0 && !robotactioned){ // 左
//                if(mapNow[robotCellY][robotCellX-1] == 4){
//                    qDebug() << "[Robot] 要放炸彈，以炸死左邊的玩家";
//                    puttingWaterballoon(((rX+10)/50+1),((rY+10)/50+1));
//                    robotSteps++;
//                    robotactioned = true;
//                }
//            }
//            if(robotCellX < 11 && !robotactioned){ // 右
//                if(mapNow[robotCellY][robotCellX+1] == 4){
//                    qDebug() << "[Robot] 要放炸彈，以炸死右邊的玩家";
//                    puttingWaterballoon(((rX+10)/50+1),((rY+10)/50+1));
//                    robotSteps++;
//                    robotactioned = true;
//                }
//            }
//            if(robotCellY > 0 && !robotactioned){ // 上
//                if(mapNow[robotCellY-1][robotCellX] == 4){
//                    qDebug() << "[Robot] 要放炸彈，以炸死上邊的玩家";
//                    puttingWaterballoon(((rX+10)/50+1),((rY+10)/50+1));
//                    robotSteps++;
//                    robotactioned = true;
//                }
//            }
//            if(robotCellY < 9 && !robotactioned){ // 下
//                if(mapNow[robotCellY+1][robotCellX] == 4){
//                    qDebug() << "[Robot] 要放炸彈，以炸死下邊的玩家";
//                    puttingWaterballoon(((rX+10)/50+1),((rY+10)/50+1));
//                    robotSteps++;
//                    robotactioned = true;
//                }
//            }
//        }

        // 6-4. 機器人判斷：無炸彈，找玩家
        if(!robotactioned){
            robotSteps++;
            switch(getDirectionToNearestPlayer(mapNow, robotCellX, robotCellY)){
            case direction::up:
                movingRobot(direction::up);
                qDebug() << "[Robot] 機器人為了找玩家，往上走";
                break;
            case direction::lf:
                movingRobot(direction::lf);
                qDebug() << "[Robot] 機器人為了找玩家，往左走";
                break;
            case direction::dw:
                movingRobot(direction::dw);
                qDebug() << "[Robot] 機器人為了找玩家，往下走";
                break;
            case direction::rg:
                movingRobot(direction::rg);
                qDebug() << "[Robot] 機器人為了找玩家，往右走";
                break;
            }
            robotactioned = true;
        }

    }

}

// MARK: - ITEM
void GameMMBScene::updateTextInfo(){
    textWater->setPlainText(QString("%1").arg(waterballMaxCount));
    textPower->setPlainText(QString("%1").arg(waterballPower));
    uiLife->setPlainText(QString("%1").arg(life));
    uiBoss->setPlainText(QString("%1/10").arg(bossLife));
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


    // Robot
    if(timeSec > 500){
        robotText->setPlainText(QString("機器人：%1 步").arg(robotSteps));
    }
}


// robot
GameMMBScene::direction GameMMBScene::getDirectionToNearestZero(int map[9][11], int robotX, int robotY) {
    const int ROWS = 9;
    const int COLS = 11;

    // 方向對應（dx, dy）
    int dx[4] = {0, 0, -1, 1};   // 上, 下, 左, 右 對應的 x 變化
    int dy[4] = {-1, 1, 0, 0};   // 上, 下, 左, 右 對應的 y 變化

    QString dirNames[4] = {"上", "下", "左", "右"};

    // BFS 初始
    bool visited[9][11] = {{false}};
    std::pair<int,int> parent[9][11];

    QQueue<std::pair<int,int>> q;

    visited[robotY][robotX] = true;
    q.enqueue({robotX, robotY});
    parent[robotY][robotX] = {-1, -1};

    while (!q.isEmpty()) {
        auto [x, y] = q.dequeue();

        // 找到安全空格 (0)，且不是起點本身
        if (map[y][x] == 0 && !(x == robotX && y == robotY)) {
            // 回溯路徑
            QStringList pathSteps;
            int tx = x, ty = y;

            while (!(tx == robotX && ty == robotY)) {
                auto [px, py] = parent[ty][tx];
                for (int d = 0; d < 4; ++d) {
                    if (px + dx[d] == tx && py + dy[d] == ty) {
                        pathSteps.prepend(dirNames[d]);
                        break;
                    }
                }
                tx = px;
                ty = py;
            }

            // 印出路徑
            QString pathStr = pathSteps.join(" → ");
            qDebug() << "[Robot - BFS Escape] " << pathStr;

            // 回傳第一步方向
            if (!pathSteps.isEmpty()) {
                QString firstStep = pathSteps[0];
                for (int d = 0; d < 4; ++d) {
                    if (dirNames[d] == firstStep) {
                        return static_cast<GameMMBScene::direction>(d);
                    }
                }
            }
            break;
        }

        // 搜尋鄰近格子
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS) continue;
            if (visited[ny][nx]) continue;

            // 判斷是否從危險區域移動
            bool fromDanger = (map[y][x] == 6 || map[y][x] == 7);
            bool toDanger = (map[ny][nx] == 6 || map[ny][nx] == 7);

            // 不允許從安全區移動到危險區（炸彈警告區）
            if (!fromDanger && toDanger) continue;

            // 牆壁禁止通行
            if (map[ny][nx] == 1 || map[ny][nx] == 2 || map[ny][nx] == 3) continue;

            visited[ny][nx] = true;
            q.enqueue({nx, ny});
            parent[ny][nx] = {x, y};
        }
    }

    qDebug() << "[Robot - BFS Escape] 找不到路徑，預設往右";
    return GameMMBScene::direction::rg;
}

GameMMBScene::direction GameMMBScene::getDirectionToNearestPlayer(int map[9][11], int robotX, int robotY) {
    const int ROWS = 9;
    const int COLS = 11;

    // 注意：x 是 column（行），y 是 row（列）
    int dx[4] = {0, 0, -1, 1};   // 上, 下, 左, 右：x 方向變化
    int dy[4] = {-1, 1, 0, 0};   // 上, 下, 左, 右：y 方向變化

    QString dirNames[4] = {"上", "下", "左", "右"};

    bool visited[9][11] = {};
    std::pair<int, int> parent[9][11];  // 儲存前一個點 (x, y)

    QQueue<std::pair<int, int>> q;
    visited[robotY][robotX] = true;
    q.enqueue({robotX, robotY});
    parent[robotY][robotX] = {-1, -1};

    while (!q.isEmpty()) {
        auto [x, y] = q.dequeue();

        // 找到玩家
        if (map[y][x] == 4) {
            // 回溯路徑
            QStringList pathSteps;
            int tx = x, ty = y;

            while (!(tx == robotX && ty == robotY)) {
                auto [px, py] = parent[ty][tx];

                for (int d = 0; d < 4; ++d) {
                    if (px + dx[d] == tx && py + dy[d] == ty) {
                        pathSteps.prepend(dirNames[d]);
                        break;
                    }
                }

                tx = px;
                ty = py;
            }

            // 印出完整路徑
            qDebug() << "[Robot] " << pathSteps.join(" → ");

            if (!pathSteps.isEmpty()) {
                QString first = pathSteps[0];
                for (int d = 0; d < 4; ++d) {
                    if (dirNames[d] == first) {
                        return static_cast<GameMMBScene::direction>(d);
                    }
                }
            }

            break;
        }

        // 搜尋四個方向
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS) continue;
            if (visited[ny][nx]) continue;

            // 可走：0, 1, 2, 3（炸彈區6/7不能走）
            if (map[ny][nx] == 6 || map[ny][nx] == 7) continue;

            visited[ny][nx] = true;
            q.enqueue({nx, ny});
            parent[ny][nx] = {x, y};
        }
    }

    qDebug() << "[Robot] 找不到路，預設往右";
    return GameMMBScene::direction::rg;
}
