TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

SOURCES += main.cpp \
    networking.cpp \
    game.cpp

HEADERS += \
    networking.h \
    game.h

