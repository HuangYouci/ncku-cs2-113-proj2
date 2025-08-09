# 國立成功大學 114-1 學期計算機概論（二）專題二

## 專案簡介

這是一個使用 C++ 和 Qt 框架開發的 2D 遊戲，其靈感來自於經典的《爆爆王》（Crazy Arcade / BnB）。本專案為國立成功大學 (NCKU) 113 學年度計算機概論二課程的 Project 2。遊戲的核心玩法是玩家在地圖上放置水球，利用水球爆炸來清除障礙物、擊敗敵人並完成關卡目標。

## 開發環境

*   **語言:** C++11
*   **框架:** Qt (版本 Qt 5 或以上)
    *   **模組:** QtCore, QtGui, QtWidgets
*   **環境:** Qt Creator

## 遊戲特色

*   **多樣的遊戲模式與場景:**
    *   **標題畫面 (`TitleScene`):** 遊戲的進入點，提供開始不同遊戲模式或退出遊戲的選項。
    *   **怪物模式 (`GameMMScene`, `GameMMTScene`):** 玩家需要在一系列關卡中擊敗所有怪物才能過關。
    *   **魔王模式 (`GameMMBScene`):** 怪物模式的最終挑戰，玩家將面對一個強大的 Boss。
    *   **機器人對戰模式 (`GameMRScene`):** 在此模式中，玩家將與一個擁有智慧型 AI 的機器人進行對抗。
    *   **結局畫面 (`WiningScene`, `EndingScene`):** 根據遊戲結果（勝利或失敗）顯示對應的畫面。

*   **核心遊玩機制:**
    *   **水球系統:** 玩家可以放置水球，水球在短暫延遲後會呈十字形爆炸。爆炸可以連鎖引爆其他水球。
    *   **可互動的環境:** 地圖由多種磚塊組成：
        *   `InBrick`: 不可破壞的磚塊。
        *   `FxBrick`: 可被水球炸毀的固定磚塊。
        *   `MaBrick`: 可被水球炸毀，也可以被玩家推動。
    *   **道具系統 (`DropItem`):** 破壞 `FxBrick` 或 `MaBrick` 有機率掉落各種道具來強化玩家：
        *   **水球 (`extra_water_balloons`):** 增加可同時放置的水球上限。
        *   **藥水 (`power_potion`):** 增強水球的爆炸範圍。
        *   **加速鞋 (`speed_shoes`):** 提升玩家的移動速度。
        *   **烏龜 (`turtle`):** 提供一次性的護盾，可抵擋一次水球傷害。
        *   **針 (`needle`):** 當被水球困住時，可使用針來掙脫。
        *   **手套 (`glove`):** 讓玩家可以舉起並扔出已放置的水球。
        *   **月亮走 (`moonwalk`):** 允許玩家從螢幕的一邊穿越到另一邊。

*   **智慧型 AI 對手:**
    *   **怪物 (`Monster`):** 擁有基本的移動 AI，會在碰到障礙物時轉向，或沿著預設路徑巡邏。
    *   **機器人/魔王 (`Robot`, `Boss`):** 採用更進階的 AI 邏輯，使用廣度優先搜尋 (BFS) 演算法來實現：
        *   **追蹤玩家:** 能夠在地圖上尋找並追擊玩家。
        *   **躲避危險:** 當偵測到附近有即將爆炸的水球時，會主動尋找安全的路徑進行躲避。
        *   **策略性攻擊:** 會在適當時機（如將玩家困住或靠近玩家時）放置水球。

*   **使用者介面 (UI):**
    *   遊戲畫面下方設有 HUD，即時顯示玩家的剩餘生命、水球數量、水球威力、移動速度及其他已獲得的道具狀態。
    *   場景中包含可點擊的按鈕，用於暫停遊戲或返回標題畫面。

## 專案結構

```
ncku-cs2-113-proj2-main/
│
├── ncku-cs2-113-proj2.pro      # Qt 專案檔
│
├── rcs/
│   ├── rcs.qrc                 # Qt 資源檔 (定義所有遊戲資產)
│   └── data/
│       ├── brick/              # 磚塊圖片
│       ├── character/          # 角色圖片 (玩家, 怪物, 機器人, Boss)
│       ├── item/               # 道具圖片
│       ├── maps/               # 地圖佈局文字檔 (gameMM01.txt, testmap.txt 等)
│       ├── ui/                 # UI 元素圖片 (開始畫面, 按鈕圖示)
│       └── water_ballon.png    # 水球與爆炸效果圖
│
└── src/                        # 原始碼目錄
    │
    ├── GameWindow.cpp/.h       # 主遊戲視窗，管理場景切換
    ├── main.cpp                # 程式進入點
    │
    ├── core/
    │   └── enums.h             # 定義場景列表的枚舉
    │
    ├── elements/
    │   └── ClickableButton.cpp/.h # 可點擊的 UI 按鈕類別
    │
    ├── objects/                # 遊戲中所有動態與靜態物件
    │   ├── Boss.cpp/.h         # Boss 角色
    │   ├── DropItem.cpp/.h     # 掉落道具
    │   ├── FxBrick.cpp/.h      # 可破壞的固定磚塊
    │   ├── InBrick.cpp/.h      # 不可破壞磚塊
    │   ├── MaBrick.cpp/.h      # 可推動磚塊
    │   ├── Monster.cpp/.h      # 怪物角色
    │   ├── Player.cpp/.h       # 玩家角色
    │   ├── Robot.cpp/.h        # 機器人角色
    │   ├── WaterBalloon.cpp/.h # 水球物件
    │   └── WaterExplosion.cpp/.h # 水球爆炸效果
    │
    └── scenes/                 # 各個遊戲場景
        ├── EndingScene.cpp/.h    # 遊戲失敗場景
        ├── GameMMBScene.cpp/.h   # 怪物模式 - Boss 戰場景
        ├── GameMMScene.cpp/.h    # 怪物模式 - 關卡一
        ├── GameMMTScene.cpp/.h   # 怪物模式 - 關卡二
        ├── GameMRScene.cpp/.h    # 機器人對戰場景
        ├── TitleScene.cpp/.h     # 標題畫面場景
        └── WiningScene.cpp/.h    # 遊戲勝利場景
```

## 建置與執行

### 前置需求

1.  **Qt SDK:** 需要安裝 Qt 開發環境 (建議使用 Qt 5.12 或更新版本，或 Qt 6)。可以從 [Qt 官方網站](https://www.qt.io/download) 下載開源版本。安裝時請確保選擇了對應您開發平台的 Desktop 組件 (例如 MinGW, MSVC for Windows, or GCC for Linux/macOS)。
2.  **C++ 編譯器:** 通常會隨 Qt SDK 一併安裝 (如 MinGW, MSVC, GCC/Clang)。

### 建置步驟

1.  **開啟專案:** 使用 Qt Creator 開啟 `ncku-cs2-113-proj2.pro` 檔案。
2.  **設定建置套件 (Kit):** Qt Creator 通常會自動偵測已安裝的 Qt 版本和編譯器。如果沒有，請至「工具」->「選項」->「Kits」(或類似路徑) 設定或選擇一個有效的建置套件。
3.  **建置專案:**
    *   點擊 Qt Creator 左下角的「建置」按鈕 (錘子圖示)，或使用快捷鍵 (通常是 `Ctrl+B`)。
    *   或者，選擇「建置」菜單中的「建置專案 "ncku-cs2-113-proj2"」。
4.  **等待編譯完成:** Qt Creator 會在「編譯輸出」視窗顯示建置過程。

### 執行步驟

1.  **從 Qt Creator 執行:**
    *   建置成功後，點擊 Qt Creator 左下角的「執行」按鈕 (綠色箭頭圖示)，或使用快捷鍵 (通常是 `Ctrl+R`)。
2.  **直接執行可執行檔:**
    *   建置完成後，可執行檔通常會生成在專案目錄之外的一個建置目錄中 (例如 `build-ncku-cs2-113-proj2-main-Desktop_Qt_...`)。
    *   找到該目錄下的可執行檔並執行。

## 遊戲操作

*   **移動:** 方向鍵 (↑, ↓, ←, →)
*   **放置水球:** 空白鍵 (Space Bar)
*   **使用針筒 (道具):** Shift 鍵
*   **暫停/繼續:** 點擊遊戲畫面下方的 **暫停/播放圖示**
*   **返回主選單:** 點擊遊戲畫面下方的 **房子圖示**
*   **測試/除錯控制:**
    *   在機器人/Boss 模式中，可使用 **W, A, S, D** 鍵控制對手移動。
    *   使用 **Q** 鍵讓對手放置水球。

---

# National Cheng Kung University, Introduction to Computer Science II, 2025 Spring, Project 2

## Project Overview

This is a 2D game developed with C++ and the Qt framework, inspired by the classic "Crazy Arcade" (also known as BnB). This project is the second assignment for the "Introduction to Computer Science II" course at National Cheng Kung University (NCKU) in the 2025 spring semester. The core gameplay revolves around the player placing water balloons to clear obstacles, defeat enemies, and complete level objectives.

## Developing Environment

*   **Language:** C++11
*   **Framework:** Qt (Version 5 or newer)
    *   **Modules:** QtCore, QtGui, QtWidgets
*   **Environment:** Qt Creator

## Game Features

*   **Diverse Game Modes & Scenes:**
    *   **Title Scene (`TitleScene`):** The entry point of the game, offering options to start different game modes or exit.
    *   **Monster Mode (`GameMMScene`, `GameMMTScene`):** The player must defeat all monsters across a series of levels to progress.
    *   **Boss Mode (`GameMMBScene`):** The final challenge of Monster Mode, where the player confronts a powerful boss.
    *   **Robot Mode (`GameMRScene`):** A mode where the player faces off against an intelligent AI robot.
    *   **Ending Screens (`WiningScene`, `EndingScene`):** Displayed based on the game's outcome (victory or defeat).

*   **Core Gameplay Mechanics:**
    *   **Water Balloon System:** Players can place bombs that explode in a cross shape after a short delay. Explosions can trigger chain reactions with other balloons.
    *   **Interactive Environment:** The map consists of various types of bricks:
        *   `InBrick`: Indestructible bricks.
        *   `FxBrick`: Destructible fixed bricks that can be destroyed by explosions.
        *   `MaBrick`: Destructible bricks that can also be pushed by the player.
    *   **Item System (`DropItem`):** Destroying `FxBrick` or `MaBrick` has a chance to spawn various items that enhance the player's abilities:
        *   **Water Balloon (`extra_water_balloons`):** Increases the maximum number of bombs that can be placed simultaneously.
        *   **Power Potion (`power_potion`):** Increases the explosion range of the bombs.
        *   **Speed Shoes (`speed_shoes`):** Increases the player's movement speed.
        *   **Turtle (`turtle`):** Provides a one-time shield that blocks a single explosion.
        *   **Needle (`needle`):** Allows the player to escape when trapped in a water bubble.
        *   **Glove (`glove`):** Enables the player to pick up and throw placed bombs.
        *   **Moonwalk (`moonwalk`):** Allows the player to wrap around from one edge of the screen to the other.

*   **Intelligent AI Opponents:**
    *   **Monsters (`Monster`):** Feature basic movement AI, changing direction upon collision or patrolling along a predefined path.
    *   **Robot/Boss (`Robot`, `Boss`):** Employ advanced AI logic using Breadth-First Search (BFS) algorithm to:
        *   **Chase the Player:** Find and pursue the player on the map.
        *   **Evade Danger:** Actively search for safe paths to escape when a nearby bomb is about to explode.
        *   **Strategic Attacks:** Place bombs at opportune moments, such as when cornering the player or in close proximity.

*   **User Interface (UI):**
    *   An in-game HUD at the bottom of the screen displays the player's remaining lives, current bomb count, power level, speed, and other acquired items.
    *   Clickable UI buttons are available to pause the game or return to the title screen.

## Project Structure

```
ncku-cs2-113-proj2-main/
│
├── ncku-cs2-113-proj2.pro      # Qt Project File
│
├── rcs/
│   ├── rcs.qrc                 # Qt Resource File (defines all game assets)
│   └── data/
│       ├── brick/              # Brick sprites
│       ├── character/          # Character sprites (Player, Monster, Robot, Boss)
│       ├── item/               # Item sprites
│       ├── maps/               # Map layout text files (gameMM01.txt, testmap.txt, etc.)
│       ├── ui/                 # UI element images (start page, button icons)
│       └── water_ballon.png    # Water balloon and explosion spritesheet
│
└── src/                        # Source Code Directory
    │
    ├── GameWindow.cpp/.h       # Main game window, manages scene transitions
    ├── main.cpp                # Program entry point
    │
    ├── core/
    │   └── enums.h             # Enum for scene list
    │
    ├── elements/
    │   └── ClickableButton.cpp/.h # Clickable UI button class
    │
    ├── objects/                # All dynamic and static game objects
    │   ├── Boss.cpp/.h         # Boss character
    │   ├── DropItem.cpp/.h     # Dropped items
    │   ├── FxBrick.cpp/.h      # Fixed destructible bricks
    │   ├── InBrick.cpp/.h      # Indestructible bricks
    │   ├── MaBrick.cpp/.h      # Movable bricks
    │   ├── Monster.cpp/.h      # Monster character
    │   ├── Player.cpp/.h       # Player character
    │   ├── Robot.cpp/.h        # Robot character
    │   ├── WaterBalloon.cpp/.h # Water balloon object
    │   └── WaterExplosion.cpp/.h # Water explosion effect
    │
    └── scenes/                 # Game scenes
        ├── EndingScene.cpp/.h    # Game Over scene
        ├── GameMMBScene.cpp/.h   # Monster Mode - Boss Battle Scene
        ├── GameMMScene.cpp/.h    # Monster Mode - Level 1
        ├── GameMMTScene.cpp/.h   # Monster Mode - Level 2
        ├── GameMRScene.cpp/.h    # Robot Battle Scene
        ├── TitleScene.cpp/.h     # Title Screen Scene
        └── WiningScene.cpp/.h    # Victory Scene
```

## Build and Run

### Prerequisites

1.  **Qt SDK:** You need to install the Qt development environment (Qt 5.12 or newer is recommended). You can download the open-source version from the [official Qt website](https://www.qt.io/download). During installation, make sure to select the Desktop components for your platform (e.g., MinGW, MSVC for Windows, or GCC for Linux/macOS).
2.  **C++ Compiler:** A C++ compiler is usually included with the Qt SDK (e.g., MinGW, MSVC, GCC/Clang).

### Build Steps

1.  **Open Project:** Open the `ncku-cs2-113-proj2.pro` file using Qt Creator.
2.  **Configure Kit:** Qt Creator typically auto-detects installed Qt versions and compilers. If not, go to "Tools" -> "Options" -> "Kits" (or a similar path) to configure a valid build kit.
3.  **Build Project:**
    *   Click the "Build" button (hammer icon) in the bottom-left corner of Qt Creator or use the shortcut (usually `Ctrl+B`).
    *   Alternatively, select "Build" -> "Build Project 'ncku-cs2-113-proj2'" from the menu.
4.  **Wait for Completion:** The build process will be displayed in the "Compile Output" pane.

### Run Steps

1.  **Run from Qt Creator:**
    *   After a successful build, click the "Run" button (green play icon) in the bottom-left corner or use the shortcut (usually `Ctrl+R`).
2.  **Run Executable Directly:**
    *   The executable file is typically generated in a build directory outside the project folder (e.g., `build-ncku-cs2-113-proj2-main-Desktop_Qt_...`).
    *   Navigate to this directory and run the executable file.

## Game Controls

*   **Movement:** Arrow Keys (↑, ↓, ←, →)
*   **Place Water Balloon:** Space Bar
*   **Use Needle (Item):** Shift Key
*   **Pause/Resume:** Click the **pause/play icon** on the in-game UI.
*   **Return to Title Screen:** Click the **home icon** on the in-game UI.
*   **Testing/Debug Controls:**
    *   In Robot/Boss mode, you can use the **W, A, S, D** keys to control the opponent's movement.
    *   Use the **Q** key to make the opponent place a water balloon.
