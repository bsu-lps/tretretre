TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    run.cpp

LIBS += -lmongoclient -lboost_regex -lboost_system -lboost_thread

HEADERS += \
    graph.h \
    run.h
