#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T13:53:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IslandGame
TEMPLATE = app
CONFIG += c++14


SOURCES += \
    mainwindow.cpp \
    main.cpp \
    gameboard.cpp \
    gamestate.cpp \
    player.cpp \
    helpers.cpp \
    hexitem.cpp \
    pawnitem.cpp \
    view.cpp \
    gamepixmapitem.cpp \
    actoritem.cpp \
    transportitem.cpp \
    startdialog.cpp

HEADERS  += \
    player.hh \
    gamestate.hh \
    mainwindow.hh \
    gameboard.hh \
    hexitem.hh \
    pawnitem.hh \
    helpers.hh \
    view.hh \
    gamepixmapitem.hh \
    actoritem.hh \
    transportitem.hh \
    startdialog.hh

INCLUDEPATH += $$PWD/../GameLogic/Engine
DEPENDPATH += $$PWD/../GameLogic/Engine

CONFIG(release, debug|release) {
   DESTDIR = release
}

CONFIG(debug, debug|release) {
   DESTDIR = debug
}

LIBS += -L$$OUT_PWD/../GameLogic/Engine
LIBS += -L$$OUT_PWD/../GameLogic/Engine/$${DESTDIR}/ -lEngine

 win32 {
    copyfiles.commands += @echo NOW COPYING ADDITIONAL FILE(S) for Windows &
    copyfiles.commands += @echo from \"$$_PRO_FILE_PWD_/../GameLogic/Assets\" to  \"$$OUT_PWD\" &
    copyfiles.commands += @call xcopy \"$$_PRO_FILE_PWD_/../GameLogic/Assets\" \"$$OUT_PWD//Assets\" /i /s /e /y
}
unix:!macx {
    copyfiles.commands += echo \"NOW COPYING ADDITIONAL FILE(S) for Linux\" &&
    copyfiles.commands += echo from $$_PRO_FILE_PWD_/../GameLogic/Assets to  $$OUT_PWD &
    copyfiles.commands += cp -r $$_PRO_FILE_PWD_/../GameLogic/Assets $$DESTDIR
}
 macx {
    copyfiles.commands += @echo \"NOW COPYING ADDITIONAL FILE(S) for MacOS\" &&
    copyfiles.commands += echo from $$_PRO_FILE_PWD_/../GameLogic/Assets to  $$OUT_PWD &
    copyfiles.commands += mkdir -p $$DESTDIR/IslandGame.app/Contents/MacOS &&
    copyfiles.commands += cp -r $$_PRO_FILE_PWD_/../GameLogic/Assets $$DESTDIR &&
    copyfiles.commands += cp -r $$_PRO_FILE_PWD_/../GameLogic/Assets $$DESTDIR/IslandGame.app/Contents/MacOS/
 }

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

FORMS += \
    mainwindow.ui \
    startdialog.ui

RESOURCES += \
    images.qrc

DISTFILES +=
