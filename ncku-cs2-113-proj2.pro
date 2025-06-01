QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/elements/ClickableButton.cpp \
    src/main.cpp \
    src/GameWindow.cpp \
    src/objects/Boss.cpp \
    src/objects/DropItem.cpp \
    src/objects/FxBrick.cpp \
    src/objects/InBrick.cpp \
    src/objects/MaBrick.cpp \
    src/objects/Monster.cpp \
    src/objects/Player.cpp \
    src/objects/Robot.cpp \
    src/objects/WaterBalloon.cpp \
    src/objects/WaterExplosion.cpp \
    src/scenes/EndingScene.cpp \
    src/scenes/GameMMBScene.cpp \
    src/scenes/GameMMScene.cpp \
    src/scenes/GameMMTScene.cpp \
    src/scenes/GameMRScene.cpp \
    src/scenes/TitleScene.cpp \
    src/scenes/WiningScene.cpp

HEADERS += \
    src/GameWindow.h \
    src/core/enums.h \
    src/elements/ClickableButton.h \
    src/objects/Boss.h \
    src/objects/DropItem.h \
    src/objects/FxBrick.h \
    src/objects/InBrick.h \
    src/objects/MaBrick.h \
    src/objects/Monster.h \
    src/objects/Player.h \
    src/objects/Robot.h \
    src/objects/WaterBalloon.h \
    src/objects/WaterExplosion.h \
    src/scenes/EndingScene.h \
    src/scenes/GameMMBScene.h \
    src/scenes/GameMMScene.h \
    src/scenes/GameMMTScene.h \
    src/scenes/GameMRScene.h \
    src/scenes/TitleScene.h \
    src/scenes/WiningScene.h

FORMS +=

CONFIG += lrelease
CONFIG += sdk_no_version_check

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rcs/rcs.qrc
