QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src/

HEADERS += \
    ../../src/tetrixpiece.h \
    ../../src/gameboard.h

SOURCES +=  \
    ../../src/tetrixpiece.cpp \
    ../../src/gameboard.cpp \
    tst_gameboard.cpp \
    tst_tetrixpiece.cpp
